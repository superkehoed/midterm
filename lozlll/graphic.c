#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include "include.h"

void DrawGraphic(Entity_T *graphic)
{
	Rect r;
	GLfloat verts[8];
	if(graphic->type != ENTYPE_GRAPHIC){
		//Generate an error
		return;
	}
	
	if(IS_SET(graphic->display.flags, GRAPHFLAG_FULLSCREEN)){
		verts[0] = 0;
		verts[1] = 0;
		verts[2] = SCREEN_WIDTH;
		verts[3] = 0;
		verts[4] = SCREEN_WIDTH;
		verts[5] = SCREEN_HEIGHT;
		verts[6] = 0;
		verts[7] = SCREEN_HEIGHT;
	}else{
		verts[0] = graphic->pos.x;
		verts[1] = 0;
		verts[2] = graphic->pos.x + graphic->sprite->frames[graphic->currentFrame].w;
		verts[3] = 0;
		verts[4] = verts[2];
		verts[5] = graphic->pos.y + graphic->sprite->frames[graphic->currentFrame].y;
		verts[6] = 0;
		verts[7] = verts[5];
	}

	//Create VBO
	glBindBuffer( GL_ARRAY_BUFFER, game->foreGraphicsVBO );
	glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), verts, GL_STATIC_DRAW );

	//Enable vertex position
	glEnableVertexAttribArray( game->shader->vertex_attrib );
	//Set vertex data
	glBindBuffer( GL_ARRAY_BUFFER, game->foreGraphicsVBO );
	glVertexAttribPointer( game->shader->vertex_attrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL );
	//Set index data and render
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, game->shader->IBO );
	glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );
	//Disable vertex position
	glDisableVertexAttribArray( game->shader->vertex_attrib );
}

void SetupGraphic(Entity_T *ent, Sprite_T *sprite, short type, GLuint timer, long flags)
{
	ent->sprite = sprite;
	ent->nextFrame = -1;
	ent->currentFrame = 0;
	ent->display.startTime = game->currentTime;
	ent->display.endTime = game->currentTime + timer;
	ent->display.type = type;
	ent->display.flags = flags;
}