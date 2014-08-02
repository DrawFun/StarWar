#ifndef _STARWAR_AIRPLANE_H_
#define _STARWAR_AIRPLANE_H_

#include "GameNode.h"

class CAirplane : public CGameNode
{
protected:
	LPDIRECT3DTEXTURE9 m_pTexture; 
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer; //D3D vertext buffer 
	LPD3DXMESH m_pMesh; 
	D3DMATERIAL9 *m_pMeshMaterials; 
	unsigned long m_materialsNum; //Number of material

public:
	CAirplane();
	void Update();
	bool InitVertices();
	bool InitColliders();
	void Render(LPDIRECT3DDEVICE9 pd3dDevice);
	
	void CollidingCallback(CGameNode *collided);
	void CollidedCallback(CGameNode *colliding);
};

#endif
