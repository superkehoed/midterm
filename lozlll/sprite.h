/** Enimation for the animation flags affecting an animation's behavior */
enum AnimationFlags{
	ANIMFLAG_NONE = 0
};

/** Structure for animations used by sprites */
typedef struct Animation_T{
	int numFrames; /**< The number of frames in this animation */
	int frames[MAX_FRAMES_PER_ANIMATION]; /**< The frame numbers for this animation */
	int frameLengths[MAX_FRAMES_PER_ANIMATION]; /**< The length each frame should be shown for in this animation */
	long flags; /**< The flags that this animation operates by */
}Animation_T;

/** Structure for sprites that entities are represented by */
typedef struct Sprite_T{
	struct Sprite_T *next; /**< Used for the unused sprite list */
	long validation; /**< Validation time of the sprite */
	GLuint texId; /**< The id of the texture this sprite uses */
	SDL_Rect frames[MAX_FRAMES]; /**< The list of frames for this sprite */
	Animation_T Animations[MAX_ANIMATIONS]; /**< The list of animations for this sprite */
}Sprite_T;
//Externals
extern Sprite_T *gUnusedSpriteList;

//Functions
void Initialize(Sprite_T *sprite);
void DrawSprite(Sprite_T *sprite, GLuint x, GLuint y);
Sprite_T  *NewSprite();
void FreeSprite(Sprite_T *sprite);
extern Sprite_T *UnusedSpriteList;