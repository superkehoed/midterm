/******************************************************************************/
/******************************************************************************/
/** Enumeration for the animation flags affecting an animation's behavior */
enum AnimationFlags{
	ANIMFLAG_NONE = 0
};
/******************************************************************************/
/** Enumeration for animation */
enum Animations{
	ANIM_STAND_DOWN,
	ANIM_STAND_LEFT,
	ANIM_STAND_RIGHT,
	ANIM_STAND_UP,
	ANIM_WALK_DOWN,
	ANIM_WALK_LEFT,
	ANIM_WALK_RIGHT,
	ANIM_WALK_UP
};
/******************************************************************************/
/** Structure for animations used by sprites */
typedef struct Animation_T{
	GLuint numFrames; /**< The number of frames in this animation */
	GLuint frames[MAX_FRAMES_PER_ANIMATION]; /**< The frame numbers for this animation */
	GLuint frameLengths[MAX_FRAMES_PER_ANIMATION]; /**< The length each frame should be shown for in this animation */
	long flags; /**< The flags that this animation operates by */
}Animation_T;
/******************************************************************************/
/** Structure for sprites that entities are represented by */
typedef struct Sprite_T{
	struct Sprite_T *next; /**< Used for the unused sprite list */
	long validation; /**< Validation time of the sprite */
	GLuint texId; /**< The id of the texture this sprite uses */
	GLuint numFrames; /**< The number of frames in this sprite */
	GLuint numAnimations; /**< The number of animations in this sprite */
	Rect frames[MAX_FRAMES_PER_SHEET]; /**< The list of frames for this sprite */
	Animation_T animations[MAX_ANIMATIONS]; /**< The list of animations for this sprite */
}Sprite_T;
/******************************************************************************/
//Externals
extern Sprite_T *gUnusedSpriteList;
/******************************************************************************/
//Functions
void Initialize(Sprite_T *sprite); /**< Initializes a new sprite */
void DrawSprite(Sprite_T *sprite, GLuint x, GLuint y); /**< Draws a sprite on the screen */
Sprite_T  *NewSprite(); /**< Provides a pointer to a new sprite for the engine */
void FreeSprite(Sprite_T *sprite); /**< Frees the sprite to the unused list */
void SetupSprite(Sprite_T *s, const char *file, 
				Rect *frames, short frame_num,
				Animation_T *animations, short anim_num);
Sprite_T *CreateBasicSprite(const char *filename, Vec2i frame_size);
void GetSpriteUVs(Sprite_T *s, GLuint frame, Vec2f *ll, Vec2f *ur);
/******************************************************************************/