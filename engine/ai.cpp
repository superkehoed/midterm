/******************************************************************************/
/******************************************************************************/
#include "include.h"
#include <stdio.h>

/******************************************************************************/
bool PushObject(Entity_T *obj, Entity_T *pusher, int dir)
{
	//Check if the object is pushable
	if(!IS_SET(obj->flags, ENTFLAG_PUSHABLE))
		return false;
	//Check if the object has an onpush function, call it and return its boolean
	if(obj->Pushed != NULL)
		return (*obj->Pushed)(obj, pusher, dir);
	else{
		//Else default to the standard behavior
	}
	return false;
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/