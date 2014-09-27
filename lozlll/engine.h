/******************************************************************************/
/******************************************************************************/
#ifndef __ENGINE_H
#define __ENGINE_H
/******************************************************************************/
enum GameState_T{
	GAMESTATE_START,
	GAMESTATE_MAIN_MENU,
	GAMESTATE_CREDITS,
	GAMESTATE_OPTIONS,
	GAMESTATE_PLAYING
};
/******************************************************************************/
typedef struct EngineData{
	SDL_Window* window; /**< The window the game renders to */
	SDL_GLContext context; /**< The OpenGL context of the window */
	GameState_T state; /**< The current gamestate */
	int entityCount; /**< The current global count for entities */
	int spriteCount; /**< The current global count for sprites */
	int fontCount; /**< The current global count for sprites */
	int lastUpdate; /**< The last time an update occurred */
	//SpriteData *mSpriteList; /**< Linked list for tracking sprite
}Engine_T;
/******************************************************************************/
void Destroy(Engine_T *e);
void Initialize(Engine_T *e);
/******************************************************************************/
extern Engine_T *game;
/******************************************************************************/
#endif //__ENGINE_H
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/