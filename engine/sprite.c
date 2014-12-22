/******************************************************************************/
/**
 * The Sprite_T datatype is used for representing generalized sprites that
 * represent entities within the game's rendering. They are not intended to
 * store specific entity data such as position, current frame, or effects and
 * are shared between different entities that have the same appearance.
 * @author Ulysee Thompson
 */
/******************************************************************************/
#include "include.h"
#include <SDL_image.h>
#include <stdio.h>


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
		s->validation = game->currentTime;
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
void SetupSprite(Sprite_T *s, const char *file, 
				Rect *frames, short frame_num,
				Animation_T *animations, short anim_num)
{
	int i;
	s->texId = LoadTex(file);
		
	for(i = 0;i < frame_num;i++)
		s->frames[i] = frames[i];
	for(i = 0;i < anim_num;i++)
		s->animations[i]= animations[i];
	return;
}
/******************************************************************************/
Sprite_T *CreateBasicSprite(const char *filename, Vec2i frame_size)
{
	int x, y;
	int frames_per_row;
	Rect r;
	Sprite_T *s = NewSprite();
	SDL_Surface *surf;
	
	if((surf = IMG_Load(filename)) == NULL)
	{
		printf("CreateBasicSprite(): %s failed to load!", filename);
		return NULL;
	}
	//SDL_InvertSurface(surf);
	r.w = (GLfloat)frame_size.x / (GLfloat)surf->w;
	r.h = (GLfloat)frame_size.y / (GLfloat)surf->h;
	s->numFrames = 0;
	frames_per_row = surf->w / frame_size.x;
	for(y = 0;y < surf->h / frame_size.y;y++)
		for(x = 0;x < surf->w / frame_size.x;x++)
		{
			r.x = x * r.w;
			r.y = 1.0f - (y+1) * r.h;
			s->frames[x+y*frames_per_row] = r;
			s->numFrames++;
		}
	s->numAnimations = 0;
	s->texId = SurfaceToTexture(surf);
	return s;
}
/******************************************************************************/
void GetSpriteUVs(Sprite_T *s, GLuint frame, Vec2f *ll, Vec2f *ur)
{
	ll->x = s->frames[frame].x;
	ll->y = s->frames[frame].y;
	ur->x = s->frames[frame].x + s->frames[frame].w;
	ur->y = s->frames[frame].y + s->frames[frame].h;
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/