#include "include.h"
#include <stdio.h>




/**
 *
 */
void BatchScreenEntities(RenderBatch_T *batch, Map_T *map, Vec2f center)
{
	//Constants for controlling how many tiles and how far apart they are
	const float tile_width_offset = SCREEN_TILE_WIDTH  / 2;
	const float tile_height_offset = SCREEN_TILE_HEIGHT / 2;
	Entity_T *e;
	Vec2f UV_ll, UV_ur, size, coord;
	int vertex, index;
	//GLuint shadow;

	vertex = batch->num_vertices;
	index = batch->num_indices;
	//shadow = glGetUniformLocation(s->id, "entlight");
	for(e = map->entities; e != NULL; e = e->next)
	{
		coord.x = e->pos.x - center.x;
		coord.y = e->pos.y - center.y;
		if(coord.x < -1 || coord.x > 1
		|| coord.y < -1 || coord.y > 1)
			continue;
		batch->textures[batch->num_textures++] = e->sprite->texId;
		GetEntityUVs(e, &UV_ll, &UV_ur);
		GetEntitySize(e, &size);
		//glProgramUniform1f(game->shader->id, shadow, 1.0f);
		//LL
		batch->vertices[vertex].x = coord.x;
		batch->vertices[vertex].y = coord.y;
		batch->vertices[vertex].z = (coord.y + 1.0f)/2.0f;
		batch->UVs[vertex].x = UV_ll.x;
		batch->UVs[vertex].y = UV_ll.y;	
		vertex++;
		//UL
		batch->vertices[vertex].x = coord.x;
		batch->vertices[vertex].y = coord.y +  size.y;
		batch->vertices[vertex].z = (coord.y + 1.0f)/2.0f;
		batch->UVs[vertex].x = UV_ll.x;
		batch->UVs[vertex].y = UV_ur.y;
		vertex++;
		//LR
		batch->vertices[vertex].x = coord.x + size.x;
		batch->vertices[vertex].y = coord.y;
		batch->vertices[vertex].z = (coord.y + 1.0f)/2.0f;
		batch->UVs[vertex].x = UV_ur.x;
		batch->UVs[vertex].y = UV_ll.y;
		vertex++;
		//UR
		batch->vertices[vertex].x = coord.x + size.x;
		batch->vertices[vertex].y = coord.y +  size.y;
		batch->vertices[vertex].z = (coord.y + 1.0f)/2.0f;
		batch->UVs[vertex].x = UV_ur.x;
		batch->UVs[vertex].y = UV_ur.y;
		vertex++;
	
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
	Vec2f offset;
	Vec2f UV_ll, UV_ur, pos;
	int x, y;
	Vec2f tcoord;
	int vertex, index;
	Tile_T *tile;
	vertex = batch->num_vertices;
	index = batch->num_indices;
	SET_FLAG(batch->flags, BATCHFLAG_LIGHTING);
	batch->textures[0] = map->tilesheet->texId;
	SET_FLAG(batch->flags, BATCHFLAG_UNIFORM_TEXTURE);
	//Start at the offset
	offset.x = (GLfloat)fmod(center.x, TILE_WIDTH);
	offset.y = (GLfloat)fmod(center.y, TILE_HEIGHT);
	pos.y = 1 + 2*TILE_HEIGHT;
	for(y = -2;y < SCREEN_TILE_HEIGHT + 2;y++)
	{
		tcoord.y = center.y + pos.y;
		pos.x = -1 - 2*TILE_WIDTH;
		for(x = -2;x < SCREEN_TILE_WIDTH + 2;x++)
		{
			tcoord.x = center.x + pos.x;
			if((tile = TileAtPos(map, tcoord)) != NULL 
			&& tile->num != 0){
				GetTileUVs(map, tile->num, &UV_ll, &UV_ur);
				//LL
				batch->vertices[vertex].x = pos.x - offset.x;
				batch->vertices[vertex].y = pos.y - offset.y;
				batch->vertices[vertex].z = 0.99f;
				batch->UVs[vertex].x = UV_ll.x;
				batch->UVs[vertex].y = UV_ll.y;
				vertex++;
				//UL
				batch->vertices[vertex].x = pos.x - offset.x;
				batch->vertices[vertex].y = pos.y - offset.y + TILE_HEIGHT;
				batch->vertices[vertex].z = 0.99f;
				batch->UVs[vertex].x = UV_ll.x;
				batch->UVs[vertex].y = UV_ur.y;
				vertex++;
				//LR
				batch->vertices[vertex].x = pos.x - offset.x + TILE_WIDTH;
				batch->vertices[vertex].y = pos.y - offset.y;
				batch->vertices[vertex].z = 0.99f;
				batch->UVs[vertex].x = UV_ur.x;
				batch->UVs[vertex].y = UV_ll.y;
				vertex++;
				//UR
				batch->vertices[vertex].x = pos.x - offset.x + TILE_WIDTH;
				batch->vertices[vertex].y = pos.y - offset.y + TILE_HEIGHT;
				batch->vertices[vertex].z = 0.99f;
				batch->UVs[vertex].x = UV_ur.x;
				batch->UVs[vertex].y = UV_ur.y;
				vertex++;
			}
			pos.x += TILE_WIDTH;
		}
		//Increment position
		pos.y -= TILE_HEIGHT;
	}
	//Set the number of indices and vertices after all that work
	batch->num_indices = index;
	batch->num_vertices = vertex;
}

void DrawBatch(RenderBatch_T *batch, Shader_T *s)
{
	Entity_T *e;
	GLuint texloc;
	GLint shadow;
	GLint transp;
	GLuint lightpos, lightstr, lightspan, numlights, lightcol;
	GLfloat light_positions[60];
	GLfloat light_strengths[20];
	GLfloat light_spans[20];
	GLfloat light_colors[20];
	GLuint rnd;
	GLuint lightcount = 0;
	int i;
	GLfloat UVs[] = {0.0, 0.0,
					 0.0, 1.0,
					 1.0, 0.0,
					 1.0, 1.0};
	GLuint entlight;
	if(batch->num_vertices == 0)
		return;
	glUseProgram(s->id);
	texloc = glGetUniformLocation(s->id, "tex");
	shadow = glGetUniformLocation(s->id, "shadow");
	entlight = glGetUniformLocation(s->id, "entlight");
	lightpos  = glGetUniformLocation(s->id, "lightpositions");
	lightstr  = glGetUniformLocation(s->id, "lightstrengths");
	lightspan = glGetUniformLocation(s->id, "lightspans");
	lightcol = glGetUniformLocation(s->id, "lightcolors");
	numlights = glGetUniformLocation(s->id, "numlights");
	transp = glGetUniformLocation(s->id, "transparency");
	rnd = glGetUniformLocation(s->id, "rnd");

	//Count the lights that may affect this batch and set up their data
	for(e = game->map->lights;e;e = e->nextLight)
	{
		light_positions[3*lightcount] = ((e->pos.x + e->light->offset.x - game->hero->pos.x) * SCREEN_WIDTH + SCREEN_WIDTH) / 2;
		light_positions[3*lightcount+1] = ((e->pos.y + e->light->offset.y - game->hero->pos.y) * SCREEN_HEIGHT + SCREEN_HEIGHT) / 2;
		light_positions[3*lightcount+2] = 0.0f;
		light_colors[3*lightcount] = e->light->color.x;
		light_colors[3*lightcount+1] = e->light->color.y;
		light_colors[3*lightcount+2] = e->light->color.z;
		light_spans[lightcount] = e->light->span * SCREEN_WIDTH;
		light_strengths[lightcount] = e->light->brightness;
		lightcount++;
	}
	glProgramUniform1i(s->id, texloc, 0);
	glProgramUniform1f(s->id, shadow, game->darkness);
	//Lighting uniforms
	if(IS_SET(batch->flags, BATCHFLAG_LIGHTING))
		glProgramUniform1i(s->id, numlights, lightcount);
	else
		glProgramUniform1i(s->id, numlights, 0);
	glProgramUniform1fv(s->id, lightstr, 20, light_strengths);
	glProgramUniform1fv(s->id, lightspan, 20, light_spans);
	glProgramUniform3fv(s->id,lightpos, 20, light_positions);
	glProgramUniform3fv(s->id,lightcol, 20, light_colors);
	glProgramUniform1f(s->id, transp, 0.0f);
	glProgramUniform2f(s->id, rnd, (GLfloat)Random(10000) / 10000.0f, (GLfloat)Random(10000) / 10000.0f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, batch->textures[0]);
	glEnableVertexAttribArray( s->uv_attrib );
	glEnableVertexAttribArray( s->vertex_attrib );

	for(i = 0;i < batch->num_vertices / 4;i++){
		if(!IS_SET(batch->flags, BATCHFLAG_UNIFORM_TEXTURE))
		{
			glBindTexture(GL_TEXTURE_2D, batch->textures[i]);
			glProgramUniform1f(s->id, entlight, 0.25f);
		}else{
			glProgramUniform1f(s->id, entlight, 0.0f);
		}
		//Enable vertex position
		//Set vertex data
		glBindBuffer( GL_ARRAY_BUFFER, s->vertexBuffer );
		glBufferData( GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), &(batch->vertices[i*4]), GL_DYNAMIC_DRAW );
		glVertexAttribPointer( s->vertex_attrib, 3, GL_FLOAT,GL_FALSE, 0, NULL );
		glBindBuffer( GL_ARRAY_BUFFER, s->textureBuffer );
		glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), &(batch->UVs[i*4]), GL_DYNAMIC_DRAW );
		glVertexAttribPointer( s->uv_attrib, 2, GL_FLOAT, GL_FALSE, 0, NULL );
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