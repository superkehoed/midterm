/*This source code copyrighted by Lazy Foo' Productions (2004-2014)
and may not be redistributed without written permission.*/

//Using SDL, SDL OpenGL, GLEW, standard IO, and strings
#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <stdio.h>
#include "include.h"

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