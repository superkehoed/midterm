/******************************************************************************/
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
/** Basic structure for handling a game engine */
typedef struct Engine_T{
	bool done; /**< Is the game done running */
	SDL_Window* window; /**< The window the game renders to */
	SDL_GLContext context; /**< The OpenGL context of the window */
	Shader_T *shader; /**< The shader being used by the engine */
	int state; /**< The current gamestate */
	int entityCount; /**< The current global count for entities */
	int spriteCount; /**< The current global count for sprites */
	int fontCount; /**< The current global count for sprites */
	int lastUpdate; /**< The last time an update occurred */
	//SpriteData *mSpriteList; /**< Linked list for tracking sprite
}Engine_T;
/******************************************************************************/
void EndGame(Engine_T *e);
bool Startup(Engine_T *e);
bool InitializeGL(Engine_T *e);
void Shutdown(Engine_T *e);
void MainLoop(Engine_T *e);
/******************************************************************************/
//Externals
extern Engine_T *game;
/******************************************************************************/
#endif //__ENGINE_H
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/