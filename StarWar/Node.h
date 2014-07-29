#ifndef _STARWAR_NODE_H_
#define _STARWAR_NODE_H_

#include "Util.h"

class CNode
{
protected:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_scale;
	D3DXVECTOR3 m_rotation;	

public:
	virtual void Update(LPDIRECT3DDEVICE9 pd3dDevice) = 0;
	virtual bool InitPosition
	(const D3DXVECTOR3 &position = D3DXVECTOR3(0, 0, 0), 
	 const D3DXVECTOR3 &scale = D3DXVECTOR3(1, 1, 1),
	 const D3DXVECTOR3 &rotation = D3DXVECTOR3(0, 0, 0)) = 0;
	virtual bool InitVertices(LPDIRECT3DDEVICE9 pd3dDevice) = 0;
	virtual bool InitNormals() = 0;
	virtual bool InitTextures() = 0;	
	void Roll();
	void Pitch();
	void Yaw();
	void Rotate();
	void Translate();
	void Scale();
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	D3DXVECTOR3 GetScale();
};

#endif
