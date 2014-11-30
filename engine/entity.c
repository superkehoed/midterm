#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include <stdio.h>
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
	memset(ent, 0, sizeof(Entity_T));
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
	}
	else if(IS_SET(tile->flags, TILEFLAG_NO_PASS))
	{
		//Error - Moving to nopass tile
	}
	if(ent->onMap != map){
		ent->next = map->entities;
		map->entities = ent;
		ent->onMap = map;
	}
}

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
	e->size.x = TILE_WIDTH;
	e->size.y = 1.5f * TILE_HEIGHT;

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
	return e;
}

void DefaultState(Entity_T *e)
{
	//TODO: Handle death if they need to linger here
	e->state = ENTSTATE_STAND;
	//TODO: Hackish, resolve later.
	StartAnimation(e, e->dir + ANIM_STAND_DOWN);
}

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

int CheckCollisions(Entity_T *e)
{
	//Check to see if the entity has arrived at a blocking tile
	//Check if the entity has collided with any other entities on the map.
		//Check to see if the entity is considered Damaging or Nopass.
	return 0;
}

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
			e->velocity = pusher->velocity;
			return true;
		}
	}
}

/**
 * Updates the entity and returns false if the entity is destroyed
 */
bool UpdateEntity(Entity_T *e, GLuint delta)
{
	Vec3f pos = e->pos;
	if(e->Think != NULL && e->nextThink <= game->currentTime)
		(e->Think)(e, delta);
	if(e->velocity.x != 0)
		e->pos.x += (e->velocity.x * delta / 1000);
	if(e->velocity.y != 0)
		e->pos.y += (e->velocity.y * delta / 1000);
	if(e->velocity.z != 0)
		e->pos.z += (e->velocity.z * delta / 1000);	

	//If they collided with anything, return them to their previous position and reset their velocity
	if(CheckCollisions(e) != 0){
		e->pos = pos;
		e->velocity.x = 0;
		e->velocity.y = 0;
		e->velocity.z = 0;
	}
	if(e->state == ENTSTATE_WALK)
	{
		e->pos.x = 0;
		e->pos.y = 0;
		e->velocity.x = 0;
		e->velocity.y = 0;
		DefaultState(e);
	}
	//Update their animation after
	if(e->nextFrame <= game->currentTime)
	{
		UpdateAnimation(e);
	}
	return true;
}


void GetEntitySize(Entity_T *e, Vec2f *size)
{
	//TODO: Add code to handle size effects here
	size->x = e->size.x;
	size->y = e->size.y;
}

void GetEntityUVs(Entity_T *e, Vec2f *ll, Vec2f *ur)
{
	GetSpriteUVs(e->sprite, 
		e->sprite->animations[e->currentAnimation].frames[e->currentFrame], ll, ur);
}

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

void WalkEntity(Entity_T *e, int direction)
{
	const float WALK_SPEED = TILE_WIDTH;
	switch(direction){
	case ENTDIR_UP:
		e->velocity.y = WALK_SPEED;
		if(e->state != ENTSTATE_WALK || e->dir == ENTDIR_DOWN)
			StartAnimation(e, ANIM_WALK_UP);
		break;
	case ENTDIR_DOWN:
		e->velocity.y = -WALK_SPEED;
		if(e->state != ENTSTATE_WALK || e->dir == ENTDIR_UP)
			StartAnimation(e, ANIM_WALK_DOWN);
		break;
	case ENTDIR_LEFT:
		e->velocity.x = -WALK_SPEED;
		if(e->state != ENTSTATE_WALK || e->dir == ENTDIR_RIGHT)
			StartAnimation(e, ANIM_WALK_LEFT);
		break;
	case ENTDIR_RIGHT:
		e->velocity.x = WALK_SPEED;
		if(e->state == ENTSTATE_WALK || e->dir == ENTDIR_LEFT)
			StartAnimation(e, ANIM_WALK_RIGHT);
		break;
	}
	e->dir = direction;
	e->state = ENTSTATE_WALK;
}


void MonsterHunt(Entity_T *e, GLuint delta)
{
	bool frozen = false;
	Vec2f offset;
	Vec2i dv[4] = {
		{ 0, -1},
		{ -1, 0},
		{ 1, 0},
		{ 0, 1}
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

	if(!frozen){
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
	e->size.x = TILE_WIDTH;
	e->size.y = 1.5f * TILE_HEIGHT;

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
