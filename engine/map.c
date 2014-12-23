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
	memset(map, 0, sizeof(Map_T));
	memset(map->tiles, 0, sizeof(Tile_T)*MAX_MAP_WIDTH*MAX_MAP_HEIGHT);
	return map;
}

void RemoveFromMap(Entity_T *ent)
{
	Entity_T *e;
	
	if(ent->onMap == NULL)
		return;
	if(ent->onMap->entities == ent)
	{
		ent->onMap->entities = ent->next;
	}
	else
	{
		for(e = ent->onMap->entities;e != NULL;e = e->next)
			if(e->next == ent)
				break;
		if(e != NULL)
			e->next = ent->next;
		else{
			//Error - Entity not on map
			return;
		}
	}
	if(ent->light != NULL){
		if(ent->onMap->lights == ent)
		{
			ent->onMap->lights = ent->nextLight;
		}
		else
		{
			for(e = ent->onMap->lights;e != NULL;e = e->nextLight)
				if(e->nextLight == ent)
					break;
			if(e != NULL)
				e->nextLight = ent->nextLight;
			else{
				//Error - Entity not on map
				return;
			}
		}
	}
	cpSpaceRemoveShape(ent->onMap->space, ent->shape);
	cpSpaceRemoveBody(ent->onMap->space, ent->body);
	ent->next = NULL;
	ent->nextLight = NULL;
	ent->onMap = NULL;
	return;
}

void AddToMap(Entity_T *ent, Map_T *map)
{
	if(ent->onMap != NULL)
		RemoveFromMap(ent);
	ent->next = map->entities;
	map->entities = ent;
	if(ent->light != NULL){
		ent->nextLight = map->lights;
		map->lights = ent;
	}
	ent->onMap = map;
	cpSpaceAddShape(ent->onMap->space, ent->shape);
	cpSpaceAddBody(ent->onMap->space, ent->body);
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
	t.y = 128 - (int)(pos.y * SCREEN_TILE_HEIGHT)/2; 
	if(t.x < 0 || t.y < 0 || map->tiles[t.x][t.y].num == 0)
		return NULL;
	else
		return &map->tiles[t.x][t.y];
}

Vec2f PosAtTile(Vec2f tile)
{
	Vec2f result;
	result.x = 2 * (tile.x - 128)/SCREEN_TILE_WIDTH;
	result.y = 2 * -(tile.y - 128)/SCREEN_TILE_HEIGHT;
	return result;
}