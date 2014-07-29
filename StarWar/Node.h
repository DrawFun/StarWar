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
	virtual void Update() = 0;
	virtual bool InitPosition() = 0;
	virtual bool InitVertices() = 0;
	virtual bool InitTextures() = 0;
	virtual bool InitNormals() = 0;
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	D3DXVECTOR3 GetScale();
};

#endif
