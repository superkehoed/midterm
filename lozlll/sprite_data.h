/** Enimation for the animation flags affecting an animation's behavior */
enum AnimationFlags{
	ANIMFLAG_NONE = 0
};

/** Structure for animations used by sprites */
struct AnimationData{
	int numFrames; /**< The number of frames in this animation */
	int frames[MAX_FRAMES_PER_ANIMATION]; /**< The frame numbers for this animation */
	int frameLengths[MAX_FRAMES_PER_ANIMATION]; /**< The length each frame should be shown for in this animation */
	long flags; /**< The flags that this animation operates by */
};

/** Structure for sprites that entities are represented by */
struct SpriteData{
	SpriteData *next; /**< Used for the unused sprite list */
	long validation; /**< Validation time of the sprite */
	GLuint texId; /**< The id of the texture this sprite uses */
	SDL_Rect frames[MAX_FRAMES]; /**< The list of frames for this sprite */
	AnimationData Animations[MAX_ANIMATIONS]; /**< The list of animations for this sprite */
};
//Externals
extern SpriteData *gUnusedSpriteList;

//Functions
void Initialize(SpriteData *sprite);
void Draw(SpriteData *sprite, GLuint x, GLuint y);
SpriteData  *NewSprite();
void FreeSprite(SpriteData *sprite);
extern SpriteData *UnusedSpriteList;