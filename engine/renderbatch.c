#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include <stdio.h>
#include "include.h"


/**
 *
 */
void BatchScreenEntities(RenderBatch_T *batch, Map_T *map, Vec2f center)
{
	//Constants for controlling how many tiles and how far apart they are
	const float tile_width_offset = SCREEN_TILE_WIDTH  / 2;
	const float tile_height_offset = SCREEN_TILE_HEIGHT / 2;
	Entity_T *e;
	Vec2f UV_ll, UV_ur, size;
	int x, y, tx, ty, vertex, index;
	//GLuint shadow;

	vertex = batch->num_vertices;
	index = batch->num_indices;
	//shadow = glGetUniformLocation(game->shader->id, "entlight");
	for(y = -2;y < SCREEN_TILE_HEIGHT + 2;y++)
	{
		ty = (int)(center.y - tile_height_offset + y);
		if(ty < 0 || ty > MAX_MAP_HEIGHT)
			continue;
		for(x = -2;x < SCREEN_TILE_WIDTH + 2;x++)
		{
			tx = (int)(center.x - tile_width_offset + x);
			if(tx < 0 || tx > MAX_MAP_WIDTH)
				continue;
			if(map->tiles[tx][ty].num == 0
			|| map->tiles[tx][ty].entities == NULL)
				continue;
			for(e = map->tiles[tx][ty].entities; e; e = e->next)
			{
				batch->textures[batch->num_textures++] = e->sprite->texId;
				GetEntityUVs(e, &UV_ll, &UV_ur);
				GetEntitySize(e, &size);
//				glProgramUniform1f(game->shader->id, shadow, 1.0f);
				//LL
				batch->vertices[vertex].x = -1.0f - TILE_WIDTH/2  + x * TILE_WIDTH + e->pos.x;
				batch->vertices[vertex].y = 1.0f - TILE_HEIGHT/2 - y * TILE_HEIGHT + e->pos.y;
				batch->UVs[vertex].x = UV_ll.x;
				batch->UVs[vertex].y = UV_ll.y;		
				vertex++;
				//UL
				batch->vertices[vertex].x = -1.0f - TILE_WIDTH/2 + x * TILE_WIDTH + e->pos.x;
				batch->vertices[vertex].y = 1.0f - TILE_HEIGHT/2 +  size.y - y * TILE_HEIGHT + e->pos.y;
				batch->UVs[vertex].x = UV_ll.x;
				batch->UVs[vertex].y = UV_ur.y;
				vertex++;
				//LR
				batch->vertices[vertex].x = -1.0f - TILE_WIDTH/2 + size.x + x * TILE_WIDTH + e->pos.x;
				batch->vertices[vertex].y = 1.0f - TILE_HEIGHT/2 - y * TILE_HEIGHT + e->pos.y;
				batch->UVs[vertex].x = UV_ur.x;
				batch->UVs[vertex].y = UV_ll.y;
				vertex++;
				//UR
				batch->vertices[vertex].x = -1.0f - TILE_WIDTH/2 + size.x + x * TILE_WIDTH + e->pos.x;
				batch->vertices[vertex].y = 1.0f - TILE_HEIGHT/2 + size.y - y * TILE_HEIGHT + e->pos.y;
				batch->UVs[vertex].x = UV_ur.x;
				batch->UVs[vertex].y = UV_ur.y;
				vertex++;
			}
		}
	}
	//Set the number of indices and vertices after all that work
	batch->num_indices = index;
	batch->num_vertices = vertex;
}
/**
 * Sets up a batch for rendering the screen map using an offset and a center tile coordinate
 */
void BatchScreenMap(RenderBatch_T *batch, Map_T *map, Vec2f center)
{
	//Constants for controlling how many tiles and how far apart they are
	const float tile_width_offset = SCREEN_TILE_WIDTH  / 2;
	const float tile_height_offset = SCREEN_TILE_HEIGHT / 2;
	Vec2f UV_ll, UV_ur, pos;
	int x, y;
	Vec2f tcoord;
	int vertex, index;
	Tile_T *tile;
	vertex = batch->num_vertices;
	index = batch->num_indices;
	batch->textures[0] = map->tilesheet->texId;
	SET_FLAG(batch->flags, BATCHFLAG_UNIFORM_TEXTURE);
	//Start at the offset
	pos.x = -1 - 2*TILE_WIDTH;
	pos.y = -1 - 2*TILE_HEIGHT;
	for(y = -2;y < SCREEN_TILE_HEIGHT + 2;y++)
	{
		tcoord.y = center.y - pos.y;
		for(x = -2;x < SCREEN_TILE_WIDTH + 2;x++)
		{
			tcoord.x = center.x - pos.x;
			if((tile = TileAtPos(map, tcoord)) == NULL 
			|| tile->num == 0)
				continue;

			GetTileUVs(map, tile->num, &UV_ll, &UV_ur);
			//LL
			batch->vertices[vertex].x = -1.0f - TILE_WIDTH/2 + x * TILE_WIDTH;
			batch->vertices[vertex].y = 1.0f -TILE_HEIGHT/2- y * TILE_HEIGHT;
			batch->UVs[vertex].x = UV_ll.x;
			batch->UVs[vertex].y = UV_ll.y;
			vertex++;
			//UL
			batch->vertices[vertex].x = -1.0f - TILE_WIDTH/2 + x * TILE_WIDTH;
			batch->vertices[vertex].y = 1.0f + TILE_HEIGHT/2- y * TILE_HEIGHT;
			batch->UVs[vertex].x = UV_ll.x;
			batch->UVs[vertex].y = UV_ur.y;
			vertex++;
			//LR
			batch->vertices[vertex].x = -1.0f+ TILE_WIDTH/2 + x * TILE_WIDTH;
			batch->vertices[vertex].y = 1.0f- TILE_HEIGHT/2 - y * TILE_HEIGHT;
			batch->UVs[vertex].x = UV_ur.x;
			batch->UVs[vertex].y = UV_ll.y;
			vertex++;
			//UR
			batch->vertices[vertex].x = -1.0f+ TILE_WIDTH/2 + x * TILE_WIDTH;
			batch->vertices[vertex].y = 1.0f + TILE_HEIGHT/2 - y * TILE_HEIGHT;
			batch->UVs[vertex].x = UV_ur.x;
			batch->UVs[vertex].y = UV_ur.y;
			vertex++;
			pos.x += TILE_WIDTH;
		}
		//Increment position
		pos.y += TILE_HEIGHT;
	}
	//Set the number of indices and vertices after all that work
	batch->num_indices = index;
	batch->num_vertices = vertex;
}


void DrawBatch(RenderBatch_T *batch, Shader_T *s)
{
	GLuint texloc;
	//GLint shadow;
	int i;
	//GLuint entlight;
	if(batch->num_vertices == 0)
		return;
	glUseProgram(s->id);
	texloc = glGetUniformLocation(s->id, "tex");
	shadow = glGetUniformLocation(s->id, "shadow");
	entlight = glGetAttribLocation(game->shader->id, "entlight");
	glProgramUniform1i(s->id, texloc, 0);
	//glProgramUniform1f(s->id, shadow, game->darkness);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, batch->textures[0]);
	glEnableVertexAttribArray( game->shader->uv_attrib );
	glEnableVertexAttribArray( s->vertex_attrib );
	for(i = 0;i < batch->num_vertices / 4;i++){
		if(!IS_SET(batch->flags, BATCHFLAG_UNIFORM_TEXTURE))
		{
			glBindTexture(GL_TEXTURE_2D, batch->textures[i]);
			glProgramUniform1f(game->shader->id, entlight, 1.0f);
		}else{
			glProgramUniform1f(game->shader->id, entlight, 0.0f);
		}
		glBindBuffer( GL_ARRAY_BUFFER, s->textureBuffer );
		glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), &(batch->UVs[i*4]), GL_DYNAMIC_DRAW );
		glVertexAttribPointer( game->shader->uv_attrib, 2, GL_FLOAT, GL_FALSE, 0, NULL );
		//Enable vertex position
		//Set vertex data
		glBindBuffer( GL_ARRAY_BUFFER, s->vertexBuffer );
		glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), &(batch->vertices[i*4]), GL_DYNAMIC_DRAW );
		glVertexAttribPointer( s->vertex_attrib, 2, GL_FLOAT,GL_FALSE, 0, NULL );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4);
	}
	glDisableVertexAttribArray( s->vertex_attrib );
	glDisableVertexAttribArray( s->uv_attrib );
	//Disable vertex position
	glUseProgram((GLuint)NULL);
}

void ResetBatch(RenderBatch_T *b)
{
	b->num_indices = 0;
	b->num_vertices = 0;
	b->num_textures = 0;
	b->flags = 0;
}