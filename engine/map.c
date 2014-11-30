#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include "include.h"

Map_T *NewMap()
{
	Map_T *map;
	if(gUnusedMapList == NULL)
		map = (Map_T *)malloc(sizeof(Map_T));
	else{
		map = gUnusedMapList;
		gUnusedMapList = gUnusedMapList->next;
	}
	memset(map->tiles, 0, sizeof(Tile_T)*MAX_MAP_WIDTH*MAX_MAP_HEIGHT);
	return map;
}

void RemoveFromTile(Entity_T *ent)
{
	Entity_T *e;
	if(ent->onTile == NULL)
		return;

	if(ent->onTile->entities == ent)
	{
		ent->onTile->entities = ent->next;
	}
	else
	{
		for(e = ent->onTile->entities;e != NULL;e = e->next)
			if(e->next == ent)
				break;
		if(e != NULL)
			e->next = ent->next;
		else{
			//Error - Entity not on tile
			return;
		}
	}
	ent->next = NULL;
	ent->onTile = NULL;
	return;
}

void AddToTile(Entity_T *ent, Tile_T *t)
{
	ent->next = t->entities;
	t->entities = ent;
	ent->onTile = t;
}

void GetTileUVs(Map_T *map, short tnum, Vec2f *ll, Vec2f *ur)
{
	Sprite_T *s = map->tilesheet;
	Rect r = s->frames[tnum-1];
	ll->x = r.x;
	ll->y = r.y;
	ur->x = r.x + r.w;
	ur->y = r.y + r.w;
}

Tile_T *TileAtPos(Map_T *map, Vec2f pos)
{
	Vec2i t;
	t.x = 128 + (int)(pos.x * SCREEN_TILE_WIDTH)/2;
	t.y = 128 + (int)(pos.y * SCREEN_TILE_HEIGHT)/2; 
	if(map->tiles[t.x][t.y].num == 0)
		return NULL;
	else
		return &map->tiles[t.x][t.y];
}