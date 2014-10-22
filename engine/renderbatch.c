#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include "include.h"



/**
 * Sets up a batch for rendering the screen map using an offset and a center tile coordinate
 */
void BatchScreenMap(RenderBatch_T *batch, Map_T *map, Vec2i center)
{
	//Constants for controlling how many tiles and how far apart they are
	const int tile_width_offset = SCREEN_TILE_WIDTH / 2 + 1;
	const int tile_height_offset = SCREEN_TILE_HEIGHT / 2 + 1;
	Vec2f UV_ul, UV_lr;
	int x, y, tx, ty, vertex, index;
	short tile;

	vertex = batch->num_vertices;
	index = batch->num_indices;
	for(y = 0;y < SCREEN_TILE_HEIGHT + 2;y++)
	{
		ty = center.y - tile_height_offset + y;
		if(ty < 0 || ty > MAX_MAP_HEIGHT)
			continue;
		for(x = 0;x < SCREEN_TILE_WIDTH + 2;x++)
		{
			tx = center.x - tile_width_offset + y;
			if(tx < 0 || tx > MAX_MAP_WIDTH)
				continue;
			if((tile = map->tiles[tx][ty].num) == 0)
				continue;

			GetTileUVs(map, tile, &UV_ul, &UV_lr);
			//LL
			batch->vertices[vertex].x = -1.0f - TILE_WIDTH + x * TILE_WIDTH;
			batch->vertices[vertex].y = 1.0f - y * TILE_HEIGHT;
			batch->UVs[vertex].x = UV_ul.x;
			batch->UVs[vertex].y = UV_lr.y;
			batch->indices[index++] = vertex++;
			//UL
			batch->vertices[vertex].x = -1.0f - TILE_WIDTH + x * TILE_WIDTH;
			batch->vertices[vertex].y = 1.0f + TILE_HEIGHT - y * TILE_HEIGHT;
			batch->UVs[vertex].x = UV_ul.x;
			batch->UVs[vertex].y = UV_ul.y;
			batch->indices[index++] = vertex++;
			//LR
			batch->vertices[vertex].x = -1.0f + x * TILE_WIDTH;
			batch->vertices[vertex].y = 1.0f - y * TILE_HEIGHT;
			batch->UVs[vertex].x = UV_lr.x;
			batch->UVs[vertex].y = UV_lr.y;
			batch->indices[index++] = vertex++;
			//UR
			batch->vertices[vertex].x = -1.0f + x * TILE_WIDTH;
			batch->vertices[vertex].y = 1.0f + TILE_HEIGHT - y * TILE_HEIGHT;
			batch->UVs[vertex].x = UV_lr.x;
			batch->UVs[vertex].y = UV_ul.y;
			batch->indices[index++] = vertex++;
			batch->indices[index++] = RESET_INDEX;
		}
	}
	//Set the number of indices and vertices after all that work
	batch->num_indices = index;
	batch->num_vertices = vertex;
}