#ifndef _STARWAR_NODE_H_
#define _STARWAR_NODE_H_

#include "Transform.h"
#include "Collider.h"
#include <list>

class CGameNode
{
protected:
	CTransform m_transform;
	std::vector<Collider> m_colliders;
	
	GameNodeType m_type;
	float m_moveSpeed;
	CGameNode *m_parents;
	std::list<CGameNode *> m_children;

public:		
	bool InitTransform(const CTransform &transform);
	virtual bool InitVertices() = 0;
	virtual bool InitColliders() = 0;

	virtual void Update() = 0;

	virtual void CollidingCallback(CGameNode *collided) = 0;
	virtual void CollidedCallback(CGameNode *colliding) = 0;

	float GetMoveSpeed();
	GameNodeType GetType();
	std::vector<Collider> GetCollider();
	CTransform& GetTransform();
	CGameNode *GetParents();

	void SetMoveSpeed(float speed);
	void SetParents(CGameNode *parents);
};

#endif
