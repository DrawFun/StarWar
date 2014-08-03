#ifndef _STARWAR_SCENE_H_
#define _STARWAR_SCENE_H_
#include "Util.h"
#include "Transform.h"
class CScene
{
protected:
	CTransform rootNode;	
public:	
	virtual void Update(ControllerInput &input) = 0;
	virtual void EventCallBack(int triggerEvent, void *trigger) = 0;
	CTransform *GetRootNode(){return &rootNode;};
};

#endif