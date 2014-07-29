#ifndef _SNOWMAN_TERRAIN_H_
#define _SNOWMAN_TERRAIN_H_

#include "Util.h"

//Define TerrainVertex
typedef PositionNormalSpecularColorTexVertex TerrainVertex;

//------------------------------------------------------------------------------
// Class Name: CTerrain
// Desc: The class of terrain
//------------------------------------------------------------------------------

class CTerrain
{
public:
	//Constructor.
	CTerrain(int sizeX, int sizeZ, int xBase, int zBase, int heightLimit, int heightBase, 
		float rate, const LPCSTR pHeightRawFileName, const LPCSTR pTexFileName, LPDIRECT3DDEVICE9 g_pd3dDevice);
	//Deconstructor.
	~CTerrain();
	//Draw the terrain.
	void Draw();	
	//Get the map constraint.
	void GetSpaceConstraint(float &x1, float &x2, float &z1, float &z2);
private:	
	//Release resource.
	void Release();
	//Create vertex buffer.
	void InitVertices();
	//Create indices buffer with corresponding vertex id.
	void InitIndices();
	//Calculate the normals of all vertices.
	void InitNormal();
	//Read height map from file.
	bool LoadHeightRaw(const LPCSTR pHeightRawFileName);
	//Read texture from file.
	bool LoadTexture(const LPCSTR pTexFileName);
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
	//Point to the texture.
	LPDIRECT3DTEXTURE9 m_pTexture;
	//Point to the global D3D device.
	LPDIRECT3DDEVICE9 m_pd3dDevice;
private:
	//Get the raw height data
	inline float GetHeightData(int x, int z) {return (float)m_pHeightData[z * m_sizeX + x];}
};

#endif