/******************************************************************************/
/**
 * The Sprite_T datatype is used for representing generalized sprites that
 * represent entities within the game's rendering. They are not intended to
 * store specific entity data such as position, current frame, or effects and
 * are shared between different entities that have the same appearance.
 * @author Ulysee Thompson
 */
/******************************************************************************/
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include "defines.h"
#include "sprite.h"
/******************************************************************************/
/** Linked list storing any unused Sprite_T objects */
Sprite_T *gUnusedSpriteList;
/******************************************************************************/
/** Memory allocator for new sprites
 * @return A new sprite
 */
Sprite_T *NewSprite()
{
	Sprite_T *s;
	if(gUnusedSpriteList == NULL)
		return (Sprite_T *)malloc(sizeof(Sprite_T));
	else{
		s = gUnusedSpriteList;
		gUnusedSpriteList = gUnusedSpriteList->next;
		s->next = NULL;
		//Set the sprite's validation time to allow the system to know if a
		//sprite association is now invalid.
		s->validation = gCurrentTime;
		return s;
	}
}
/******************************************************************************/
/** Memory deallocator for sprites
 * @param s The sprite to be freed
 */
void FreeSprite(Sprite_T *s)
{
	s->next = gUnusedSpriteList;
	gUnusedSpriteList = s;
	//Clear the sprite's validation to an impossible time to make it invalid
	//For any entities that may still have pointers to it.
	s->validation = -1;
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/