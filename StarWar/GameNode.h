#ifndef _STARWAR_NODE_H_
#define _STARWAR_NODE_H_

#include "DXEngine.h"
#include "Collider.h"

class CGameNode
{
protected:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_scale;
	D3DXVECTOR3 m_rotation;	
	std::vector<Collider> colliders;

public:		
	bool InitPosition
	(const D3DXVECTOR3 &position = D3DXVECTOR3(0, 0, 0), 
	 const D3DXVECTOR3 &scale = D3DXVECTOR3(1, 1, 1),
	 const D3DXVECTOR3 &rotation = D3DXVECTOR3(0, 0, 0));
	virtual bool InitVertices() = 0;
	virtual bool InitColliders() = 0;

	virtual void Update() = 0;
	
	void Roll(float angle);
	void Pitch(float angle);
	void Yaw(float angle);
	void Rotate(D3DXVECTOR3 &rotation);
	void Translate(D3DXVECTOR3 &position);
	void Scale(D3DXVECTOR3 &scale);
	void UpdateMatrix();

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	D3DXVECTOR3 GetScale();
	std::vector<Collider> GetCollider();
	void SetPosition(D3DXVECTOR3 &position);
	void SetScale(D3DXVECTOR3 &scale);
	void SetRotation(D3DXVECTOR3 &rotation);
};

#endif
