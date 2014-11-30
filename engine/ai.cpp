#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include <stdio.h>
#include "include.h"

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
	
}