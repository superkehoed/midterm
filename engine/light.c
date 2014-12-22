/******************************************************************************/
/**
 * @file light.c
 * @author Ulysee Thompson
 */
/******************************************************************************/
#include "include.h"
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************/
Light_T *gUnusedLightList;
/******************************************************************************/
/**
 * Gets a Light_T from the unused list or allocates one if the unused list is 
 * empty.
 * @return A new, initialized Light_T
 */
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
	l->brightness = 0;
	l->color.x = 0;
	l->color.y = 0;
	l->color.z = 0;
	l->next = NULL;
	l->span = 0;
	return l;
}
/******************************************************************************/
/**
 * Places a Light_T on the unused list
 * @param l The Light_T to be freed
 */
void FreeLight(Light_T *l)
{
	l->next = gUnusedLightList;
	gUnusedLightList = l;
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/