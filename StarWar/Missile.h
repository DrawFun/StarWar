#ifndef _STARWAR_MISSILE_H_
#define _STARWAR_MISSILE_H_

#include "TargetedBullet.h"

class CMissile : public CTargetedBullet
{
protected:
	float m_radius1;
	float m_radius2;
	float m_length;	
	LPD3DXMESH m_pMesh; 
	D3DMATERIAL9 m_meshMaterials; 
	unsigned long m_materialsNum; //Number of material

	int m_destroyCounter;
	static const long long DESTROY_FRAME_PERIOD = 1000;
	
public:
	CMissile(float radius1, float radius2, float length, float moveSpeed, CGameNode *target);
	~CMissile();
	void SetAlive();

	void Update();
	bool InitVertices();
	bool InitColliders();
	void Render(LPDIRECT3DDEVICE9 pd3dDevice);
	void CollidingCallback(CGameNode *collided);
	void CollidedCallback(CGameNode *colliding);
};

#endif
