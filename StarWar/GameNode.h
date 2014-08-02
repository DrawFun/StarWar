#ifndef _STARWAR_NODE_H_
#define _STARWAR_NODE_H_

#include "Transform.h"
#include "Collider.h"
#include "Scene.h"
#include <list>

class CGameNode
{
protected:
	CScene *m_scene;
	CTransform m_transform;
	std::vector<Collider> m_colliders;
	
	GameNodeType m_type;
	float m_moveSpeed;

	bool m_enableRender;
	bool m_enablePhysics;
	bool m_enableControl;

	bool m_isControlable;
	bool m_isFlyable;

public:
	bool InitTransform(const CTransform &transform);
	virtual bool InitVertices() = 0;	
	virtual bool InitColliders() = 0;

	virtual void Render(LPDIRECT3DDEVICE9 pd3dDevice) = 0;
	virtual void Update() = 0;

	virtual void CollidingCallback(CGameNode *collided) = 0;
	virtual void CollidedCallback(CGameNode *colliding) = 0;

	GameNodeType GetType();
	std::vector<Collider> GetCollider();
	CTransform *GetTransform();

	void SwitchRender(bool flag){m_enableRender = flag;};
	void SwitchPhysics(bool flag){m_enablePhysics = flag;};
	void SwitchControl(bool flag){m_enableControl = flag;};
	bool IsEnableRender(){return m_enableRender;};
	bool IsEnablePhysics(){return m_enablePhysics;};

	bool IsEnableControl(){return m_enableControl;};
	bool IsFlyable(){return m_isFlyable;};
	bool IsControlable(){return m_isControlable;};

	float GetMoveSpeed(){return m_moveSpeed;};
	void SetMoveSpeed(float speed){m_moveSpeed = speed;};

	CScene* GetScene(){return m_scene;};
	void SetScene(CScene *scene){m_scene = scene;};
};

#endif
