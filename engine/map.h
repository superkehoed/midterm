/******************************************************************************/
/**
 @file map.h
 */
/******************************************************************************/
#ifndef __MAP_H
#define __MAP_H
/******************************************************************************/
#define TILEFLAG_NO_PASS				0x1
/******************************************************************************/
typedef struct Tile_T
{
	short num;
	long flags;
	struct Entity_T *entities;
}Tile_T;
/******************************************************************************/
typedef struct Map_T
{
	struct Map_T *next;
	struct Sprite_T *tilesheet;
	Vec2i	start;
	Tile_T tiles[256][256];
	Event_T	*events;
}Map_T;
/******************************************************************************/
extern Map_T *gUnusedMapList;
void FreeMap(Map_T *map);
Map_T *StartMap();
Map_T *BlankMap();
Map_T *NewMap();
void MoveEntity(struct Entity_T *ent, Map_T *map, Vec2i pos);
void RemoveFromTile(struct Entity_T *ent);
void AddToTile(struct Entity_T *ent, Tile_T *t);
void GetTileUVs(Map_T *map, short tnum, Vec2f *ul, Vec2f *lr);
/******************************************************************************/
/******************************************************************************/
#endif //__MAP_H
/******************************************************************************/