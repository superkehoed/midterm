/******************************************************************************/
#ifndef __ENTITY_H
#define __ENTITY_H
/******************************************************************************/
//Types of entities
#define ENTYPE_EFFECT	0	/**< Entity is an effect */
#define ENTYPE_CREATURE	1   /**< Entity is a living, animated creature */
#define ENTYPE_OBJECT	2   /**< Entity is a nonliving, limited animation creature */
#define ENTYPE_GRAPHIC	3   /**< Entity is a graphic */
/******************************************************************************/
//Flags for entities
#define ENTFLAG_HARMFUL		0x1 /**< Touching the entity is harmful to the hero */
#define ENTFLAG_BLOCKING	0x2 /**< The entity cannot be passed through by the player */
#define ENTFLAG_PUSHABLE	0x3 /**< The entity can be pushed */
/******************************************************************************/
//Directions entities may face
#define ENTDIR_DOWN		0
#define ENTDIR_LEFT		1
#define ENTDIR_RIGHT	2
#define ENTDIR_UP		3
/******************************************************************************/
#define ENTSTATE_STAND	0
#define ENTSTATE_WALK	1
/******************************************************************************/
#define IS_STANDING(e) (e->state == ENTSTATE_STAND)
#define IS_WALKING(e) (e->state == ENTSTATE_WALK)
/******************************************************************************/
typedef struct Entity_T{
	short type; /**< The type of entity */
	Vec2f size; /**< The size of the entity when drawn on the screen */
	Vec3f pos; /**< The current position of the entity within the map */
	Vec3f velocity; /**< The velocity for the entity */
	Vec3f acceleration; /**< The acceleration for the entity */
	Tile_T *onTile; /**< The tile the entity is on */
	Map_T *onMap; /**< The map the entity is on */
	union{
		Sprite_T *sprite; /**< The sprite which represents this entity */
		Sprite_T *particles[MAX_PARTICLES]; /**< The particles this graphic uses to be drawn */
	};
	Graphic_T display; /**< The display data for the graphic */
	GLuint state; /**< The state the entity is currently in */
	GLuint dir;	/**< The direction the entity is currently facing */
	GLuint currentAnimation; /**< The current animation the entity is on */
	GLuint currentFrame; /**< The current frame the entity is animating on */
	GLuint nextFrame; /**< The time the next frame should be started */
	GLulong flags; /**< The flags for the entity */
	void (*Think)(struct Entity_T *e, GLuint delta); /**< A pointer for the entity's think function */
	bool (*Pushed)(struct Entity_T *e, struct Entity_T *pusher, GLuint dir); /**< Pointer for the entity's callback for when pushed */ 
	GLuint nextThink; /**< Timer for next think to be carried out */
	struct Entity_T *next; /**< The next entity in the linked list */
	//Effect data here?
}Entity_T;
/******************************************************************************/
extern Entity_T *gUnusedEntityList; /**< Pointer to the unused entity list */
void FreeEntity(Entity_T *ent);     /**< Frees the entity back to the unused list*/
Entity_T *NewEntity(); /**< Provides a cleared entity for the engine to use */
void GetEntityUVs(Entity_T *e, Vec2f *ll, Vec2f *ur);
void GetEntitySize(Entity_T *e, Vec2f *size); 
void WalkEntity(Entity_T *e, int direction);
bool UpdateEntity(Entity_T *e, GLuint delta);
Entity_T *SetupHero();
Entity_T *SetupMonster(int monster);
void MonsterHunt(Entity_T *e, GLuint delta);
void StartAnimation(Entity_T *e, GLuint anim);

/******************************************************************************/
#endif //__ENTITY_H
/******************************************************************************/