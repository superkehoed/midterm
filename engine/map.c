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

void GetTileUVs(Map_T *map, short tnum, Vec2f *ul, Vec2f *lr)
{
	Sprite_T *s = map->tilesheet;
	Rect r = s->frames[tnum-1];
	ul->x = r.x;
	ul->y = r.y;
	lr->x = r.x + r.w;
	lr->y = r.y + r.w;
}