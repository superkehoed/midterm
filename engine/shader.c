/******************************************************************************/
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
unsigned long GetFileLength(FILE *file)
{
	int length = 0;
    fseek(file, 0, SEEK_END);
	length = ftell(file);
	rewind(file);
	return length;
}

void ValidateShader(GLuint shader, const char* file) {
	const unsigned int BUFFER_SIZE = 512;
	char buffer[512];
	GLsizei length = 0;
    memset(buffer, 0, 512);

	glGetShaderInfoLog(shader, 512, &length, buffer);
	if (length > 0) {
		printf("Shader %d (%s) compile error: %s\n", shader, (file?file:""), buffer);
	}
}

int LoadShaderSource(const char* filename, GLchar** source, unsigned long* len)
{
   FILE *file;
   unsigned int i=0;
   GLchar *txt;
   unsigned long size;

   file = fopen(filename, "rb"); // opens as Binary
   if(!file) 
	   return -1;
   *len = GetFileLength(file);
   fclose(file);
   if(*len <= 0)
	   return -2;
   size = *len;
   file = fopen(filename, "r"); //Opens as ASCII
   *source = (GLchar*)malloc(sizeof(GLchar)*(*len));
   txt = *source;
   if (*source == 0) return -3;   // can't reserve memory
   
    // len isn't always strlen cause some characters are stripped in ascii read...
    // it is important to 0-terminate the real length later, len is just max possible value... 
   txt[size-1] = 0; 

   
   while ((txt[i] = fgetc(file)) != EOF)
        i++;
    
   txt[i] = 0;  // 0-terminate it at the correct position
   fclose(file);
      
   return 0; // No Error
}
/******************************************************************************/
int UnloadShaderSource(GLchar** source)
{
   if (*source != 0)
     free(*source);
   *source = 0;
   return 0;
}

/******************************************************************************/
bool InitializeShader(Shader_T *s, const char *vsfile, const char *fsfile)
{
	bool success = true;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLint vShaderCompiled = GL_FALSE;
	GLint fShaderCompiled = GL_FALSE;
	GLint programSuccess = GL_TRUE;
	GLchar *txt;
	unsigned long len;	
	//Generate program
	s->id = glCreateProgram();
	s->vertex_attrib = -1;
	s->uv_attrib = -1;
	//Create vertex shader
	vertexShader = glCreateShader( GL_VERTEX_SHADER );
	//Set vertex source
	LoadShaderSource(vsfile, &txt, &len);
	glShaderSource( vertexShader, 1, &txt, NULL );
	//Compile vertex source
	glCompileShader( vertexShader );
	ValidateShader(vertexShader, vsfile);
	UnloadShaderSource(&txt);
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
		LoadShaderSource(fsfile, &txt, &len);
		fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
		//Set fragment source
		glShaderSource( fragmentShader, 1, &txt, NULL );
		//Compile fragment source
		glCompileShader( fragmentShader );
		ValidateShader(fragmentShader, fsfile);
		UnloadShaderSource(&txt);
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
				s->uv_attrib = glGetAttribLocation( s->id, "vertexUV" );
				s->vertex_attrib = glGetAttribLocation( s->id, "vertexPosition" );
				if( s->vertex_attrib == -1 )
				{
					printf( "vertexPosition is not a valid glsl program variable!\n" );
					success = false;
				}
				if(s->uv_attrib == -1 )
				{
					printf( "vertexTex is not a valid glsl program variable!\n" );
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