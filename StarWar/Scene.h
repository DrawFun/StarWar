#ifndef _STARWAR_SCENE_H_
#define _STARWAR_SCENE_H_

#include "Util.h"

class CScene
{
public:	
	virtual void Update(ControllerInput &input) = 0;
	virtual void EventCallBack(int triggerEvent, void *trigger) = 0;
};

#endif