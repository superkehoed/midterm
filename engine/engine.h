/******************************************************************************/
/**
@file engine.h
 */
/******************************************************************************/
#ifndef __ENGINE_H
#define __ENGINE_H
/******************************************************************************/
/** Gamestate definitions */
#define GAMESTATE_UNINITIALIZED		0
#define GAMESTATE_INTRO				1
#define GAMESTATE_MAIN_MENU			2
#define GAMESTATE_CREDITS			3
#define GAMESTATE_OPTIONS			4
#define GAMESTATE_PLAYING			5
#define GAMESTATE_GAME_MENU			6
/******************************************************************************/
#define GAMEFLAG_SETUP_STATE		0x1
#define GAMEFLAG_SELECTING			0x2
/******************************************************************************/
/** Basic structure for handling a game engine */
typedef struct Engine_T{
	bool done; /**< Is the game done running */
	SDL_Window* window; /**< The window the game renders to */
	SDL_GLContext context; /**< The OpenGL context of the window */
	Shader_T *shader; /**< The shader being used by the engine */
	Entity_T *hero; /**< The entity the player is currently controlling */
	Entity_T *foreGraphicsHead; /**< The foreground graphics for the game */
	Entity_T *foreGraphicsTail; /**< The foreground graphics for the game */
	Entity_T *backGraphicsHead; /**< The background graphics for the game */
	Entity_T *backGraphicsTail; /**< The background graphics for the game */
	RenderBatch_T mapBatch; /**< The render batch for the tiles on screen */
	RenderBatch_T entBatch; /**< The render batch for the entities on screen */ 
	struct Map_T *map; /**< The current map the hero is on */
	int selection; /**< The current menu selection */
	long flags; /**< The current gamestate related flags */
	int state; /**< The current gamestate */
	int entityCount; /**< The current global count for entities */
	int spriteCount; /**< The current global count for sprites */
	int fontCount; /**< The current global count for sprites */
	float darkness;
	GLuint lastUpdate; /**< The last time an update occurred */
	GLuint currentTime;	/**< The current time for the game */
	//SpriteData *mSpriteList; /**< Linked list for tracking sprite
}Engine_T;
/******************************************************************************/
void EndGame();
bool Startup();
bool InitializeGL();
void Shutdown();
void MainLoop();
void AddSplashScreen(Entity_T *e);
void UpdateForeGraphics();
void UpdateBackGraphics();
void DequeueForeGraphics(bool free);
void DequeueBackGraphics(bool free);
GLuint LoadTex(const char *name);
int SDL_InvertSurface(SDL_Surface* image);
GLuint SurfaceToTexture(SDL_Surface *surface);
/******************************************************************************/
//General Functions
GLuint Random(GLuint num);
Vec2f Vec3fToVec2f(Vec3f vec);
/******************************************************************************/
//Externals
extern Engine_T *game;
/******************************************************************************/
#endif //__ENGINE_H
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/