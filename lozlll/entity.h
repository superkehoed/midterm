#ifndef __ENTITY_H
#define __ENTITY_H

//Types of entities
#define ENTYPE_EFFECT	0
#define ENTYPE_ALIVE	1
#define ENTYPE_OBJECT	2
#define ENTYPE_GRAPHIC	3

//Flags for entities
#define ENTFLAG_HARMFUL		0x1
#define ENTFLAG_BLOCKING	0x2

typedef struct Entity_T{
	short type;
	Vec3D pos;
	Vec3D velocity;
	Vec3D acceleration;
	union{
		Sprite_T *sprite;
		Sprite_T *particles[MAX_PARTICLES];
	};

	union{
		//CharData_T status;
		Graphic_T display;
	};
	GLuint currentFrame;
	GLuint nextFrame;
	struct Entity_T *next;
	//Effect data here?
}Entity_T;

extern Entity_T *gUnusedEntityList;
void FreeEntity(Entity_T *ent);
Entity_T *NewEntity();
#endif //__ENTITY_H