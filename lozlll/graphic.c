#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include "include.h"

void DrawGraphic(Entity_T *graphic)
{
	GLfloat verts[8];
	GLfloat UVs[8];
	GLuint texloc;
	GLuint transp;
	GLfloat completion;

	if(graphic->type != ENTYPE_GRAPHIC){
		//Generate an error
		return;
	}
	transp = glGetUniformLocation(game->shader->id, "transparency");
	//Check to make sure this isn't an indefinite animation for fades
	if(graphic->display.endTime != 0)
		completion = (float)(game->currentTime - graphic->display.startTime)
			/ (graphic->display.endTime - graphic->display.startTime);
	else
		completion = (game->currentTime - graphic->display.startTime) / 3000.0f;

	if(IS_SET(graphic->display.flags, GRAPHFLAG_FULLSCREEN)){
		verts[0] = -1.0f;
		verts[1] = -1.0f;
		
		verts[2] = -1.0f;
		verts[3] = 1.0f;
		
		verts[4] = 1.0f;
		verts[5] = 1.0f;
		
		verts[6] = 1.0f;
		verts[7] = -1.0f;
		//Now the texture coordinates
		UVs[0] = 0.0f;
		UVs[1] = 1.0f;
		UVs[2] = 0.0f;
		UVs[3] = 0.0f;
		UVs[4] = 1.0f;
		UVs[5] = 0.0f;
		UVs[6] = 1.0f;
		UVs[7] = 1.0f;
	}else{
		verts[0] = graphic->pos.x;
		verts[1] = graphic->pos.y;

		verts[2] = verts[0];
		verts[3] = graphic->pos.y + graphic->size.y;

		verts[4] = graphic->pos.x + graphic->size.x;
		verts[5] = verts[3];
		
		verts[6] = verts[4];
		verts[7] = verts[1];
		
		UVs[0] = 0.0f;
		UVs[1] = 1.0f;
		UVs[2] = 0.0f;
		UVs[3] = 0.0f;
		UVs[4] = 1.0f;
		UVs[5] = 0.0f;
		UVs[6] = 1.0f;
		UVs[7] = 1.0f;
	}

	if(IS_SET(graphic->display.flags, GRAPHFLAG_FADE_IN) && completion < .33){
		  glProgramUniform1f(game->shader->id, transp, 1 - 3*completion); 
	}
	if(IS_SET(graphic->display.flags, GRAPHFLAG_FADE_OUT) 
	&& graphic->display.endTime != 0
	&& completion > .66){
		  glProgramUniform1f(game->shader->id, transp, -2 + 3*completion); 
	}
	//Create VBO
	glBindBuffer( GL_ARRAY_BUFFER, game->vertexBuffer );
	//Buffer the VBO with the new vertices
	glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), verts, GL_STATIC_DRAW );
	//Set the uniform up for the texture
	texloc = glGetUniformLocation(game->shader->id, "tex");
	glProgramUniform1i(game->shader->id, texloc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphic->sprite->texId);
	//Enable vertex position
	glEnableVertexAttribArray( game->shader->vertex_attrib );
	//Set vertex data
	glVertexAttribPointer( game->shader->vertex_attrib, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	//Enable texture UVs
	glEnableVertexAttribArray( game->shader->uv_attrib );
	glBindBuffer( GL_ARRAY_BUFFER, game->textureBuffer );
	glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), UVs, GL_STATIC_DRAW );
	glVertexAttribPointer( game->shader->uv_attrib, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glDrawArrays( GL_TRIANGLE_FAN, 0, 4);
	//Disable vertex position
	glDisableVertexAttribArray( game->shader->vertex_attrib );
	glDisableVertexAttribArray( game->shader->uv_attrib );
}

void SetupGraphic(Entity_T *ent, Sprite_T *sprite, short type, GLuint timer, long flags)
{
	ent->sprite = sprite;
	ent->type = ENTYPE_GRAPHIC;
	ent->nextFrame = 0;
	ent->currentFrame = 0;
	ent->display.startTime = game->currentTime;
	if(timer == 0)
		ent->display.endTime = timer;
	else
		ent->display.endTime = game->currentTime + timer;
	ent->display.type = type;
	ent->display.flags = flags;
}