/******************************************************************************/

/******************************************************************************/
//Using SDL, SDL OpenGL, GLEW, standard IO, and strings
#include "include.h"
#include <stdio.h>
/******************************************************************************/
int main( int argc, char* args[] )
{
	Engine_T engine;
	game = &engine;
	if(!Startup())
		printf("Failed to initialize!\n");
	while(!game->done)
		MainLoop();
	Shutdown();
	return 0;
}
/******************************************************************************/