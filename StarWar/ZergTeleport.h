#ifndef _STARWAR_ZERGTELEPORT_H_
#define _STARWAR_ZERGTELEPORT_H_

#include "GameNode.h"
#include "HitPoint.h"

class CZergTeleport : public CGameNode
{
protected:
	CHitPoint m_hp;

	float m_width;
	float m_height;
	float m_depth;	
	float m_rotationSpeed;
	LPD3DXMESH m_pMesh; 
	D3DMATERIAL9 m_meshMaterials; 
	unsigned long m_materialsNum; //Number of material
	int m_generationCounter;
	static const int GENERATION_FRAME_PERIOD = 200;

public:
	CZergTeleport(float width, float height, float depth, float rotationSpeed);
	~CZergTeleport();

	void Update();
	bool InitVertices();
	bool InitColliders();
	CHitPoint *GetHitPoint(){return &m_hp;};
	void Render(LPDIRECT3DDEVICE9 pd3dDevice);
	void CollidingCallback(CGameNode *collided);
	void CollidedCallback(CGameNode *colliding);
};

#endif
