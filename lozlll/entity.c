#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include "globals.h"
#include "sprite.h"
#include "graphic.h"
#include "entity.h"

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
	case ENTYPE_ALIVE:
		break;
	case ENTYPE_OBJECT:
		break;
	case ENTYPE_GRAPHIC:
		if(IS_SET(ent->display.flags, GRAPHFLAG_FULLSCREEN))
			
		break;
	}
}