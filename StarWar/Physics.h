#ifndef _STARWAR_PHYSICS_H_
#define _STARWAR_PHYSICS_H_

#include "Util.h"
#include "GameNode.h"

class CPhysics
{
private:
	//��ײ��ξ���
	std::vector<std::vector<bool>> m_layerMatrix;
	//��ײ����
	std::list<CGameNode*> m_collisionGameNodes[GameNodeType::LENGTH];

public:
	void CollisionCheck();
	CPhysics(std::list<CGameNode*> listRootGameNodes);

};

#endif
