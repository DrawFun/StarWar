#ifndef _STARWAR_PLATFORM_H_
#define _STARWAR_PLATFORM_H_

#include "GameNode.h"

class CPlatform : public CGameNode
{
protected:
	float m_width;
	float m_height;
	float m_depth;
	float m_horizontalSpeed;
	float m_rotationSpeed;
	int m_moveCounter;
	static const int PLATFORM_MOVE_FRAME_PERIOD = 1000;

	LPD3DXMESH m_pMesh; //Point to mesh
	D3DMATERIAL9 m_pMeshMaterials; //Point to mesh material
public:
	CPlatform(float width, float height, float depth, float horizonlSpeed, float rotationSpeed);
	void Update();
	void Render(LPDIRECT3DDEVICE9 pd3dDevice);
	bool InitVertices();
	bool InitColliders();
	
	void CollidingCallback(CGameNode *collided);
	void CollidedCallback(CGameNode *colliding);

	float GetHeight(){return m_height;};
	void Move();
};

#endif

