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

	LPD3DXMESH m_pMesh; //Point to mesh
	D3DMATERIAL9 m_pMeshMaterials; //Point to mesh material


	LPDIRECT3DTEXTURE9 pSnowmanTexture0; //First texture
	LPDIRECT3DTEXTURE9 pSnowmanTexture1; //Second texture
	LPDIRECT3DVERTEXBUFFER9 pSnowmanVertexBuffer; //D3D vertext buffer 
	
	D3DMATERIAL9 *pSnowmanMeshMaterials; //Point to snowman mesh material
	unsigned long snowManNumMaterials; //Number of material

public:
	CPlatform(float width, float height, float depth) : 
		m_width(width), m_height(height), m_depth(depth) {m_type = PLATFORM;};
	void Update();
	bool InitVertices();
	bool InitColliders();
	
	void CollidingCallback(CGameNode *collided);
	void CollidedCallback(CGameNode *colliding);

	float GetHeight(){return m_height;};
};

#endif

