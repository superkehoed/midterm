/******************************************************************************/
/******************************************************************************/
#include "include.h"
#include <stdio.h>

/******************************************************************************/
//Globals
Entity_T *gUnusedEntityList;
/******************************************************************************/
/**
 * Frees the entity and adds it to the unused list
 * @param ent The entity to be freed
 */
void FreeEntity(Entity_T *ent)
{
	ent->next = gUnusedEntityList;
	gUnusedEntityList = ent;
}
/******************************************************************************/
/**
 * Initializes the entity with whatever steps are necessary
 * @param ent The entity to be initialized
 */
void InitializeEntity(Entity_T *ent)
{
	memset(ent, 0, sizeof(Entity_T));
}
/******************************************************************************/
/**
 * Retrieves an entity from the unused list or allocates one if the list is empty
 * @return A new, initialized entity
 */
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
	ent->next = NULL;
	ent->nextLight = NULL;

	return ent;
}
/******************************************************************************/
/**
 * Draws an entity on the screen
 * @param ent The entity to be drawn
 */
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
/******************************************************************************/
/**
 * Instantly moves an entity to a specific position on a map
 * @param ent The entity to be moved
 * @param map The map to move the entity on
 * @param pos The map position to move the entity to
 */
void MoveEntity(Entity_T *ent, Map_T *map, Vec2f pos)
{
	const GLfloat map_width_limit = MAX_MAP_WIDTH / 2 * TILE_WIDTH;
	const GLfloat map_height_limit = MAX_MAP_HEIGHT / 2 * TILE_HEIGHT;
	Tile_T *tile;
	if(pos.x < -map_width_limit || pos.x > map_width_limit
	|| pos.y < -map_height_limit || pos.y > map_height_limit){
		//Error - Position out of bounds
		return;
	}

	if((tile = TileAtPos(map, pos)) == NULL)
	{
		//Error - Moving to nonexistent tile
		return;
	}
	else if(IS_SET(tile->flags, TILEFLAG_NO_PASS))
	{
		//Error - Moving to nopass tile
		return;
	}
	ent->body->p.x = pos.x;
	ent->body->p.y = pos.y;
	if(ent->onMap != map)
		AddToMap(ent, map);
}
/******************************************************************************/
/**
 * Creates and sets up the hero entity
 * @return The hero entity created
 */
Entity_T *SetupHero()
{
	int i, x;
	const GLfloat SPRITE_WIDTH = 32.0f / 128;
	const GLfloat SPRITE_HEIGHT = 48.0f / 192;
	Rect frames[20];
	Animation_T animations[MAX_ANIMATIONS];
	Entity_T *e = NewEntity();
	e->currentAnimation = 0;
	e->currentFrame = 0;
	e->sprite = NewSprite();
	e->body = cpBodyNew(1.0f, 1.0f);
	e->size.x = TILE_WIDTH;
	e->size.y = 1.5f * TILE_HEIGHT;
	e->shape = cpBoxShapeNew(e->body, e->size.x, e->size.y);
	

	for(i = 0;i < 20;i++)
	{
		frames[i].x = (i % 4) * SPRITE_WIDTH;
		frames[i].y = 1.0f - (i/4+1) * SPRITE_HEIGHT;
		frames[i].w = SPRITE_WIDTH;
		frames[i].h = SPRITE_HEIGHT;
	}
	
	for(x = 0;x < 4;x++)
	{
		animations[x].numFrames = 1;
		animations[x].frames[0] = x*4;
		animations[x].frameLengths[0] = 100;
		animations[x].flags = ANIMFLAG_NONE;
	}

	for(x = 4;x < 8;x++){
		animations[x].numFrames = 4;
		for(i = 0;i < 4;i++){
			animations[x].frames[i] = (x-4)*4+i;
			animations[x].frameLengths[i] = 100;
		}
		animations[x].flags = ANIMFLAG_NONE;
	}
	SetupSprite(e->sprite, "hero.png", frames, 20, animations, 8);
	//Set up the hero's inner light
	e->light = NewLight();
	e->light->brightness = 0.8f;
	e->light->color.x = 1.0f;
	e->light->color.y = .95f;
	e->light->color.z = 0.5f;
	e->light->span = TILE_WIDTH / 2;
	e->light->offset.x = TILE_WIDTH / 2;
	e->light->offset.y = 0;
	SET_FLAG(e->flags, ENTFLAG_LIGHT);
	e->next = NULL;
	return e;
}
/******************************************************************************/
void DefaultState(Entity_T *e)
{
	GLint anim;
	//TODO: Handle death if they need to linger here
	e->state = ENTSTATE_STAND;
	//TODO: Hackish, resolve later.
	switch(e->dir){
	case ENTDIR_UP: anim = ANIM_STAND_UP; break;
	case ENTDIR_DOWN: anim = ANIM_STAND_DOWN; break;
	case ENTDIR_LEFT: anim = ANIM_STAND_LEFT; break;
	case ENTDIR_RIGHT: anim = ANIM_STAND_RIGHT; break;
	}
	StartAnimation(e, anim);
}
/******************************************************************************/
void UpdateAnimation(Entity_T *e)
{
	if(++e->currentFrame >= e->sprite->animations[e->currentAnimation].numFrames){
		if(!IS_STANDING(e) && !IS_WALKING(e))
		{
			DefaultState(e);
		}
		e->currentFrame = 0;
		
	}
	e->nextFrame = game->currentTime 
		+ e->sprite->animations[e->currentAnimation].frameLengths[e->currentFrame];
}
/******************************************************************************/
int CheckCollisions(Entity_T *e)
{
	//Check to see if the entity has arrived at a blocking tile
	//Check if the entity has collided with any other entities on the map.
		//Check to see if the entity is considered Damaging or Nopass.
	return 0;
}
/******************************************************************************/
/**
 * Attempts to have the pusher push e in a direction if possible
 */
bool PushEntity(Entity_T *e, Entity_T *pusher, GLuint dir)
{
	//Does the entity have default behavior?
	if(e->Pushed != NULL)
		return (*e->Pushed)(e, pusher, dir);
	else
	{
		if(!IS_SET(e->flags, ENTFLAG_PUSHABLE))
			return false;
		else{
			WalkEntity(e, dir);
			e->body->v = pusher->body->v;
			return true;
		}
	}
}
/******************************************************************************/
/**
 * Updates the entity and returns false if the entity is destroyed
 */
bool UpdateEntity(Entity_T *e, GLuint delta)
{
	Vec3f pos = e->pos;
	if(e->Think != NULL && e->nextThink <= game->currentTime)
		(e->Think)(e, delta);
	e->pos.x = (GLfloat)e->body->p.x;
	e->pos.y = (GLfloat)e->body->p.y;

	//Update their animation after
	if(e->nextFrame <= game->currentTime)
	{
		UpdateAnimation(e);
	}
	return true;
}
/******************************************************************************/

void GetEntitySize(Entity_T *e, Vec2f *size)
{
	GLfloat scale = 1.0f;
	if(IS_SET(e->flags, ENTFLAG_BOSS))
		scale *= 3.0f;
	//Handle size effects here
	size->x = e->size.x * scale;
	size->y = e->size.y * scale;
	
}
/******************************************************************************/
void GetEntityUVs(Entity_T *e, Vec2f *ll, Vec2f *ur)
{
	GetSpriteUVs(e->sprite, 
		e->sprite->animations[e->currentAnimation].frames[e->currentFrame], ll, ur);
}
/******************************************************************************/
void StartAnimation(Entity_T *e, GLuint anim)
{
	if(anim >= e->sprite->numAnimations
	|| e->sprite->animations[anim].numFrames  < 1){
		printf("StartAnimation(): Attempted to start an animation that does not exist.\n");
		return;
	}
	e->currentAnimation = anim;
	e->currentFrame = 0;
	e->nextFrame = game->currentTime + e->sprite->animations[anim].frameLengths[0];
}
/******************************************************************************/
void WalkEntity(Entity_T *e, int direction)
{
	const float WALK_SPEED = TILE_WIDTH;
	switch(direction){
	case ENTDIR_UP:
		e->body->v.y = WALK_SPEED;
		if(e->state != ENTSTATE_WALK || e->dir == ENTDIR_DOWN)
			StartAnimation(e, ANIM_WALK_UP);
		break;
	case ENTDIR_DOWN:
		e->body->v.y = -WALK_SPEED;
		if(e->state != ENTSTATE_WALK || e->dir == ENTDIR_UP)
			StartAnimation(e, ANIM_WALK_DOWN);
		break;
	case ENTDIR_LEFT:
		e->body->v.x = -WALK_SPEED;
		if(e->state != ENTSTATE_WALK || e->dir == ENTDIR_RIGHT)
			StartAnimation(e, ANIM_WALK_LEFT);
		break;
	case ENTDIR_RIGHT:
		e->body->v.x = WALK_SPEED;
		if(e->state != ENTSTATE_WALK || e->dir == ENTDIR_LEFT)
			StartAnimation(e, ANIM_WALK_RIGHT);
		break;
	}
	e->dir = direction;
	e->state = ENTSTATE_WALK;
}
/******************************************************************************/

void MonsterHunt(Entity_T *e, GLuint delta)
{
	bool frozen = false;
	Vec2f offset;
	Vec2i dv[4] = {
		{ 0, 1},
		{ -1, 0},
		{ 1, 0},
		{ 0, -1}
	};
	GLuint dirs[4];
	GLuint max_dirs = 0;
	GLuint num;
	int i;
	for(i = 0;i < 4;i++)
		dirs[i] = 0;
	offset.x = e->pos.x - game->hero->pos.x;
	offset.y = e->pos.y - game->hero->pos.y;
	//Check if the hero is facing them
	if(offset.y > 0){
		if(game->hero->dir == ENTDIR_DOWN)
			frozen = true;
		else{
			dirs[ENTDIR_UP] += 5;
			max_dirs +=5;
		}
	}else if(offset.y < 0){
		if(game->hero->dir == ENTDIR_UP)
			frozen = true;
		else{
			dirs[ENTDIR_DOWN] += 5;
			max_dirs +=5;
		}
	}

	if(offset.x > 0){
		if(game->hero->dir == ENTDIR_RIGHT)
			frozen = true;
		else{
			dirs[ENTDIR_LEFT] += 5;
			max_dirs +=5;
		}
	}else if(offset.x < 0){
		if(game->hero->dir == ENTDIR_LEFT)
			frozen = true;
		else{
			dirs[ENTDIR_RIGHT] += 5;
			max_dirs +=5;
		}
	}

	if(!frozen && max_dirs > 0){
		num = Random(max_dirs);
		//Weighted randoms
		for(i = 0;i < 4;i++)
			if(num <= dirs[i])
				break;
			else
				num -= dirs[i];
		if(i != 4){
			offset.x = e->pos.x + TILE_WIDTH * dv[i].x;
			offset.y = e->pos.y + TILE_HEIGHT * dv[i].y;
			MoveEntity(e, e->onMap, offset);
		}
	}
	e->nextThink = game->currentTime + Random(1000) + 500;
}
/******************************************************************************/
Entity_T *SetupMonster(int monster)
{
	int i, x;
	const GLfloat SPRITE_WIDTH = 32.0f / 128;
	const GLfloat SPRITE_HEIGHT = 48.0f / 192;
	Rect frames[20];
	Animation_T animations[MAX_ANIMATIONS];
	Entity_T *e = NewEntity();
	e->currentAnimation = 0;
	e->currentFrame = 0;
	e->sprite = NewSprite();
	e->body = cpBodyNew(1.0f, 1.0f);
	e->size.x = 3* TILE_WIDTH;
	e->size.y = 3 * 1.5f * TILE_HEIGHT;
	e->shape = cpBoxShapeNew(e->body, e->size.x, e->size.y);
	
	for(i = 0;i < 20;i++)
	{
		frames[i].x = (i % 4) * SPRITE_WIDTH;
		frames[i].y = 1.0f - (i/4+1) * SPRITE_HEIGHT;
		frames[i].w = SPRITE_WIDTH;
		frames[i].h = SPRITE_HEIGHT;
	}
	
	for(x = 0;x < 4;x++)
	{
		animations[x].numFrames = 1;
		animations[x].frames[0] = x*4;
		animations[x].frameLengths[0] = 100;
		animations[x].flags = ANIMFLAG_NONE;
	}

	for(x = 4;x < 8;x++){
		animations[x].numFrames = 4;
		for(i = 0;i < 4;i++){
			animations[x].frames[i] = (x-4)*4+i;
			animations[x].frameLengths[i] = 100;
		}
		animations[x].flags = ANIMFLAG_NONE;
	}
	//TODO: Kludgy as heck, redo later.
	switch(monster){
	case 0:
		SetupSprite(e->sprite, "monster0.png", frames, 20, animations, 8);
		break;
	case 1:
		SetupSprite(e->sprite, "monster1.png", frames, 20, animations, 8);
		break;
	}
	return e;
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/