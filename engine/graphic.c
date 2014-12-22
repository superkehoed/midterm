/******************************************************************************/

/******************************************************************************/
#include "include.h"
/******************************************************************************/
/**
 * Draws a graphic on the screen
 * @param graphic The Entity_T of the graphic to be drawn
 */
void DrawGraphic(Entity_T *graphic)
{
	GLfloat verts[12];
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
		verts[3] = -1.0f;
		verts[4] = 1.0f;
		verts[5] = -1.0f;
		verts[6] = 1.0f;
		verts[7] = -1.0f;
		verts[8] = -1.0f;
		verts[9] = 1.0f;
		verts[10] = 1.0f;
		verts[11] = -1.0f;
		//Now the texture coordinates
		UVs[0] = 0.0f;
		UVs[1] = 0.0f;
		UVs[2] = 0.0f;
		UVs[3] = 1.0f;
		UVs[4] = 1.0f;
		UVs[5] = 0.0f;
		UVs[6] = 1.0f;
		UVs[7] = 1.0f;
	}else{
		verts[0] = graphic->pos.x;
		verts[1] = graphic->pos.y;
		verts[2] = -1.0f;
		verts[3] = verts[0];
		verts[4] = graphic->pos.y + graphic->size.y;
		verts[5] = -1.0f;
		verts[6] = graphic->pos.x + graphic->size.x;
		verts[7] = verts[1];
		verts[8] = -1.0f;
		verts[9] = verts[4];
		verts[10]= verts[3];
		verts[11]= -1.0f;
		UVs[0] = 0.0f;
		UVs[1] = 0.0f;
		UVs[2] = 0.0f;
		UVs[3] = 1.0f;
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
	//Bind VBO
	glBindBuffer( GL_ARRAY_BUFFER, game->shader->vertexBuffer );
	//Buffer the VBO with the new vertices
	glBufferData( GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), verts, GL_STATIC_DRAW );
	//Set the uniform up for the texture
	texloc = glGetUniformLocation(game->shader->id, "tex");
	glProgramUniform1i(game->shader->id, texloc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphic->sprite->texId);
	//Enable vertex position
	glEnableVertexAttribArray( game->shader->vertex_attrib );
	//Set vertex data
	glVertexAttribPointer( game->shader->vertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	//Enable texture UVs
	glEnableVertexAttribArray( game->shader->uv_attrib );
	glBindBuffer( GL_ARRAY_BUFFER, game->shader->textureBuffer );
	glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), UVs, GL_STATIC_DRAW );
	glVertexAttribPointer( game->shader->uv_attrib, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4);
	//Disable vertex position
	glDisableVertexAttribArray( game->shader->vertex_attrib );
	glDisableVertexAttribArray( game->shader->uv_attrib );
}
/******************************************************************************/
/**
 * Sets up a graphic for use
 * @param ent The Entity_T representing the graphic
 * @param sprite The Sprite_T representing the graphic
 * @param short The type of graphic
 * @param timer How long the graphic should last for
 * @param flags Any graphic flags that should be set on the graphic
 */
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
/******************************************************************************/
/**
 * Draws the selector and the default weapon
 */
void DrawSelector()
{
	Vec2f pos;
	GLfloat verts[12];
	GLfloat UVs[8];
	GLuint texloc;
	return;
	if(game->heart == NULL)
		return;
	verts[0] = pos.x;
	verts[1] = pos.y;
	verts[2] = 0.0f;	
	verts[3] = pos.x;
	verts[4] = pos.y + TILE_HEIGHT;
	verts[5] = 0.0f;
	verts[6] = pos.x + TILE_WIDTH;
	verts[7] = pos.y;
	verts[8] = 0.0f;
	verts[9] = pos.x + TILE_WIDTH;
	verts[10] = pos.y + TILE_HEIGHT;
	verts[11] = 0.0f;
	//Now the texture coordinates
	UVs[0] = 0.0f;
	UVs[1] = 0.0f;
	UVs[2] = 0.0f;
	UVs[3] = 1.0f;
	UVs[4] = 1.0f;
	UVs[5] = 0.0f;
	UVs[6] = 1.0f;
	UVs[7] = 1.0f;
	//Bind VBO
	glBindBuffer( GL_ARRAY_BUFFER, game->shader->vertexBuffer );
	//Buffer the VBO with the new vertices
	glBufferData( GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), verts, GL_STATIC_DRAW );
	//Set the uniform up for the texture
	texloc = glGetUniformLocation(game->shader->id, "tex");
	glProgramUniform1i(game->shader->id, texloc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, game->heart->sprite->texId);
	//Enable vertex position
	glEnableVertexAttribArray( game->shader->vertex_attrib );
	//Set vertex data
	glVertexAttribPointer( game->shader->vertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	//Enable texture UVs
	glEnableVertexAttribArray( game->shader->uv_attrib );
	glBindBuffer( GL_ARRAY_BUFFER, game->shader->textureBuffer );
	glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), UVs, GL_STATIC_DRAW );
	glVertexAttribPointer( game->shader->uv_attrib, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4);
	//Disable vertex position
	glDisableVertexAttribArray( game->shader->vertex_attrib );
	glDisableVertexAttribArray( game->shader->uv_attrib );
}
/******************************************************************************/
/**
 * Draws hearts using the heart entity created by setupstate
 * @param pos The position of the heart
 * @param piece The piece of the heart to draw or -1 for all of it
 */
void DrawHeart(Vec2f pos, int piece)
{
	GLfloat verts[12];
	GLfloat UVs[8];
	GLuint texloc;
	GLint shadow;
	if(game->heart == NULL)
		return;
	switch(piece){
	default:
		verts[0] = pos.x;
		verts[1] = pos.y;
		verts[2] = 0.0f;
		verts[3] = pos.x;
		verts[4] = pos.y + TILE_HEIGHT;
		verts[5] = 0.0f;
		verts[6] = pos.x + TILE_WIDTH;
		verts[7] = pos.y;
		verts[8] = 0.0f;
		verts[9] = pos.x + TILE_WIDTH;
		verts[10] = pos.y + TILE_HEIGHT;
		verts[11] = 0.0f;
		//Now the texture coordinates
		UVs[0] = 0.0f;
		UVs[1] = 0.0f;
		UVs[2] = 0.0f;
		UVs[3] = 1.0f;
		UVs[4] = 1.0f;
		UVs[5] = 0.0f;
		UVs[6] = 1.0f;
		UVs[7] = 1.0f;
		break;
	//Upper left
	case 0:
		verts[0] = pos.x;
		verts[1] = pos.y + TILE_HEIGHT / 2;
		verts[2] = 0.0f;
		verts[3] = pos.x;
		verts[4] = pos.y + TILE_HEIGHT;
		verts[5] = 0.0f;
		verts[6] = pos.x + TILE_WIDTH / 2;
		verts[7] = pos.y + TILE_HEIGHT / 2;
		verts[8] = 0.0f;
		verts[9] = pos.x + TILE_WIDTH / 2;
		verts[10] = pos.y + TILE_HEIGHT;
		verts[11] = 0.0f;
		//Now the texture coordinates
		UVs[0] = 0.0f;
		UVs[1] = 0.5f;

		UVs[2] = 0.0f;
		UVs[3] = 1.0f;
		
		UVs[4] = 0.5f;
		UVs[5] = 0.5f;
		
		UVs[6] = 0.5f;
		UVs[7] = 1.0f;
		break;
	//Lower left
	case 1:
		verts[0] = pos.x;
		verts[1] = pos.y;
		verts[2] = 0.0f;
		verts[3] = pos.x;
		verts[4] = pos.y + TILE_HEIGHT / 2;
		verts[5] = 0.0f;
		verts[6] = pos.x + TILE_WIDTH / 2;
		verts[7] = pos.y;
		verts[8] = 0.0f;
		verts[9] = pos.x + TILE_WIDTH / 2;
		verts[10] = pos.y + TILE_HEIGHT / 2;
		verts[11] = 0.0f;
		//Now the texture coordinates
		UVs[0] = 0.0f;
		UVs[1] = 0.0f;
		UVs[2] = 0.0f;
		UVs[3] = 0.5f;
		UVs[4] = 0.5f;
		UVs[5] = 0.0f;
		UVs[6] = 0.5f;
		UVs[7] = 0.5f;
		break;
	//Lower right
	case 2:
		verts[0] = pos.x + TILE_WIDTH / 2;
		verts[1] = pos.y;
		verts[2] = 0.0f;
		verts[3] = pos.x + TILE_WIDTH / 2;
		verts[4] = pos.y + TILE_HEIGHT / 2;
		verts[5] = 0.0f;
		verts[6] = pos.x + TILE_WIDTH;
		verts[7] = pos.y;
		verts[8] = 0.0f;
		verts[9] = pos.x + TILE_WIDTH;
		verts[10]= pos.y + TILE_HEIGHT / 2;
		verts[11]= 0.0f;
		//Now the texture coordinates
		UVs[0] = 0.5f;
		UVs[1] = 0.0f;
		UVs[2] = 0.5f;
		UVs[3] = 0.5f;
		UVs[4] = 1.0f;
		UVs[5] = 0.0f;
		UVs[6] = 1.0f;
		UVs[7] = 0.5f;
		break;
	//Upper right
	case 3:
		verts[0] = pos.x + TILE_WIDTH / 2;
		verts[1] = pos.y + TILE_HEIGHT / 2;
		verts[2] = 0.0f;
		verts[3] = pos.x + TILE_WIDTH / 2;
		verts[4] = pos.y + TILE_HEIGHT;
		verts[5] = 0.0f;
		verts[6] = pos.x + TILE_WIDTH;
		verts[7] = pos.y + TILE_HEIGHT / 2;
		verts[8] = 0.0f;
		verts[9] = pos.x + TILE_WIDTH;
		verts[10] = pos.y + TILE_HEIGHT;
		verts[11] = 0.0f;
		//Now the texture coordinates
		UVs[0] = 0.5f;
		UVs[1] = 0.5f;
		UVs[2] = 0.5f;
		UVs[3] = 1.0f;
		UVs[4] = 1.0f;
		UVs[5] = 0.5f;
		UVs[6] = 1.0f;
		UVs[7] = 1.0f;
	}
	//Bind VBO
	glBindBuffer( GL_ARRAY_BUFFER, game->shader->vertexBuffer );
	//Buffer the VBO with the new vertices
	glBufferData( GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), verts, GL_STATIC_DRAW );
	//Set the uniforms up for the heart
	shadow = glGetUniformLocation(game->shader->id, "shadow");
	texloc = glGetUniformLocation(game->shader->id, "tex");
	glProgramUniform1f(game->shader->id, shadow, -0.5f);
	glProgramUniform1i(game->shader->id, texloc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, game->heart->sprite->texId);
	//Enable vertex position
	glEnableVertexAttribArray( game->shader->vertex_attrib );
	//Set vertex data
	glVertexAttribPointer( game->shader->vertex_attrib,3, GL_FLOAT, GL_FALSE, 0, NULL );
	//Enable texture UVs
	glEnableVertexAttribArray( game->shader->uv_attrib );
	glBindBuffer( GL_ARRAY_BUFFER, game->shader->textureBuffer );
	glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), UVs, GL_STATIC_DRAW );
	glVertexAttribPointer( game->shader->uv_attrib, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4);
	//Disable vertex position
	glDisableVertexAttribArray( game->shader->vertex_attrib );
	glDisableVertexAttribArray( game->shader->uv_attrib );
}
/******************************************************************************/
/**
  * Draws the HUD for the player during gamestate playing
  */
void DrawHUD()
{
	int i;
	int health = 23;
	Vec2f pos = {/*Heart Start position*/-1, 1-TILE_HEIGHT};
	//Draw the hearts in the corner
	for(i = 0;i < health;i+=4){
		//Draw a heart
		DrawHeart(pos, -1);
		pos.x += TILE_WIDTH;
	}
	for(i = 0;i < health % 4;i++)
		DrawHeart(pos, i);
	//Draw the selected weapon

	DrawSelector();
	//Draw the outline
	//Draw the weapon
	//Draw the energy bar as necessary

	//TODO: Draw the minimap
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/