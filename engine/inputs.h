struct input_keys
{
	long SDL_key;
	unsigned int key;
};

#define KEY_UP		0
#define KEY_DOWN	1
#define KEY_LEFT	2
#define KEY_RIGHT	3
#define KEY_ATTACK	4
#define KEY_USE		5
#define KEY_ESCAPE	6
#define KEY_MAX		7

#define KEYSTATE_NONE		0
#define KEYSTATE_ERROR		1
#define KEYSTATE_RELEASED	2
#define KEYSTATE_PRESSED	3
#define KEYSTATE_HELD		4

extern struct input_keys key_table[];
