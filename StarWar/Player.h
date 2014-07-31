#ifndef _STARWAR_PLAYER_H_
#define _STARWAR_PLAYER_H_

#include "GameNode.h"

class CPlayer : public CGameNode
{
protected:
	LPDIRECT3DTEXTURE9 pSnowmanTexture0; //First texture
	LPDIRECT3DTEXTURE9 pSnowmanTexture1; //Second texture
	LPDIRECT3DVERTEXBUFFER9 pSnowmanVertexBuffer; //D3D vertext buffer 
	LPD3DXMESH pSnowmanMesh; //Point to snowman mesh
	D3DMATERIAL9 *pSnowmanMeshMaterials; //Point to snowman mesh material
	unsigned long snowManNumMaterials; //Number of material

public:
	CPlayer();
	void Update();
	bool InitVertices();
	bool InitColliders();
	void CollidingCallback(CGameNode *collided);
	void CollidedCallback(CGameNode *colliding);
};

#endif

