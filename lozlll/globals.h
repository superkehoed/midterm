/******************************************************************************/
/**List of global definitions, enumerations and the external current time 
 * variable.
 * @author Ulysee Thompson
 */
/******************************************************************************/
#ifndef __DEFINES_H
#define __DEFINES_H
/******************************************************************************/
#define GAME_NAME "Cave of the Lightblob"
/******************************************************************************/
#define STARTING_NUM_SPRITES	100
#define STARTING_NUM_ENTITIES	20
/******************************************************************************/
#define MAX_PARTICLES			10
/******************************************************************************/
#define MAX_EVENT_VARIABLES		10
//Screen dimension constants
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define MILLISECONDS_PER_FRAME	30
/******************************************************************************/
/** The typedefined enumeration bringing booleans to C */
typedef enum bool{
	false,
	true
}bool;
/******************************************************************************/
#define MAX_FRAMES	100
#define MAX_ANIMATIONS				16
#define MAX_FRAMES_PER_ANIMATION	12
/******************************************************************************/
extern struct Engine_T *game;
/******************************************************************************/
//Macros
#define UMIN(a, b)	(a < b ? a : b)
#define UMAX(a, b)  (a < b ? b : a)
#define URANGE(a, b, c) UMIN(c, UMAX(a, b))

#define IS_SET(a, b) (a & b)
#define SET_FLAG(a, b) (a |= b)
#define REMOVE_FLAG(a, b) (a &= ~b)
/******************************************************************************/
typedef struct Vec2D
{
	GLfloat x;
	GLfloat y;
}Vec2D;
/******************************************************************************/
typedef struct Vec3D
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
}Vec3D;
/******************************************************************************/
typedef struct RGB
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
}RGB;
/******************************************************************************/
typedef struct RGBA
{
	float r;
	float g;
	float b;
	float a;
}RGBA;
/******************************************************************************/
typedef struct Rect
{
	GLfloat x;
	GLfloat y;
	GLfloat w;
	GLfloat h;
}Rect;
#endif //__DEFINES_H
/******************************************************************************/
#define VARTYPE_INT				0
#define VARTYPE_CHAR			1
#define VARTYPE_BOOL			2
#define VARTYPE_STRING			3
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/