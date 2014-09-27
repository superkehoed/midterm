#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include "shader.h"

void Shader::init(const char *vsfile, const char *fsfile)
{
	
	mShaderId = glCreateProgram();
	glBindAttribLocation(mShaderId, 0, "in_Position");
	glBindAttribLocation(mShaderId, 1, "in_Color");

}