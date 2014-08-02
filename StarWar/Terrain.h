#ifndef _STARWAR_TERRAIN_H_
#define _STARWAR_TERRAIN_H_

#include "GameNode.h"

//Define TerrainVertex
typedef PositionNormalSpecularColorTexVertex TerrainVertex;

//------------------------------------------------------------------------------
// Class Name: CTerrain
// Desc: The class of terrain
//------------------------------------------------------------------------------

class CTerrain : public CGameNode
{
private:
	//Number of vertex in x-axis. Use as length of terrain. 
	int m_sizeX;
	//Number of vertex in z-axis. Use as width of terrain. 
	int m_sizeZ;
	//Base coordinate offset in x-axis. Adjust x-coordinate to make the terrain cover the field of view. 
	int m_xBase;
	//Base coordinate offset in z-axis. Adjust z-coordinate to make the terrain cover the field of view. 
	int m_zBase;
	//The maximum generated height. 
	int m_heightLimit;
	//Base height offset in y-axis.
	int m_heightBase;
	//Zoom in/out rate. Adjust it to make the terrain cover the field of view. 
	int m_rate;
	//Storage for raw height data.
	char *m_pHeightData;  
	//Storage buffer for vertices.
	TerrainVertex *m_pVertexArray;
	//Storage buffer for indices.
	int *m_pIndices;
	//Point to mesh material
	D3DMATERIAL9 m_pMeshMaterials; 
	//Point to the texture.
	LPDIRECT3DTEXTURE9 m_pTexture;

public:
	//Constructor.
	CTerrain(int sizeX, int sizeZ, int xBase, int zBase, int heightLimit, int heightBase, 
		float rate);
	//Deconstructor.
	~CTerrain();
	//Update the terrain.
	void Update();	
	void Render(LPDIRECT3DDEVICE9 pd3dDevice);
	//Create vertex buffer.
	bool InitVertices();
	bool InitColliders(){return true;};
	void CollidingCallback(CGameNode *collided){};
	void CollidedCallback(CGameNode *colliding){};

private:
	//Create indices buffer with corresponding vertex id.
	void InitIndices();
	//Calculate the normals of all vertices.
	void InitNormal();
	//Read height map from file.
	bool LoadHeightRaw(const LPCSTR pHeightRawFileName, LPDIRECT3DDEVICE9 pd3dDevice);
	//Read texture from file.
	bool LoadTexture(const LPCSTR pTexFileName, LPDIRECT3DDEVICE9 pd3dDevice);
	//Get the raw height data
	inline float GetHeightData(int x, int z) {return (float)m_pHeightData[z * m_sizeX + x];}
};

#endif