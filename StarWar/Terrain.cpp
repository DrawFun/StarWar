#include "Terrain.h"
//Use to read height map
#include <fstream>

//------------------------------------------------------------------------------
// Name: CTerrain::CTerrain(int sizeX, int sizeZ, int xBase, int zBase, int heightLimit, int heightBase, 
//			float rate, const LPCSTR pHeightRawFileName, const LPCSTR pTexFileName, LPDIRECT3DDEVICE9 g_pd3dDevice)
// Desc: Terrain constructor. 
//------------------------------------------------------------------------------
CTerrain::CTerrain(int sizeX, int sizeZ, int xBase, int zBase, int heightLimit, int heightBase, 
	float rate) : 
	m_sizeX(sizeX), m_sizeZ(sizeZ), m_xBase(xBase), m_zBase(zBase), m_heightLimit(heightLimit), 
	m_heightBase(heightBase), m_rate(rate)
{
	m_type = TERRAIN; 	
	m_enableControl = false;
	m_enablePhysics = false;
	m_enableRender = true;
	m_isControlable = false; 
	m_isFlyable = false;
}

//------------------------------------------------------------------------------
// Name: CTerrain::~CTerrain()
// Desc: Deconstructor.
//------------------------------------------------------------------------------
CTerrain::~CTerrain()
{
	if(m_pHeightData != NULL)
	{
		free(m_pHeightData);
	}
	if(m_pTexture != NULL)
	{
		m_pTexture->Release();
	}
	if(m_pVertexArray != NULL)
	{
		free(m_pVertexArray);
	}
	if(m_pIndices != NULL)
	{
		free(m_pIndices);
	}
}

//------------------------------------------------------------------------------
// Name: CTerrain::LoadHeightRaw(const LPCSTR pHeightRawFileName)
// Desc: Load the raw height data from file.
//------------------------------------------------------------------------------
bool CTerrain::LoadHeightRaw(const LPCSTR pHeightRawFileName, LPDIRECT3DDEVICE9 pd3dDevice)
{
	std::ifstream inFile(pHeightRawFileName, std::ios::in | std::ios_base::binary);
	//Read data or report error. 
	if(inFile != NULL)
	{
		//Init raw height array.
		m_pHeightData = (char*)malloc(m_sizeX * m_sizeZ * sizeof(char));
		memset(m_pHeightData, 0, m_sizeX * m_sizeZ * sizeof(char));
		//Read data from file.
		inFile.read(m_pHeightData, m_sizeX * m_sizeZ);
		inFile.close();
		//Re-generate raw data according to height limit and base offset.
		for(int i = 0; i < m_sizeX * m_sizeZ; ++i)
		{

			m_pHeightData[i] %= m_heightLimit;
			m_pHeightData[i] += m_heightBase;
		}
		return true;
	}
	else
	{
		//Report error and return false.
		::MessageBox(NULL, "Invalide file name!", "Error During: CTerrain::LoadHeightRaw", MB_OK | MB_ICONSTOP);
		return false;
	}
}

//------------------------------------------------------------------------------
// Name: CTerrain::LoadTexture(const LPCSTR pTexFileName)
// Desc: Load terrain texture from file.
//------------------------------------------------------------------------------
bool CTerrain::LoadTexture(const LPCSTR pTexFileName, LPDIRECT3DDEVICE9 pd3dDevice)
{
	//Check whether pointer to D3D device is available
	if(NULL == pd3dDevice)
	{
		::MessageBox(NULL, "Null global D3D device pointer!", "Error During: CTerrain::LoadTexture", MB_OK | MB_ICONSTOP);
		return false;
	}

	//Use HRESULT as the typical return value of D3DXCreateTextureFromFile is HRESULT
	HRESULT hRet;

	//Load textures from files. 
	hRet = D3DXCreateTextureFromFile(pd3dDevice, pTexFileName, &m_pTexture);

	//Check loading result
	if(FAILED(hRet))
	{	
		//Report error and return false.
		::MessageBox(NULL, "Failed to load terrain textures from files!", "Error During: CTerrain::LoadTexture", MB_OK | MB_ICONSTOP);
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------
// Name: CTerrain::InitVertices(void)
// Desc: Create vertex buffer. Init vertex buffer accoring to the raw height data.
//------------------------------------------------------------------------------
bool CTerrain::InitVertices(void)
{
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();

	const LPCSTR pHeightRawFileName = "Resource//Heighmap.raw";
	const LPCSTR pTexFileName = "Resource//SnowTerrain.jpg";
	
	m_pHeightData = NULL;
	m_pVertexArray = NULL;
	m_pIndices = NULL;
	m_pTexture = NULL;	

	//Try to read height map and texture files. 
	if(LoadHeightRaw(pHeightRawFileName, pd3dDevice) && LoadTexture(pTexFileName, pd3dDevice))
	{
		//Create vertex buffer 
		m_pVertexArray = (TerrainVertex*)malloc(m_sizeX * m_sizeZ * sizeof(TerrainVertex));	

		//Init one by one
		for(int i = 0, z = 0; z < m_sizeZ; ++z)
		{
			for(int x = 0; x < m_sizeX; ++x)
			{
				//Get raw height data in index position(x, z)
				float y = GetHeightData(x,z);
				//Init TerrainVertex
				m_pVertexArray[i++] = TerrainVertex
					(m_xBase + x * m_rate, y, m_zBase + z * m_rate, //Init world position according to zoom rate and base offset
					0, 0, 0, //Leave normal initialization later
					0xffffff00, //White as it is snow
					(float)x, (float)z); //Texture position
			}
		}
		InitIndices();
		//Init normal of each vertex in terrain for specular light effect. 		
		InitNormal();
		ZeroMemory(&m_pMeshMaterials, sizeof(D3DMATERIAL9));
		m_pMeshMaterials.Diffuse.r = 1;
		m_pMeshMaterials.Diffuse.g = 1;
		m_pMeshMaterials.Diffuse.b = 1;
		m_pMeshMaterials.Diffuse.a = 1;
		m_pMeshMaterials.Ambient.r = 1;
		m_pMeshMaterials.Ambient.g = 1;
		m_pMeshMaterials.Ambient.b = 1;
		m_pMeshMaterials.Ambient.a = 1;
		return true;
	}
	else
	{
		return false;
	}

}

//------------------------------------------------------------------------------
// Name: CTerrain::InitIndices(void)
// Desc: Create indices buffer. Init inde buffer with corresponding vertex id.
//------------------------------------------------------------------------------
void CTerrain::InitIndices(void)
{
	//First, calculate total indices number then allocate memory.
	/*
	  *---*---*
      | \ | \ |
      *---*---*  ... ... (Height = H, Width = W)
      | \ | \ |
      *---*---*
	*/
	// The whole rectangle area is H * W, meaning H * W unit rectangle.
	// Each unit rectangle contains two triangles. 
	// Each triangle contains three indices.
	// Further, H = m_sizeZ - 1, W = m_sizeX - 1.
	// Hence, Total indices = H * W * 2 * 3 = (m_sizeX - 1) * (m_sizeZ - 1) * 2 * 3.	
	m_pIndices = (int*)malloc((m_sizeX - 1) * (m_sizeZ - 1) * 2 * 3 * sizeof(int));
	
	//Then, init one by one.
	/*
      1---3
      | \ | ... ... 
      0---2
	*/
	//First triangle
		//0 : current index, position(x, z), vertex id = z * m_sizeX + x.
		//1 : next index in z-direction, position(x, z + 1), vertex id = (z + 1) * m_sizeX + x.
		//2 : next index in x-direction, position(x + 1, z), vertex id = z * m_sizeX + x + 1.
	//Second triangle
		//1 : position(x, z + 1), vertex id = (z + 1) * m_sizeX + x.
		//3 : position(x + 1, z + 1), vertex id = (z + 1) * m_sizeX + x + 1.
		//2 : position(x + 1, z), vertex id = z * m_sizeX + x + 1.

	for(int i = 0, z = 0; z < m_sizeZ - 1; ++z)
	{
		for(int x = 0; x < m_sizeX - 1; ++x)
		{
			m_pIndices[i++] = z * m_sizeX + x;
			m_pIndices[i++] = (z + 1) * m_sizeX + x;
			m_pIndices[i++] = z * m_sizeX + x + 1;

			m_pIndices[i++] = (z + 1) * m_sizeX + x;
			m_pIndices[i++] = (z + 1) * m_sizeX + x + 1;
			m_pIndices[i++] = z * m_sizeX + x + 1;
		}
	}
}

//------------------------------------------------------------------------------
// Name: CTerrain::InitNormal(void)
// Desc: Calculate the normals of all vertices.
//------------------------------------------------------------------------------
void CTerrain::InitNormal(void)
{
	//Calculate normal for each vertex
	for(int i = 0; i < (m_sizeX - 1) * (m_sizeZ - 1) * 2; ++i)
	{
		//Calculate the cross vector of the next triangle
		//First side of the next triangle
		D3DXVECTOR3 vec1 = D3DXVECTOR3(
			m_pVertexArray[m_pIndices[i * 3 + 1]]._x - m_pVertexArray[m_pIndices[i * 3]]._x, 
			m_pVertexArray[m_pIndices[i * 3 + 1]]._y - m_pVertexArray[m_pIndices[i * 3]]._y,
			m_pVertexArray[m_pIndices[i * 3 + 1]]._z - m_pVertexArray[m_pIndices[i * 3]]._z);
		//Second side of the next triangle
		D3DXVECTOR3 vec2 = D3DXVECTOR3(
			m_pVertexArray[m_pIndices[i * 3 + 2]]._x - m_pVertexArray[m_pIndices[i * 3]]._x, 
			m_pVertexArray[m_pIndices[i * 3 + 2]]._y - m_pVertexArray[m_pIndices[i * 3]]._y,
			m_pVertexArray[m_pIndices[i * 3 + 2]]._z - m_pVertexArray[m_pIndices[i * 3]]._z);
		//Calculate cross vector
		D3DXVECTOR3 normal;
		D3DXVec3Cross(&normal, &vec1, &vec2);
		//Normalize cross vector
		D3DXVec3Normalize(&normal,&normal);

		//Add normalized cross vector to related three vectices
		m_pVertexArray[m_pIndices[i * 3]]._nx += normal.x;
		m_pVertexArray[m_pIndices[i * 3]]._ny += normal.y;
		m_pVertexArray[m_pIndices[i * 3]]._nz += normal.z;

		m_pVertexArray[m_pIndices[i * 3 + 1]]._nx += normal.x;
		m_pVertexArray[m_pIndices[i * 3 + 1]]._ny += normal.y;
		m_pVertexArray[m_pIndices[i * 3 + 1]]._nz += normal.z;

		m_pVertexArray[m_pIndices[i * 3 + 2]]._nx += normal.x;
		m_pVertexArray[m_pIndices[i * 3 + 2]]._ny += normal.y;
		m_pVertexArray[m_pIndices[i * 3 + 2]]._nz += normal.z;
	}

	//Re-normalize 
	for(int i = 0; i < m_sizeX * m_sizeZ; ++i)
	{		
		D3DXVECTOR3 normal;
		D3DXVec3Normalize(&normal, &D3DXVECTOR3(m_pVertexArray[i]._nx, m_pVertexArray[i]._ny, m_pVertexArray[i]._nz));
		m_pVertexArray[i]._nx = normal.x;
		m_pVertexArray[i]._ny = normal.y;
		m_pVertexArray[i]._nz = normal.z;
	}
}

//------------------------------------------------------------------------------
// Name: CTerrain::Draw(void)
// Desc: Draw terrain.
//------------------------------------------------------------------------------
void CTerrain::Update(void)
{
	m_transform.UpdateMatrix();
}

void CTerrain::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if(m_enableRender)
	{
		pd3dDevice->SetTransform(D3DTS_WORLD, &m_transform.GetWorldMatrix());
		//Set the textures to be used
		pd3dDevice->SetTexture(0,m_pTexture);
		//Set the vector FVF format
		pd3dDevice->SetFVF(TerrainVertex::FVF);
		pd3dDevice->SetMaterial(&m_pMeshMaterials);
		//Draw terrain
		pd3dDevice->DrawIndexedPrimitiveUP
			(D3DPT_TRIANGLELIST,  //Primitive type
			0, //Starting vertex id
			m_sizeX * m_sizeZ, //Vertex number
			(m_sizeX - 1) * (m_sizeZ - 1) * 2, //Primitive number
			&m_pIndices[0], //Primitive starting address
			D3DFMT_INDEX32,  //Index data format
			&m_pVertexArray[0], //Vertex starting address
			sizeof(TerrainVertex)); //Vertex size
	}
}