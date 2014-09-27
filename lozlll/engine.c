/******************************************************************************/
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include <stdio.h>
#include "defines.h"
#include "sprite_data.h"
#include "engine.h"
/******************************************************************************/
//Globals
/******************************************************************************/
long gCurrentTime;
/******************************************************************************/
/** Initializes the game engine and sets everything up for the game
*/
void Initialize(Engine_T *e)
{
	bool success = true;
	//Basic refresh
	e->entityCount = 0;
	e->spriteCount = 0;
	gCurrentTime = SDL_GetTicks();
	e->lastUpdate = 0;
	//Reserve the memory necessary for entities and set up the unused sprites list
	SpriteData *s = (SpriteData *)malloc(sizeof(SpriteData)*STARTING_NUM_SPRITES);
	for(int i = 0;i < STARTING_NUM_SPRITES;i++){
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
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

		//Create window
		e->window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
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
				GLenum glewError = glewInit();
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
				if( !initGL() )
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
void Poll(EngineData *e)
{
	//Poll for any inputs by the player at this time
}
/******************************************************************************/
void Update(EngineData *e)
{
	gCurrentTime = SDL_GetTicks();
	//Go through the current map and update all entities
	mLastUpdate = gCurrentTime;
}
/******************************************************************************/
void Draw(EngineData *e)
{
	//Draw all of the entities for the game

}
/******************************************************************************/