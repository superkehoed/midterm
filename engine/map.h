/******************************************************************************/
/**
 @file map.h
 */
/******************************************************************************/
#ifndef __MAP_H
#define __MAP_H
/******************************************************************************/
#define TILEFLAG_NO_PASS				0x1 /**< Tile cannot be passed */
#define TILEFLAG_NO_HERO				0x2 /**< Hero cannot pass tile */
#define TILEFLAG_NO_MONSTER				0x3 /**< Monsters cannot pass tile */
/******************************************************************************/
typedef struct Tile_T
{
	short num; /**< The tilenum, used for drawing the correct tile from sheet */
	long flags;/**< Flags that apply to the tile */
	struct Entity_T *entities; /**< Linked list of entities on tile */
}Tile_T;
/******************************************************************************/
typedef struct Map_T
{
	struct cpSpace *space; /**< The map's cpSpace for collision detection */
	struct Entity_T *entities; /**< The entities on the map */
	struct Map_T *next; /**< The next map in the linked list */
	struct Sprite_T *tilesheet; /**< A sprite representing the tilesheet */
	Vec2f	start; /**< The start point of the tile */
	Tile_T tiles[256][256]; /**< An array of the map's tiles */
	Event_T	*events; /**< A linked list of any events that occur on this map */
	struct Entity_T *lights; /**< A linked list of any light-bearing entities on the current map */
}Map_T;
/******************************************************************************/
extern Map_T *gUnusedMapList; /**< The unused map list */
void FreeMap(Map_T *map); /**< Free the map back to the unused list*/
Map_T *StartMap(); /**< Creates a basic starting game map */
Map_T *BlankMap(); /**< Creates a blank map using NewMap */
Map_T *NewMap(); /**< Provides a pointer to a new map for the engine */
void MoveEntity(struct Entity_T *ent, Map_T *map, Vec2f pos); /**< Moves the entity to a new position on the map */
void RemoveFromMap(struct Entity_T *ent); /**< Removes the entity from a map */
void AddToMap(struct Entity_T *ent, Map_T *map); /**< Adds the entity to a map */
void GetTileUVs(Map_T *map, short tnum, Vec2f *ul, Vec2f *lr); /**< Provides the UV coordinates for a tilenum */
Tile_T *TileAtPos(Map_T *map, Vec2f pos); /**< Get the tile at that position  or NULL */
Vec2f PosAtTile(Vec2f tile); /**< Get the position at a tile coordinate */
/******************************************************************************/
/******************************************************************************/
#endif //__MAP_H
/******************************************************************************/