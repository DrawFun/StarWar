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


	LPDIRECT3DTEXTURE9 pSnowmanTexture0; //First texture
	LPDIRECT3DTEXTURE9 pSnowmanTexture1; //Second texture
	LPDIRECT3DVERTEXBUFFER9 pSnowmanVertexBuffer; //D3D vertext buffer 
	
	D3DMATERIAL9 *pSnowmanMeshMaterials; //Point to snowman mesh material
	unsigned long snowManNumMaterials; //Number of material

public:
	CPlatform(float width, float height, float depth, float horizonlSpeed, float rotationSpeed) : 
		m_width(width), m_height(height), m_depth(depth), m_horizontalSpeed(horizonlSpeed), m_rotationSpeed(rotationSpeed) {m_type = PLATFORM; m_moveCounter = 0;};
	void Update();
	bool InitVertices();
	bool InitColliders();
	
	void CollidingCallback(CGameNode *collided);
	void CollidedCallback(CGameNode *colliding);

	float GetHeight(){return m_height;};
	void Move();
};

#endif

