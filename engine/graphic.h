#ifndef __GRAPHIC_H
#define __GRAPHIC_H

//Graphic types
#define GRAPHICTYPE_SPLASH	1
#define GRAPHICTYPE_WINDOW	2
#define GRAPHICTYPE_ELEMENT	3

//Graphic effect flags
#define GRAPHFLAG_FADE_IN		0x1
#define GRAPHFLAG_FADE_OUT		0x2
#define GRAPHFLAG_WIPE_IN		0x4
#define GRAPHFLAG_WIPE_OUT		0x8
#define GRAPHFLAG_WIPE_LEFT		0x10
#define GRAPHFLAG_FLICKER		0x40
#define GRAPHFLAG_FULLSCREEN	0x80

#define GRAPHFLAG_FADE	GRAPHFLAG_FADE_IN|GRAPHFLAG_FADE_OUT
/**
 * Structure for the information about graphics
 */
typedef struct Graphic_T{
	int type;	/**< The type of graphic */
	long flags; /**< The graphics flags set on the graphic */
	GLuint startTime; /**< The start time of the graphic */
	GLuint endTime; /**< The end time of the graphic */
}Graphic_T;


void SetupGraphic(struct Entity_T *ent, struct Sprite_T *sprite, short type, GLuint timer, long flags);
void DrawGraphic(struct Entity_T *graphic);
void DrawHUD();
#endif //__GRAPHIC_H