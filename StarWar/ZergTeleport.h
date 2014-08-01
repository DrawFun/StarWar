#ifndef _STARWAR_ZERGTELEPORT_H_
#define _STARWAR_ZERGTELEPORT_H_

#include "GameNode.h"

class CZergTeleport : public CGameNode
{
protected:
	LPDIRECT3DTEXTURE9 m_pTexture; 
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer; //D3D vertext buffer 
	LPD3DXMESH m_pMesh; 
	D3DMATERIAL9 *m_pMeshMaterials; 
	unsigned long m_materialsNum; //Number of material

public:
	CZergTeleport(){m_type = ZERG_TELEPORT;};
	void Update();
	bool InitVertices();
	bool InitColliders();
	
	void CollidingCallback(CGameNode *collided);
	void CollidedCallback(CGameNode *colliding);
};

#endif
