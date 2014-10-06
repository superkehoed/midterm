/******************************************************************************/
/**
 * @file engine.c
 * @author Ulysee Thompson
 */
/******************************************************************************/
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <GL/GLU.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "sprite.h"
#include "graphic.h"
#include "entity.h"
#include "shader.h"
#include "engine.h"
/******************************************************************************/
//Globals
/** Global pointer for tracking the current game engine */
Engine_T *game;
/******************************************************************************/
/** Initializes the game engine and sets everything up for the game
 * @param e The engine to be started
 */
bool Startup()
{
	bool success = true;
	Sprite_T *s;
	Entity_T *ent;
	int i;
	GLenum glewError;
	//Global initialization
	gUnusedSpriteList = NULL;
	gUnusedEntityList = NULL;
	//Engine initialization
	game->state = GAMESTATE_INTRO;
	game->entityCount = 0;
	game->spriteCount = 0;
	game->lastUpdate = 0;
	game->hero = NULL;
	game->foreGraphicsHead = NULL;
	game->foreGraphicsTail = NULL;
	game->backGraphicsHead = NULL;
	game->backGraphicsTail = NULL;
	game->done = false;
	//Reserve the memory necessary for entities and set up the unused entity list
	ent = (Entity_T *)malloc(sizeof(Entity_T)*STARTING_NUM_ENTITIES);
	for(i = 0;i < STARTING_NUM_ENTITIES;i++){
		FreeEntity(ent);
		ent++;
	}
	//Reserve the memory necessary for sprites and set up the unused sprites list
	s = (Sprite_T *)malloc(sizeof(Sprite_T)*STARTING_NUM_SPRITES);
	for(i = 0;i < STARTING_NUM_SPRITES;i++){
		FreeSprite(s);
		s++;
	}
	//Initialize SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else if(!IS_SET(IMG_Init(IMG_INIT_PNG), IMG_INIT_PNG)){
		printf("ERROR - Startup(): Could not initialize SDL_image: %s\n", IMG_GetError());
		success = false;
	}else{
		game->currentTime = SDL_GetTicks();

		//Create window
		game->window = SDL_CreateWindow( GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
									SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if( game->window == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create context
			game->context = SDL_GL_CreateContext( game->window );
			if( game->context == NULL )
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
				if( !InitializeGL() )
				{
					printf( "Unable to initialize OpenGL!\n" );
					success = false;
				}

			}
		}
	}
	//TODO: Enable mouse hiding in final run
	//SDL_SetRelativeMouseMode(SDL_TRUE);
	return success;
}
/******************************************************************************/
/** The polling method for the engine
 * @param e The engine to receiving polling events
 */
void Poll()
{
	//Poll for any inputs by the player at this time
	switch(game->state){
	case GAMESTATE_INTRO: break;
	default: printf("Poll(): No state set!\n"); break;
	}
}
/******************************************************************************/
/** The updating method for the engine
 * @param e The engine to run the update method
 */
void Update()
{
	Entity_T *e;
	Sprite_T *s;
	Rect r;
	Animation_T a;
	game->currentTime = SDL_GetTicks();
	switch(game->state){
	case GAMESTATE_INTRO:
		if(game->foreGraphicsHead == NULL){
			e = NewEntity();
			s = NewSprite();
			r.x = 0;
			r.y = 0;
			r.w = SCREEN_WIDTH;
			r.h = SCREEN_HEIGHT;
			memset(&a, 0, sizeof(a));
			SetupSprite(s, "splash.bmp", &r, 1, &a, 1); 
			SetupGraphic(e, s, GRAPHICTYPE_SPLASH, 300, GRAPHFLAG_FADE|GRAPHFLAG_FULLSCREEN);
			AddSplashScreen(e);
		}
		break;
	default: printf("Update(): No state set!\n"); break;
	}
	//Go through the current map and update all entities
	game->lastUpdate = game->currentTime;
}
/******************************************************************************/
/** The drawing method for the engine
 * @param e The engine to run the draw method
 */
void Draw()
{
	Entity_T *e;
	if(game->shader == NULL){
		printf("Draw(): No shader has been assigned to the game engine!");
		return;
	}

	switch(game->state){
	case GAMESTATE_INTRO:
		//Draw the current splash screen
		//Clear color buffer
		glClear( GL_COLOR_BUFFER_BIT );
		//Bind program
		glUseProgram( game->shader->id );
		for(e = game->foreGraphicsHead;e != NULL;e = e->next)
			DrawGraphic(e);
		//Unbind program
		glUseProgram( (GLuint)NULL );
		SDL_GL_SwapWindow( game->window );
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
bool InitializeGL()
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
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); 
	shader = NewShader();
	game->shader = shader;
	//Initialize the Shader
	InitializeShader(shader, "default.vp", "default.fp");
	//Initialize clear color
	glClearColor( 1.f, 0.f, 0.f, 1.f );

	//Create VBO
	glGenBuffers( 1, &shader->VBO );
	glBindBuffer( GL_ARRAY_BUFFER, shader->VBO );
	glBufferData( GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW );

	//Create IBO
	glGenBuffers( 1, &shader->IBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, shader->IBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );

	
	glGenBuffers(1, &game->vertexBuffer);
	glGenBuffers(1, &game->textureBuffer);
	glDisable(GL_DEPTH_TEST);
	return success;
}
/******************************************************************************/
/**The main loop for the engine
 * @param e The engine which will run the main loop
 */
void MainLoop()
{
	Uint32 ticks;
	Poll();
	Update();
	Draw();
	//Delay for at least one full frame-length if it wasn't already
	if((ticks = game->currentTime - game->lastUpdate) < MILLISECONDS_PER_FRAME){
		//printf("Cycle Timer: %d\n", ticks);
		SDL_Delay(URANGE(1, MILLISECONDS_PER_FRAME -ticks, MILLISECONDS_PER_FRAME));
	}
	//else printf("Cycle Timer: %d\n", game->currentTime - game->lastUpdate);
	return;
}
/******************************************************************************/
/**Shuts down and cleans up the engine and SDL/OpenGL code
 * @param e The engine which needs to be shut down
 */
void Shutdown()
{
	//TODO: Add a check to make sure startup has been run on the game engine
	//Deallocate program
	DestroyShader(game->shader);
	//Destroy window	
	SDL_DestroyWindow( game->window );
	game->window = NULL;
	//Quit SDL subsystems
	SDL_Quit();
}
/******************************************************************************/
/**
 * Borrowed graciously from the Game Programming Wiki and improvised.
 * Loads a png file by name as an SDL surface, converts it to an OpenGL
 * Texture and then returns the texture ID.
 */
GLuint LoadTex(const char *name)
{
	GLuint texture;			// This is a handle to our texture object
	SDL_Surface *surface;	// This surface will tell us the details of the image
	GLenum texture_format;
	GLint  nOfColors;
 
	if ( (surface = SDL_LoadBMP(name)) ) { 
 
		// Check that the image's width is a power of 2
		if ( (surface->w & (surface->w - 1)) != 0 ) {
			printf("warning: %s has a width that is not a power of 2\n", name);
		}
 
		// Also check if the height is a power of 2
		if ( (surface->h & (surface->h - 1)) != 0 ) {
			printf("warning: %s has a height that is not a power of 2\n", name);
		}
 
			// get the number of channels in the SDL surface
			nOfColors = surface->format->BytesPerPixel;
			if (nOfColors == 4)     // contains an alpha channel
			{
					if (surface->format->Rmask == 0x000000ff)
							texture_format = GL_RGBA;
					else
							texture_format = GL_BGRA;
			} else if (nOfColors == 3)     // no alpha channel
			{
					if (surface->format->Rmask == 0x000000ff)
							texture_format = GL_RGB;
					else
							texture_format = GL_BGR;
			} else {
					printf("warning: the image is not truecolor..  this will probably break\n");
					// this error should not go unhandled
			}
 
		// Have OpenGL generate a texture object handle for us
		glGenTextures( 1, &texture );
 
		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, texture );
 
		// Set the texture's stretching properties
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
 
		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
						  texture_format, GL_UNSIGNED_BYTE, surface->pixels );
	} 
	else {
		printf("LoadTex(): SDL could not load %s: %s\n", name, SDL_GetError());
		return -1;
	}    
 
	// Free the SDL_Surface only if it was successfully created
	if ( surface ) { 
		SDL_FreeSurface( surface );
	}
	return texture;
}
/******************************************************************************/
void AddSplashScreen(Entity_T *e)
{
	//Add to the back of the linked list
	if(game->foreGraphicsHead == NULL){
		game->foreGraphicsHead = e;
		game->foreGraphicsTail = e;
	}else{
		if(game->foreGraphicsTail == NULL){
			//Generate an error, tail is missing
		}else{
			game->foreGraphicsTail = e;
		}
	}
}
/******************************************************************************/
void RemoveSplashScreen(Entity_T *e)
{
	bool success = false;
	//Remove from the linked list
	Entity_T *ent;
	if(game->foreGraphicsHead == e){
		if(game->foreGraphicsTail == e)
			game->foreGraphicsTail = NULL;
		FreeEntity(game->foreGraphicsHead);
		game->foreGraphicsHead = 0;
	}else{
		for(ent = game->foreGraphicsHead;ent != NULL;ent = ent->next)
			if(ent->next == e)
				break;
		if(ent != NULL){
			ent->next = e->next;
			FreeEntity(e);
		}
	}
}
/******************************************************************************/
/******************************************************************************/