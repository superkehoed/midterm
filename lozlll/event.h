#ifndef __EVENT_H
#define __EVENT_H

#define EVENTTYPE_TELEPORT		1
#define EVENTTYPE_SPAWN			2
#define EVENTTYPE_TOUCH			3
#define EVENTTYPE_ACTIVATE		4

typedef struct Event_T{
	short type;
	void *variables[MAX_EVENT_VARIABLES];
	short vartypes[MAX_EVENT_VARIABLES];
}Event_T;

#endif //__EVENT_H