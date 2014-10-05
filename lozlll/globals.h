/******************************************************************************/
/**List of global definitions, enumerations and the external current time 
 * variable.
 * @author Ulysee Thompson
 */
/******************************************************************************/
#ifndef __DEFINES_H
#define __DEFINES_H
/******************************************************************************/
#define GAME_NAME "Legend of Zelda : Link to the Darkness"
/******************************************************************************/
#define STARTING_NUM_SPRITES	100
#define STARTING_NUM_ENTITIES	20
/******************************************************************************/
#define MAX_PARTICLES			10
//Screen dimension constants
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
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
#define SET_BIT(a, b) (a |= b)
#define REMOVE_BIT(a, b) (a &= ~b)
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
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/