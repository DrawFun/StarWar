#ifndef _STARWAR_BULLET_H_
#define _STARWAR_BULLET_H_

#include "GameNode.h"

class CTargetedBullet : public CGameNode
{
protected:
	CGameNode *m_target;
	bool m_isAlive;

	void MoveToTarget();
	void Recycle();

public:
	CTargetedBullet(CGameNode *target) : m_target(target){};
	void SetTarget(CGameNode *target) {m_target = target;};
	bool IsAlive(){ return m_isAlive;};
	virtual void SetAlive(){ m_isAlive = true;};
};

#endif