/******************************************************************************/
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "shader.h"
/******************************************************************************/
bool InitializeShader(Shader_T *s, const char *vsfile, const char *fsfile)
{
	bool success = true;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLint vShaderCompiled = GL_FALSE;
	GLint fShaderCompiled = GL_FALSE;
	GLint programSuccess = GL_TRUE;
	//TODO: Fix this to use real files
	//Get vertex source
	const GLchar* vertexShaderSource[] =
	{
		"#version 400\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
	};
	//Get fragment source
	const GLchar* fragmentShaderSource[] =
	{
		"#version 400\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
	};	
	//Generate program
	s->id = glCreateProgram();
	s->vertex_attrib = -1;
	//Create vertex shader
	vertexShader = glCreateShader( GL_VERTEX_SHADER );
	//Set vertex source
	glShaderSource( vertexShader, 1, vertexShaderSource, NULL );
	//Compile vertex source
	glCompileShader( vertexShader );
	//Check vertex shader for errors
	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &vShaderCompiled );
	if( vShaderCompiled != GL_TRUE )
	{
		printf( "Unable to compile vertex shader %d!\n", vertexShader );
		printShaderLog( vertexShader );
        success = false;
	}
	else
	{
		//Attach vertex shader to program
		glAttachShader( s->id, vertexShader );
		//Create fragment shader
		fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
		//Set fragment source
		glShaderSource( fragmentShader, 1, fragmentShaderSource, NULL );
		//Compile fragment source
		glCompileShader( fragmentShader );
		//Check fragment shader for errors
		glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled );
		if( fShaderCompiled != GL_TRUE )
		{
			printf( "Unable to compile fragment shader %d!\n", fragmentShader );
			printShaderLog( fragmentShader );
			success = false;
		}
		else
		{
			//Attach fragment shader to program
			glAttachShader( s->id, fragmentShader );
			//Link program
			glLinkProgram( s->id );
			//Check for errors
			glGetProgramiv( s->id, GL_LINK_STATUS, &programSuccess );
			if( programSuccess != GL_TRUE )
			{
				printf( "Error linking program %d!\n", s->id );
				printProgramLog( s->id );
				success = false;
			}
			else
			{
				//Get vertex attribute location
				s->vertex_attrib = glGetAttribLocation( s->id, "LVertexPos2D" );
				if( s->vertex_attrib == -1 )
				{
					printf( "LVertexPos2D is not a valid glsl program variable!\n" );
					success = false;
				}
			}
		}
	}
	return success;
}
/******************************************************************************/
void printProgramLog( GLuint program )
{
	int infoLogLength = 0;
	int maxLength;
	char *infoLog;
	//Make sure name is shader
	if( glIsProgram( program ) )
	{
		//Program log length
		maxLength = infoLogLength;	
		//Get info string length
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
		
		//Allocate string
		infoLog = (char*)malloc(sizeof(char)*maxLength);
		
		//Get info log
		glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			printf( "%s\n", infoLog );
		}
		
		//Deallocate string
		free(infoLog);
	}
	else
	{
		printf( "Name %d is not a program\n", program );
	}
}
/******************************************************************************/
void printShaderLog( GLuint shader )
{
	int infoLogLength = 0;
	int maxLength;
	char *infoLog;
	//Make sure name is shader
	if( glIsShader( shader ) )
	{
		//Shader log length
		maxLength = infoLogLength;
		
		//Get info string length
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
		
		//Allocate string
		infoLog = (char*)malloc(sizeof(char)*maxLength);
		
		//Get info log
		glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			printf( "%s\n", infoLog );
		}

		//Deallocate string
		free(infoLog);
	}
	else
	{
		printf( "Name %d is not a shader\n", shader );
	}
}
/******************************************************************************/
/** Creates a new shader object
  * @returns A new, uninitialized Shader_T object 
  */
Shader_T *NewShader()
{
	//This event is rare enough that for the time being, I might as well malloc all of them.
	//Consider adding a memory handler later if that changes.
	return (Shader_T*)malloc(sizeof(Shader_T));
}
/******************************************************************************/
/** Does the destructor process for shaders before freeing them
  * @param s A pointer to the shader to be destroyed
  */
void DestroyShader(Shader_T *s)
{
	//Delete the shader's program
	glDeleteProgram( s->id );
	//Free the shader's memory
	free(s);
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/