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
	Entity_T *hero; /**< The entity the player is currently controlling */
	Entity_T *foreGraphicsHead; /** The foreground graphics for the game */
	Entity_T *foreGraphicsTail; /** The foreground graphics for the game */
	Entity_T *backGraphicsHead; /** The background graphics for the game */
	Entity_T *backGraphicsTail; /** The background graphics for the game */
	Entity_T *splashScreensHead; /** The splash screens for the game */
	GLuint	vertexBuffer; /**< The vertex buffer object*/
	GLuint	textureBuffer; /**< The UV buffer object*/

	int state; /**< The current gamestate */
	int entityCount; /**< The current global count for entities */
	int spriteCount; /**< The current global count for sprites */
	int fontCount; /**< The current global count for sprites */
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
GLuint LoadTex(const char *name);
/******************************************************************************/
//Externals
extern Engine_T *game;
/******************************************************************************/
#endif //__ENGINE_H
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/