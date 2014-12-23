/******************************************************************************/
/**
 * @file engine.c
 * @author Ulysee Thompson
 */
/******************************************************************************/
#include "include.h"
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

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
	game->flags = GAMEFLAG_SETUP_STATE;
	game->entityCount = 0;
	game->spriteCount = 0;
	game->lastUpdate = 0;
	game->hero = NULL;
	game->foreGraphicsHead = NULL;
	game->foreGraphicsTail = NULL;
	game->backGraphicsHead = NULL;
	game->backGraphicsTail = NULL;
	game->heart = NULL;
	game->map = NULL;
	game->selection = 0;
	game->done = false;
	game->mapBatch.num_vertices = 0;
	game->mapBatch.num_indices = 0;
	game->entBatch.num_vertices = 0;
	game->entBatch.num_indices = 0;
	game->darkness = TOTAL_DARKNESS;
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
									SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
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
	SDL_SetRelativeMouseMode(SDL_TRUE);
	return success;
}
/******************************************************************************/
/** The polling method for the engine
 * @param e The engine to receiving polling events
 */
void Poll()
{
	short inputs[KEY_MAX];
	SDL_Event e;
	Vec2f pos;
	unsigned int key;
	//Poll for any inputs by the player at this time
	switch(game->state){
	case GAMESTATE_INTRO: 
		//Nope, make them sit through it all.
		break;
	case GAMESTATE_MAIN_MENU: 
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_KEYDOWN){
				switch(e.key.keysym.sym){
				case SDLK_UP: 
				case SDLK_DOWN: 
					game->selection = (game->selection + 1) % 2;
					//TODO: Hackish, find a cleaner way of doing this.
					game->foreGraphicsTail->pos.y = -.3f - .2f * game->selection;
					break;
				case SDLK_SPACE: 
					SET_FLAG(game->flags, GAMEFLAG_SELECTING);
					break;
				case SDLK_ESCAPE:
					game->done = true;
					break;
				}
			}else if(e.type == SDL_QUIT){
				game->done = true;
			}
		}
		break;
	case GAMESTATE_PLAYING:
		if(game->hero != NULL)
		{
			pos.x = game->hero->pos.x;
			pos.y = game->hero->pos.y;
		}
		//Poll for events over and over
		SDL_PumpEvents();
		game->inputs = SDL_GetKeyboardState(NULL);
		if(game->inputs[SDL_SCANCODE_ESCAPE]){
			game->done = true;
		}
		if(game->inputs[SDL_SCANCODE_Q])
			game->darkness+=.01f;
		if(game->inputs[SDL_SCANCODE_W])
			game->darkness-=.01f;
		if(game->inputs[SDL_SCANCODE_V]){
			game->hero->light->offset.x+=.001f;
			printf("Adjusting light to: %f,%f\n",
				game->hero->light->offset.x, 
				game->hero->light->offset.y);
		}
		if(game->inputs[SDL_SCANCODE_C]){
			game->hero->light->offset.x-=.001f;
			printf("Adjusting light to: %f,%f\n",
				game->hero->light->offset.x, 
				game->hero->light->offset.y);
		}


		break;
	case GAMESTATE_CREDITS:
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_KEYDOWN){
				if(e.key.keysym.sym == SDLK_ESCAPE){
					SET_FLAG(game->flags, GAMEFLAG_SELECTING);
				}
			}if(e.type == SDL_QUIT){
				game->done = true;
			}
		}
		break;
	default: printf("Poll(): No state set!\n"); break;
	}
}
/******************************************************************************/
/** The updating method for the engine
 * @param e The engine to run the update method
 */
void Update()
{
	Entity_T *e, *e_next;
	Sprite_T *s;
	Rect r;
	Animation_T a;
	GLuint delta = game->currentTime - game->lastUpdate;
	r.x = 0;
	r.y = 0;
	r.w = SCREEN_WIDTH;
	r.h = SCREEN_HEIGHT;
	memset(&a, 0, sizeof(a));
	if(SetupState())
		return;
	switch(game->state){
	case GAMESTATE_INTRO:
		UpdateForeGraphics();
		if(game->foreGraphicsHead == NULL){
			game->state = GAMESTATE_MAIN_MENU;
			SET_FLAG(game->flags, GAMEFLAG_SETUP_STATE);
		}
		break;
	case GAMESTATE_MAIN_MENU:
		//Handle if they have selected something
		if(IS_SET(game->flags, GAMEFLAG_SELECTING)){
			REMOVE_FLAG(game->flags, GAMEFLAG_SELECTING);
			switch(game->selection){
			case 0: //New Game
				game->state = GAMESTATE_PLAYING;
				SET_FLAG(game->flags, GAMEFLAG_SETUP_STATE);
				break;
			case 1: //Credits
				game->state = GAMESTATE_CREDITS;
				SET_FLAG(game->flags, GAMEFLAG_SETUP_STATE);
				break;
			}
			DequeueForeGraphics(true);
			DequeueBackGraphics(true);
		}
		break;
	case GAMESTATE_PLAYING:
		//Interpret their input
		InterpretInput();
		//Run the physics tests
		UpdatePhysics(delta);
		//Go through the current map and update all entities
		for(e = game->map->entities; e; e = e_next){
			e_next = e->next;
			UpdateEntity(e, delta);
		}
		//Update any world events that are pending
		break;
	case GAMESTATE_GAME_MENU:
		//Handled in update
		break;
	case GAMESTATE_CREDITS:
		//Midterm - See list below
		//Fade in a credits page
		if(IS_SET(game->flags, GAMEFLAG_SETUP_STATE))
		{
			//TODO: Encapsulate this behavior later into a formal menu system
			e = NewEntity();
			s = NewSprite();
			SetupSprite(s, "credits.png", &r, 1, &a, 1);
			SetupGraphic(e, s, GRAPHICTYPE_SPLASH, 0, GRAPHFLAG_FADE_IN|GRAPHFLAG_FULLSCREEN);
			AddSplashScreen(e);
			REMOVE_FLAG(game->flags, GAMEFLAG_SETUP_STATE);
		}
		else if(IS_SET(game->flags, GAMEFLAG_SELECTING))
		{
			DequeueForeGraphics(true);
			REMOVE_FLAG(game->flags, GAMEFLAG_SELECTING);
			SET_FLAG(game->flags, GAMEFLAG_SETUP_STATE);
			game->state = GAMESTATE_MAIN_MENU;
		}
		break;
	default: printf("Update(): No state set!\n"); break;
	}
	
}
/******************************************************************************/
/** The drawing method for the engine
 * @param e The engine to run the draw method
 */
void Draw()
{
	Entity_T *e;
	Vec2f offset;
	bool swap = false;
	if(game->shader == NULL){
		printf("Draw(): No shader has been assigned to the game engine!");
		return;
	}

	switch(game->state){
	case GAMESTATE_INTRO:
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glUseProgram( game->shader->id );
		DrawGraphic(game->foreGraphicsHead);
		glUseProgram( (GLuint)NULL );
		swap = true;
		break;
	case GAMESTATE_MAIN_MENU:
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glUseProgram( game->shader->id );
		for(e = game->foreGraphicsHead;e != NULL;e = e->next)
			DrawGraphic(e);
		glUseProgram( (GLuint)NULL );
		swap = true;
		break;
	case GAMESTATE_PLAYING:
		//Draw the current map
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		if(game->map != NULL){
			ResetBatch(&game->mapBatch);
			offset.x = game->hero->pos.x;
			offset.y = game->hero->pos.y;
			SET_FLAG(game->mapBatch.flags, BATCHFLAG_LIGHTING);
			BatchScreenMap(&game->mapBatch, game->map, game->hero->pos);
			DrawBatch(&game->mapBatch, game->shader);
			ResetBatch(&game->entBatch);
			BatchScreenEntities(&game->entBatch, game->map, game->hero->pos);
			DrawBatch(&game->entBatch, game->shader);
		}
		glUseProgram(game->shader->id);
		DrawHUD();
		glUseProgram((GLuint)NULL);
		swap = true;
		//Draw particle/light effects
		//Draw the HUD
		break;
	case GAMESTATE_GAME_MENU:
		//TODO:
		//Draw the background
		//Draw the item list
		//List stats
		//Draw the selector positioned at point n
		break;
	case GAMESTATE_CREDITS:
		//Draw the credits
		glClear( GL_COLOR_BUFFER_BIT );
		glUseProgram( game->shader->id );
		for(e = game->foreGraphicsHead;e != NULL;e = e->next)
			DrawGraphic(e);
		glUseProgram( (GLuint)NULL );
		swap = true;
		break;
	default: 
		printf("Draw(): No state set!\n"); 
		break;
	}
	if(swap)
		SDL_GL_SwapWindow( game->window );
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
	glClearColor( 0.f, 0.f, 0.f, 1.f );

	//Create VBO
	glGenBuffers( 1, &shader->VBO );
	glBindBuffer( GL_ARRAY_BUFFER, shader->VBO );
	glBufferData( GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW );

	//Create IBO
	glGenBuffers( 1, &shader->IBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, shader->IBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );
	glGenBuffers(1, &shader->vertexBuffer);
	glGenBuffers(1, &shader->textureBuffer);
	glGenBuffers(1, &shader->indexBuffer);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(RESET_INDEX);
	return success;
}
/******************************************************************************/
/**The main loop for the engine
 * @param e The engine which will run the main loop
 */
void MainLoop()
{
	game->lastUpdate = game->currentTime;
	game->currentTime = SDL_GetTicks();
	Poll();
	Update();
	Draw();
	//Delay for at least one full frame-length if it wasn't already
	if( game->currentTime - game->lastUpdate < MILLISECONDS_PER_FRAME){
		//printf("Cycle Timer: %d\n", ticks);
		SDL_Delay(URANGE(1, MILLISECONDS_PER_FRAME - game->currentTime + game->lastUpdate, MILLISECONDS_PER_FRAME));
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
/**Borrowed from Gribble Games -- Thanks guys!
 * Inverts an image to avoid issues with SDL and OpenGL interoperability
 * @param pitch The Pitch of the image
 * @param height The height of the image
 * @param image_pixels A void pointer to an array holding the image pixels
 * @return Either 0 (success) or -1 (Error)
 */
int invert_image(int pitch, int height, void* image_pixels)
{
	int index;
	void* temp_row;
	int height_div_2;

	temp_row = (void *)malloc(pitch);
	if(NULL == temp_row)
	{
		SDL_SetError("Not enough memory for image inversion");
		return -1;
	}
	//if height is odd, don't need to swap middle row
	height_div_2 = (int) (height * .5);
	for(index = 0; index < height_div_2; index++) 	{
		//uses string.h
		memcpy((Uint8 *)temp_row,
			(Uint8 *)(image_pixels) +
			pitch * index,
			pitch);

		memcpy(
			(Uint8 *)(image_pixels) +
			pitch * index,
			(Uint8 *)(image_pixels) +
			pitch * (height - index-1),
			pitch);
		memcpy(
			(Uint8 *)(image_pixels) +
			pitch * (height - index-1),
			temp_row,
			pitch);
	}
	free(temp_row);
	return 0;
}
/******************************************************************************/
/** Used to invert an sdl surface for OpenGL use
  * @param image A pointer to the SDL_Surface to be inverted
  * @return Either 0 (Success) or -1 (Error)
  */
int SDL_InvertSurface(SDL_Surface* image)
{
	if(NULL == image)
	{
		SDL_SetError("Surface is NULL");
		return -1;
	}
	return( invert_image(image->pitch, image->h,
			image->pixels) );
}
/******************************************************************************/
/**
 * Borrowed graciously from the Game Programming Wiki and improvised.
 * Loads a png file by name as an SDL surface, converts it to an OpenGL
 * Texture and then returns the texture ID.
 * @param name The name of the file to be loaded
 * @return The GLuint associated with the loaded texture
 */
GLuint LoadTex(const char *name)
{
	GLuint texture;			// This is a handle to our texture object
	SDL_Surface *surface;	// This surface will tell us the details of the image
	GLenum texture_format;
	GLint  nOfColors;

	if ( (surface = IMG_Load(name)) ) { 
 
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
 
		//Invert the texture
		SDL_InvertSurface(surface);
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
/** Turns an SDL surface into an OpenGL Texture
 * @param surface The surface to be converted
 * @return A GLuint representing the new texture
 */
GLuint SurfaceToTexture(SDL_Surface *surface)
{
	int nOfColors;
	GLuint texture;
	GLenum texture_format;
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
 
	//Invert the texture
	SDL_InvertSurface(surface);
	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
						texture_format, GL_UNSIGNED_BYTE, surface->pixels );
	if(surface)
		SDL_FreeSurface(surface);
	return texture;
}
/******************************************************************************/
/**
  * Adds a splashscreen to the foregraphics list
  * @param e The entity representing the splashscreen to be added
  */
void AddSplashScreen(Entity_T *e)
{
	//Add to the back of the linked list
	if(game->foreGraphicsHead == NULL){
		game->foreGraphicsHead = e;
		game->foreGraphicsTail = e;
		e->next = NULL;
	}else{
		if(game->foreGraphicsTail == NULL){
			//Generate an error, tail is missing
		}else{
			//Fix the timer on the splash screen
			if(game->foreGraphicsTail->display.endTime > 0)
			{
				e->display.endTime = e->display.endTime - e->display.startTime + game->foreGraphicsTail->display.endTime;
				e->display.startTime = game->foreGraphicsTail->display.endTime;
			}
			game->foreGraphicsTail->next = e;
			game->foreGraphicsTail = e;
		}
	}
}
/******************************************************************************/
/**
  * Removes a splashscreen from the foregraphics list
  * @param e The entity representing the splashscreen to be removed
  */
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
/**
  * Updates the foregraphics, removing anything with a lapsed time
  */ 
void UpdateForeGraphics()
{
	Entity_T *e, *NextEntity;
	for(e = game->foreGraphicsHead;e != NULL;e = NextEntity){
		NextEntity = e->next;
		if(e->type == ENTYPE_GRAPHIC
		&& e->display.endTime != 0
		&& e->display.endTime < game->currentTime){
			game->foreGraphicsHead = e->next;
			FreeEntity(e);
		}
	}
}
/******************************************************************************/
/**
 * Updates the backgraphics, removing anything with a lapsed time
 */
void UpdateBackGraphics()
{
	Entity_T *e, *NextEntity;
	for(e = game->backGraphicsHead;e != NULL;e = NextEntity){
		NextEntity = e->next;
		if(e->type == ENTYPE_GRAPHIC
		&& e->display.endTime != 0
		&& e->display.endTime < game->currentTime){
			game->backGraphicsHead = e->next;
			if(game->backGraphicsTail == e)
				game->backGraphicsTail = NULL;
			FreeEntity(e);
		}
	}
}
/******************************************************************************/
/**
  * Dequeues and potentially frees all entities in the foregraphics list
  * @param free Boolean representing if dequeued entities should be freed
  */
void DequeueForeGraphics(bool free)
{
	Entity_T *e, *NextEntity;
	for(e = game->foreGraphicsHead;e != NULL;e = NextEntity){
		NextEntity = e->next;
		if(game->foreGraphicsTail == e)
			game->foreGraphicsTail = NULL;
		if(free)
			FreeEntity(e);
	}
	game->foreGraphicsHead = NULL;

}
/******************************************************************************/
/**
  * Dequeues and potentially frees all entities in the bacgraphics list
  * @param free Boolean representing if dequeued entities should be freed
  */
void DequeueBackGraphics(bool free)
{
	Entity_T *e, *NextEntity;
	for(e = game->backGraphicsHead;e != NULL;e = NextEntity){
		NextEntity = e->next;
		if(game->backGraphicsTail == e)
			game->backGraphicsTail = NULL;
		if(free)
			FreeEntity(e);
	}
	game->backGraphicsHead = NULL;
}
/******************************************************************************/
/**
  * Returns a random value between 0 and num-1
  * @param num The noninclusive maximum value of the range
  * @return A random value between 0 and num-1
  */
GLuint Random(GLuint num)
{
	return rand() % num;
}
/******************************************************************************/
/**
  * Generates a Vec2f structure from a Vec3f by eliminating the z value
  * @param vec The Vec3f structure to be converted
  * @return A Vec2f structure containing the x and y values of vec
  */
Vec2f Vec3fToVec2f(Vec3f vec)
{
	Vec2f nvec;
	nvec.x = vec.x;
	nvec.y = vec.y;
	return nvec;
}
/******************************************************************************/
/**
  * Interprets the input during GAMESTATE_PLAYING
  */
void InterpretPlayInput()
{
	Vec2f vel = {0, 0};
	if(game->inputs == NULL)
		return;
	//Deal with walking
	if(game->inputs[SDL_SCANCODE_UP])
		vel.y += 1;
	if(game->inputs[SDL_SCANCODE_DOWN])
		vel.y -= 1;
	if(game->inputs[SDL_SCANCODE_LEFT])
		vel.x -= 1;
	if(game->inputs[SDL_SCANCODE_RIGHT])
		vel.x += 1;
	if(vel.y)
		WalkEntity(game->hero, vel.y == 1 ? ENTDIR_UP : ENTDIR_DOWN);
	if(vel.x)
		WalkEntity(game->hero, vel.x == 1 ? ENTDIR_RIGHT : ENTDIR_LEFT);
	if(game->hero->state == ENTSTATE_WALK
	&& vel.x == 0 && vel.y == 0){
		DefaultState(game->hero);
		game->hero->body->v.x = 0;
		game->hero->body->v.y = 0;
	}
}
/******************************************************************************/
/**
  * Flow controller for interpreting input based upon the current game state
  */
void InterpretInput()
{
	switch(game->state){
	case GAMESTATE_PLAYING:
		InterpretPlayInput();
		break;
	default:
		if(game->inputs[SDLK_ESCAPE])
			game->done = true;
		break;
	}
}
/******************************************************************************/
/**
  * Converts SDLK values into the correct representative values based upon
  * Those defined in inputs and the key_table
  * @param SDLkey The SDLK value to be looked up
  * @return The key value found or -1 if no key value is found
  */
int KeyLookup(Sint32 SDLkey)
{
	int key = -1;
	int i;
	for(i = 0;i < KEY_MAX;i++)
		if(key_table[i].SDL_key == SDLkey){
			key = key_table[i].key;
			break;
		}
	return key;
}
/******************************************************************************/
/**
  * A table representing the key values within the game and their linked sdlk
  * values.
  */
struct input_keys key_table[] = {
	{SDLK_UP, KEY_UP},
	{SDLK_DOWN, KEY_DOWN},
	{SDLK_LEFT, KEY_LEFT},
	{SDLK_RIGHT, KEY_RIGHT},
	{SDLK_LCTRL, KEY_ATTACK},
	{SDLK_SPACE, KEY_USE},
	{SDLK_ESCAPE, KEY_ESCAPE},
	{-1, 0}
};
/******************************************************************************/
/**
  * Creates the heart entity which will be used to represent the hero's current
  * health.
  * @return The entity representing the hearts used for health representation
  */
Entity_T *CreateHeart()
{
	Entity_T *health = NewEntity();
	Sprite_T *s = NewSprite();
	Rect r;
	Animation_T a;
	SetupSprite(s, "heart.png", &r, 1, &a, 1); 
	SetupGraphic(health, s, GRAPHICTYPE_ELEMENT, -1, 0);
	return health;
}
/******************************************************************************/
/** Handles any setup for a new state and decides if updating should be interrupted
  * @return A boolean signifying if interrupt should be interrupted
  */
bool SetupState()
{
	Entity_T *e;
	Sprite_T *s;
	Rect r;
	Animation_T a;
	Map_T *map;
	bool interrupt = true;
	if(!IS_SET(game->flags, GAMEFLAG_SETUP_STATE))
		return false;
	switch(game->state)
	{
	case GAMESTATE_INTRO:
			e = NewEntity();
			s = NewSprite();
			SetupSprite(s, "splash.png", &r, 1, &a, 1); 
			SetupGraphic(e, s, GRAPHICTYPE_SPLASH, 2000, GRAPHFLAG_FADE|GRAPHFLAG_FULLSCREEN);
			AddSplashScreen(e);
			e = NewEntity();
			s = NewSprite();
			SetupSprite(s, "chipmunk.png", &r, 1, &a, 1);
			SetupGraphic(e, s, GRAPHICTYPE_SPLASH, 2000, GRAPHFLAG_FADE|GRAPHFLAG_FULLSCREEN);
			AddSplashScreen(e);
			interrupt = false;
			break;
	case GAMESTATE_MAIN_MENU:
			//TODO: Encapsulate this behavior later into a formal menu system
			e = NewEntity();
			s = NewSprite();
			SetupSprite(s, "main_menu.png", &r, 1, &a, 1);
			SetupGraphic(e, s, GRAPHICTYPE_SPLASH, 0, GRAPHFLAG_FADE_IN|GRAPHFLAG_FULLSCREEN);
			AddSplashScreen(e);
			e = NewEntity();
			s = NewSprite();
			SetupSprite(s, "cursor.png", &r, 1, &a, 1);
			s->frames[0].w = 1;
			s->frames[0].h = 1;
			SetupGraphic(e, s, GRAPHICTYPE_ELEMENT, 0, GRAPHFLAG_FADE_IN);
			e->pos.x = -.55f;
			e->pos.y = -.30f;
			e->size.x = .1f;
			e->size.y = .1f;
			game->foreGraphicsTail->next = e;
			game->foreGraphicsTail = e;
			interrupt = false;
			break;
	case GAMESTATE_PLAYING:
			//Set up the hero
			game->hero = SetupHero();
			game->heart = CreateHeart();
			map = StartMap();
			game->map = map;
			MoveEntity(game->hero, map, PosAtTile(map->start));
			REMOVE_FLAG(game->flags, GAMEFLAG_SETUP_STATE);
			interrupt = false;
			break;
	}
	REMOVE_FLAG(game->flags, GAMEFLAG_SETUP_STATE);
	return interrupt;
}
/******************************************************************************/
void UpdatePhysics(GLuint delta)
{
	cpSpaceStep(game->map->space, cpFloat(delta));
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
