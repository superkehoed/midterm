#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include "defines.h"
#include "sprite_data.h"

SpriteData *gUnusedSpriteList;

SpriteData *NewSprite()
{
	SpriteData *s;
	if(gUnusedSpriteList == NULL)
		return (SpriteData *)malloc(sizeof(SpriteData));
	else{
		s = gUnusedSpriteList;
		gUnusedSpriteList = gUnusedSpriteList->next;
		s->next = NULL;
		s->validation = gCurrentTime;
		return s;
	}
}

void FreeSprite(SpriteData *s)
{
	s->next = gUnusedSpriteList;
	gUnusedSpriteList = s;
	s->validation = -1;
}