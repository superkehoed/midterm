/******************************************************************************/
/**
 * @file light.c
 * @author Ulysee Thompson
 */
/******************************************************************************/
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <GL/GLU.h>
#include <stdio.h>
#include <stdlib.h>
#include "include.h"

Light_T *gUnusedLightList;

Light_T *NewLight()
{
	Light_T *l;
	if(gUnusedLightList == NULL)
		l = (Light_T *)malloc(sizeof(Light_T));
	else{
		l = gUnusedLightList;
		gUnusedLightList = gUnusedLightList->next;
	}
	//Initialize it before sending it out
	return l;
}

void FreeLight(Light_T *l)
{
}