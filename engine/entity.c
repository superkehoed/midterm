#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include "include.h"

//Globals
Entity_T *gUnusedEntityList;

void FreeEntity(Entity_T *ent)
{
	ent->next = gUnusedEntityList;
	gUnusedEntityList = ent;
}

void InitializeEntity(Entity_T *ent)
{
	ent->next = NULL;
	ent->nextFrame = -1;
	ent->currentFrame = 0;
	ent->sprite = NULL;
	ent->type = 0;
}

Entity_T *NewEntity()
{
	Entity_T *ent;
	if(gUnusedEntityList == NULL)
		ent = (Entity_T *)malloc(sizeof(Entity_T));
	else{
		ent = gUnusedEntityList;
		gUnusedEntityList = gUnusedEntityList->next;
	}
	//Initialize it before sending it out
	InitializeEntity(ent);
	return ent;
}

void DrawEntity(Entity_T *ent)
{
	if(ent->sprite == NULL)
		return;
	//Draw the entity
	switch(ent->type){
	case ENTYPE_EFFECT:
		break;
	case ENTYPE_CREATURE:
		break;
	case ENTYPE_OBJECT:
		break;
	case ENTYPE_GRAPHIC:
		if(IS_SET(ent->display.flags, GRAPHFLAG_FULLSCREEN))
			
		break;
	}
}

void MoveEntity(Entity_T *ent, Map_T *map, Vec2i pos)
{

	if(pos.x < 0 || pos.x > MAX_MAP_WIDTH
	|| pos.y < 0 || pos.y > MAX_MAP_HEIGHT){
		//Error - Position out of bounds
		return;
	}

	if(ent->onTile != NULL)
		RemoveFromTile(ent);
	if(map->tiles[pos.x][pos.y].num == 0)
	{
		//Error - Moving to nonexistent tile
	}
	else if(IS_SET(map->tiles[pos.x][pos.y].flags, TILEFLAG_NO_PASS))
	{
		//Error - Moving to nopass tile
	}
	AddToTile(ent, &map->tiles[pos.x][pos.y]);
}