/******************************************************************************/
/**List of global definitions, enumerations and the external current time 
 * variable.
 * @author Ulysee Thompson
 */
/******************************************************************************/
#ifndef __DEFINES_H
#define __DEFINES_H
/******************************************************************************/
#define GAME_NAME "Generic Quest : The Search For Excuses To Use Light Effects"
/******************************************************************************/
#define RESET_INDEX				0xFFFFFFFF
#define STARTING_NUM_SPRITES	100
#define STARTING_NUM_ENTITIES	20
/******************************************************************************/
#define MAX_PARTICLES			10
/******************************************************************************/
#define MAX_EVENT_VARIABLES		10
//Screen dimension constants
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024
#define MILLISECONDS_PER_FRAME	30
/******************************************************************************/
#define SCREEN_TILE_WIDTH		15
#define SCREEN_TILE_HEIGHT		12
#define SCREEN_MAX_SPRITES		200
/******************************************************************************/
#define TILE_WIDTH (32.0f / SCREEN_WIDTH )
#define TILE_HEIGHT (32.0f / SCREEN_HEIGHT)

/** The typedefined enumeration bringing booleans to C */
typedef enum bool{
	false,
	true
}bool;
/******************************************************************************/
#define MAX_ANIMATIONS				16
#define MAX_FRAMES_PER_ANIMATION	12
#define MAX_FRAMES_PER_SHEET		200
/******************************************************************************/
#define MAX_MAP_HEIGHT				256
#define MAX_MAP_WIDTH				256
/******************************************************************************/
/******************************************************************************/
#define MAX_VERTICES ((SCREEN_TILE_WIDTH + 1)*(SCREEN_TILE_HEIGHT + 1) + SCREEN_MAX_SPRITES)
#define MAX_BILLBOARDS (SCREEN_TILE_WIDTH * SCREEN_TILE_HEIGHT + SCREEN_MAX_SPRITES)
#define MAX_INDICES (4 * MAX_BILLBOARDS)
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
typedef struct Vec2f
{
	GLfloat x;
	GLfloat y;
}Vec2f;
/******************************************************************************/
typedef struct Vec2i
{
	GLint x;
	GLint y;
}Vec2i;
/******************************************************************************/
typedef struct Vec3f
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
}Vec3f;
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