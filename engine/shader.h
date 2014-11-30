/******************************************************************************/
#ifndef __SHADER_H
#define __SHADER_H
/******************************************************************************/
typedef struct Shader_T{
	GLuint id; /**< ProgramID for the shader */
	GLuint VBO; /**< Vertex Buffer Object id */
	GLuint IBO; /**< Index Buffer Object id */
	GLuint vertex_attrib; /**< Id for the 2d vertex attribute */
	GLuint uv_attrib; /**< Id for the 2d uv attribute */
	GLuint vertexBuffer; /**< The vertex buffer object*/
	GLuint textureBuffer; /**< The UV buffer object*/
	GLuint indexBuffer; /**< The index buffer object */
}Shader_T;
/******************************************************************************/
bool InitializeShader(Shader_T *s, const char *vsfile, const char *fsfile);
Shader_T *NewShader();
void DestroyShader(Shader_T *s);
/******************************************************************************/
void printProgramLog( GLuint program );
void printShaderLog( GLuint shader );
/******************************************************************************/
#endif //__SHADER_H
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/