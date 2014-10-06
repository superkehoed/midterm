#ifndef __MAP_H
#define __MAP_H

typedef struct Map_T
{
	char tiles[256][256];
	Event_T	*events;
}Map_T;

#endif //__MAP_H