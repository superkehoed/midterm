/******************************************************************************/
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "sprite.h"
#include "shader.h"
#include "engine.h"
/******************************************************************************/
//Globals
/******************************************************************************/
/** Global variable for tracking the current time as of the last engine update*/
long gCurrentTime;
/******************************************************************************/
/** Initializes the game engine and sets everything up for the game
 * @param e The engine to be started
 */
bool Startup(Engine_T *e)
{
	bool success = true;
	Sprite_T *s;
	int i;
	GLenum glewError;
	//Basic refresh
	gCurrentTime = SDL_GetTicks();
	e->state = GAMESTATE_INTRO;
	e->entityCount = 0;
	e->spriteCount = 0;
	e->lastUpdate = 0;
	e->done = false;
	//Reserve the memory necessary for entities and set up the unused sprites list
	s = (Sprite_T *)malloc(sizeof(Sprite_T)*STARTING_NUM_SPRITES);
	for(i = 0;i < STARTING_NUM_SPRITES;i++){
		FreeSprite(s);
		s++;
	}
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Use OpenGL 3.1 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

		//Create window
		e->window = SDL_CreateWindow( GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
									SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if( e->window == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create context
			e->context = SDL_GL_CreateContext( e->window );
			if( e->context == NULL )
			{
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize GLEW
				glewExperimental = GL_TRUE; 
				glewError = glewInit();
				if( glewError != GLEW_OK )
				{
					printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
				}

				//Use Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 )
				{
					printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
				}

				//Initialize OpenGL
				if( !InitializeGL(e) )
				{
					printf( "Unable to initialize OpenGL!\n" );
					success = false;
				}
			}
		}
	}
	return success;
}
/******************************************************************************/
/** The polling method for the engine
 * @param e The engine to receiving polling events
 */
void Poll(Engine_T *e)
{
	//Poll for any inputs by the player at this time
	switch(e->state){
	default: printf("Poll(): No state set!\n"); break;
	}
}
/******************************************************************************/
/** The updating method for the engine
 * @param e The engine to run the update method
 */
void Update(Engine_T *e)
{
	gCurrentTime = SDL_GetTicks();
	switch(e->state){
	default: printf("Update(): No state set!\n"); break;
	}
	//Go through the current map and update all entities
	e->lastUpdate = gCurrentTime;
}
/******************************************************************************/
/** The drawing method for the engine
 * @param e The engine to run the draw method
 */
void Draw(Engine_T *e)
{
	if(e->shader == NULL){
		printf("Draw(): No shader has been assigned to the game engine!");
		return;
	}

	switch(e->state){
	case GAMESTATE_INTRO:
		//Draw the current splash screen
		//Clear color buffer
		glClear( GL_COLOR_BUFFER_BIT );
		//Bind program
		glUseProgram( e->shader->id );
		//Enable vertex position
		glEnableVertexAttribArray( e->shader->vertex_attrib );
		//Set vertex data
		glBindBuffer( GL_ARRAY_BUFFER, e->shader->VBO );
		glVertexAttribPointer( e->shader->vertex_attrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL );
		//Set index data and render
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, e->shader->IBO );
		glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );
		//Disable vertex position
		glDisableVertexAttribArray( e->shader->vertex_attrib );
		//Unbind program
		glUseProgram( (GLuint)NULL );
		SDL_GL_SwapWindow( e->window );
		break;
	case GAMESTATE_MAIN_MENU:
		//Draw the background
		//Draw the title
		//Draw the menu list
		//Draw the current selector
		break;
	case GAMESTATE_PLAYING:
		//Draw the current map
		//Draw particle effects
		//Draw the HUD
		break;
	case GAMESTATE_GAME_MENU:
		//Draw the background
		//Draw the item list
		//List stats
		break;
	case GAMESTATE_CREDITS:
		//Draw the background
		//Draw the credits
		break;
	default: 
		printf("Draw(): No state set!\n"); 
		break;
	}
	//Draw all of the entities for the game
}
/******************************************************************************/
/**Initializes GL and sets up a shader for the engine
 * @param e The engine which will have its shader set up
 */
bool InitializeGL(Engine_T *e)
{
	//Success flag
	bool success = true;
	Shader_T *shader;
	GLuint ProgramID = 0;

	//VBO data
	GLfloat vertexData[] =
	{
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f,  0.5f,
		-0.5f,  0.5f
	};
	//IBO data
	GLuint indexData[] = { 0, 1, 2, 3 };
	shader = NewShader();
	e->shader = shader;
	//Initialize the Shader
	InitializeShader(shader, "shader.vs", "shader.fs");
	//Initialize clear color
	glClearColor( 0.f, 0.f, 0.f, 1.f );

	//Create VBO
	glGenBuffers( 1, &shader->VBO );
	glBindBuffer( GL_ARRAY_BUFFER, shader->VBO );
	glBufferData( GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW );

	//Create IBO
	glGenBuffers( 1, &shader->IBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, shader->IBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );

	
	return success;
}
/******************************************************************************/
/**The main loop for the engine
 * @param e The engine which will run the main loop
 */
void MainLoop(Engine_T *e)
{
	Uint32 ticks;
	Poll(e);
	Update(e);
	Draw(e);
	//Delay for at least one full frame-length if it wasn't already
	if((ticks = gCurrentTime - e->lastUpdate) < MILLISECONDS_PER_FRAME)
		SDL_Delay(URANGE(1, MILLISECONDS_PER_FRAME -ticks, MILLISECONDS_PER_FRAME));
	return;
}
/******************************************************************************/
/**Shuts down and cleans up the engine and SDL/OpenGL code
 * @param e The engine which needs to be shut down
 */
void Shutdown(Engine_T *e)
{
	//TODO: Add a check to make sure startup has been run on the game engine
	//Deallocate program
	DestroyShader(e->shader);
	//Destroy window	
	SDL_DestroyWindow( e->window );
	e->window = NULL;
	//Quit SDL subsystems
	SDL_Quit();
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/