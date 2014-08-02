#ifndef _STARWAR_ZERG_H_
#define _STARWAR_ZERG_H_

#include "TargetedBullet.h"

class CZerg : public CTargetedBullet
{
protected:
	float m_width;
	float m_height;
	float m_depth;	
	LPD3DXMESH m_pMesh; 
	D3DMATERIAL9 m_pMeshMaterials; 
	unsigned long m_materialsNum; //Number of material
	D3DXVECTOR3 m_originalPosition;

public:
	CZerg(float width, float height, float depth, float moveSpeed, CGameNode *target);
	void SetAlive();

	void Update();
	bool InitVertices();
	bool InitColliders();
	void Render(LPDIRECT3DDEVICE9 pd3dDevice);
	void CollidingCallback(CGameNode *collided);
	void CollidedCallback(CGameNode *colliding);
};

#endif
