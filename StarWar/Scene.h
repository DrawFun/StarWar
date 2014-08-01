#ifndef _STARWAR_SCENE_H_
#define _STARWAR_SCENE_H_

#include "GameNode.h"
#include <vector>
#include <list>

class CScene
{
protected:
	std::list<CGameNode *> m_objectsList;
public:
	virtual void Update() = 0;
};

#endif