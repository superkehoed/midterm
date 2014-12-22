/******************************************************************************/
#ifndef __LIGHT_H
#define __LIGHT_H
/******************************************************************************/
typedef struct Light_T
{
	Vec3f color; /**< The color of the light */
	GLfloat brightness; /**< How bright the light is initially */  
	GLfloat span;	/**< How far the light should reach, maximally*/
	Vec2f offset; /**< The offset to be applied to the light's position relative
				       to the bearer */
	struct Light_T *next; /**< Pointer to the next light in the resource list */
}Light_T;
/******************************************************************************/
Light_T *NewLight();
/******************************************************************************/
#endif //__LIGHT_H
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/