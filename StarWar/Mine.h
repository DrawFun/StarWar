#ifndef _STARWAR_MINE_H_
#define _STARWAR_MINE_H_

#include "GameNode.h"

class CMine : public CGameNode
{
protected:
	float m_rotateSpeed;
	LPD3DXMESH pSnowmanMesh;
	D3DMATERIAL9 *pSnowmanMeshMaterials;
	unsigned long snowManNumMaterials;
	LPDIRECT3DTEXTURE9 pSnowmanTexture0;
	LPDIRECT3DTEXTURE9 pSnowmanTexture1;

public:
	CMine(float rotateSpeed);
	void Update();
	void Render(LPDIRECT3DDEVICE9 pd3dDevice);
	bool InitVertices();
	bool InitColliders();
	void CollidingCallback(CGameNode *collided);
	void CollidedCallback(CGameNode *colliding);
};

#endif
