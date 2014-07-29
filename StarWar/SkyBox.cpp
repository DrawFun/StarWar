#include "SkyBox.h"

//------------------------------------------------------------------------------
// Name: CSkyBox::CSkyBox(const LPCSTR (&pImageFileNameArray)[TOTAL_SIDES], LPDIRECT3DDEVICE9 g_pd3dDevice)
// Desc: Skybox constructor. 
//------------------------------------------------------------------------------
CSkyBox::CSkyBox(const LPCSTR (&pImageFileNameArray)[TOTAL_SIDES], LPDIRECT3DDEVICE9 g_pd3dDevice)
	: m_pd3dDevice(g_pd3dDevice)
{	
	for(int i = 0; i < TOTAL_SIDES; ++i)
	{
		m_pTextureArray[i] = NULL;
	}
	m_pVertexArray = NULL;
	m_pVertexBuffer = NULL;

	//Init vertices then load texture from files.
	if(InitVertices() && LoadTexture(pImageFileNameArray))
	{
		return;
	}
	else
	{
		//Fail to init. Release resource and quit.
		Release();
		exit(1);
	}
}

//------------------------------------------------------------------------------
// Name: CSkyBox::Release()
// Desc: Release resource. 
//------------------------------------------------------------------------------
void CSkyBox::Release()
{
	for(int i = 0; i < TOTAL_SIDES; ++i)
	{
		if(m_pTextureArray[i] != NULL)
		{
			m_pTextureArray[i]->Release();
		}
		else
		{
			continue;
		}
	}	
	if(m_pVertexArray != NULL)
	{
		free(m_pVertexArray);
	}
	if(m_pVertexBuffer != NULL)
	{
		m_pVertexBuffer->Release();
	}
}

//------------------------------------------------------------------------------
// Name: CSkyBox::~CSkyBox()
// Desc: Deconstructor. 
//------------------------------------------------------------------------------
CSkyBox::~CSkyBox()
{
	Release();
}

//------------------------------------------------------------------------------
// Name: CSkyBox::InitVertices(void)
// Desc: Create vertex buffer. 
//------------------------------------------------------------------------------
bool CSkyBox::InitVertices(void)
{
	//Allocate memory for vertices constructing skybox
	m_pVertexArray = (SkyBoxVertex*) malloc(sizeof(SkyBoxVertex) * TOTAL_VERTICES);
	
	//Init side by side
	//Four vertices form into a side.
	//Init the position, normal, color and texture position.

	//Front
	m_pVertexArray[0] = SkyBoxVertex(-30.0f, -30.0f, 30.0f, 0, 0, -1, 0xffffff00, 0.0f, 1.0f);
	m_pVertexArray[1] = SkyBoxVertex(-30.0f,  30.0f, 30.0f, 0, 0, -1, 0xffffff00, 0.0f, 0.0f);
	m_pVertexArray[2] = SkyBoxVertex( 30.0f, -30.0f, 30.0f, 0, 0, -1, 0xffffff00, 1.0f, 1.0f);
	m_pVertexArray[3] = SkyBoxVertex( 30.0f,  30.0f, 30.0f, 0, 0, -1, 0xffffff00, 1.0f, 0.0f);

	//Back
	m_pVertexArray[4] = SkyBoxVertex( 30.0f, -30.0f, -30.0f, 0, 0, 1, 0xffffff00, 0.0f, 1.0f);
	m_pVertexArray[5] = SkyBoxVertex( 30.0f,  30.0f, -30.0f, 0, 0, 1, 0xffffff00, 0.0f, 0.0f);
	m_pVertexArray[6] = SkyBoxVertex(-30.0f, -30.0f, -30.0f, 0, 0, 1, 0xffffff00, 1.0f, 1.0f);
	m_pVertexArray[7] = SkyBoxVertex(-30.0f,  30.0f, -30.0f, 0, 0, 1, 0xffffff00, 1.0f, 0.0f);

	//Left
	m_pVertexArray[8]  = SkyBoxVertex(-30.0f, -30.0f, -30.0f, 1, 0, 0, 0xffffff00, 0.0f, 1.0f);
	m_pVertexArray[9]  = SkyBoxVertex(-30.0f,  30.0f, -30.0f, 1, 0, 0, 0xffffff00, 0.0f, 0.0f);
	m_pVertexArray[10] = SkyBoxVertex(-30.0f, -30.0f,  30.0f, 1, 0, 0, 0xffffff00, 1.0f, 1.0f);
	m_pVertexArray[11] = SkyBoxVertex(-30.0f,  30.0f,  30.0f, 1, 0, 0, 0xffffff00, 1.0f, 0.0f);

	//Right
	m_pVertexArray[12] = SkyBoxVertex( 30.0f, -30.0f,  30.0f, -1, 0, 0, 0xffffff00, 0.0f, 1.0f);
	m_pVertexArray[13] = SkyBoxVertex( 30.0f,  30.0f,  30.0f, -1, 0, 0, 0xffffff00, 0.0f, 0.0f);
	m_pVertexArray[14] = SkyBoxVertex( 30.0f, -30.0f, -30.0f, -1, 0, 0, 0xffffff00, 1.0f, 1.0f);
	m_pVertexArray[15] = SkyBoxVertex( 30.0f,  30.0f, -30.0f, -1, 0, 0, 0xffffff00, 1.0f, 0.0f);

	//Ceiling
	m_pVertexArray[16] = SkyBoxVertex(-30.0f,  30.0f,  30.0f, 0, -1, 0, 0xffffff00, 0.0f, 1.0f);
	m_pVertexArray[17] = SkyBoxVertex(-30.0f,  30.0f, -30.0f, 0, -1, 0, 0xffffff00, 0.0f, 0.0f);
	m_pVertexArray[18] = SkyBoxVertex( 30.0f,  30.0f,  30.0f, 0, -1, 0, 0xffffff00, 1.0f, 1.0f);
	m_pVertexArray[19] = SkyBoxVertex( 30.0f,  30.0f, -30.0f, 0, -1, 0, 0xffffff00, 1.0f, 0.0f);

	//Floor
	m_pVertexArray[20] = SkyBoxVertex(-30.0f, -30.0f, -30.0f, 0, 1, 0, 0xffffff00, 0.0f, 1.0f);
	m_pVertexArray[21] = SkyBoxVertex(-30.0f, -30.0f,  30.0f, 0, 1, 0, 0xffffff00, 0.0f, 0.0f);
	m_pVertexArray[22] = SkyBoxVertex( 30.0f, -30.0f, -30.0f, 0, 1, 0, 0xffffff00, 1.0f, 1.0f);
	m_pVertexArray[23] = SkyBoxVertex( 30.0f, -30.0f,  30.0f, 0, 1, 0, 0xffffff00, 1.0f, 0.0f);

	// Create corresponding vertex buffer in D3D. 
    HRESULT hRet = m_pd3dDevice->CreateVertexBuffer(sizeof(SkyBoxVertex) * TOTAL_VERTICES, //Buffer size
											 0, //Hardware vertex processing
											 SkyBoxVertex::FVF,	//Vertex FVF type
											 D3DPOOL_MANAGED, //Memory pool is management type
											 &m_pVertexBuffer, //Target vertex buffer
											 NULL); //Reserved
	//Check creating results
    if (FAILED(hRet))
	{
		::MessageBox(NULL, "Failed to create the vertex buffer!", "Error in BuildSkybox()", MB_OK | MB_ICONSTOP);
		return false;
	}


	//Copy the initialized skybox mesh into the vertex buffer.	
	void *pVertices = NULL;
	m_pVertexBuffer->Lock(0, sizeof(SkyBoxVertex) * TOTAL_VERTICES, (void**)&pVertices, 0);
    memcpy(pVertices, m_pVertexArray, sizeof(SkyBoxVertex) * TOTAL_VERTICES);
    m_pVertexBuffer->Unlock();
	return true;
}

//------------------------------------------------------------------------------
// Name: CSkyBox::LoadTexture(const LPCSTR *pImageFileNameArray)
// Desc: Load skybox texture from file array.
//------------------------------------------------------------------------------
bool CSkyBox::LoadTexture(const LPCSTR *pImageFileNameArray)
{
	//Check whether pointer to D3D device is available
	if(NULL == m_pd3dDevice)
	{
		::MessageBox(NULL, "Null global D3D device pointer!", "Error During: LoadTexture", MB_OK | MB_ICONSTOP);
		return false;
	}

	//Use HRESULT as the typical return value of D3DXCreateTextureFromFile is HRESULT
	HRESULT hRet;

	//Load textures from files. 
	hRet = D3DXCreateTextureFromFile(m_pd3dDevice, pImageFileNameArray[FRONT], &m_pTextureArray[FRONT]);
	hRet |= D3DXCreateTextureFromFile(m_pd3dDevice, pImageFileNameArray[BACK], &m_pTextureArray[BACK]);
	hRet |= D3DXCreateTextureFromFile(m_pd3dDevice, pImageFileNameArray[LEFT], &m_pTextureArray[LEFT]);
	hRet |= D3DXCreateTextureFromFile(m_pd3dDevice, pImageFileNameArray[RIGHT], &m_pTextureArray[RIGHT]);
	hRet |= D3DXCreateTextureFromFile(m_pd3dDevice, pImageFileNameArray[CEILING], &m_pTextureArray[CEILING]);
	hRet |= D3DXCreateTextureFromFile(m_pd3dDevice, pImageFileNameArray[FLOOR], &m_pTextureArray[FLOOR]);

	//Check loading results
	if(FAILED(hRet))
	{	
		::MessageBox(NULL, "Failed to load sky box textures from files!", "Error During LoadTexture", MB_OK | MB_ICONSTOP);
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------
// Name: CSkyBox::Draw(void)
// Desc: Draw skybox and put it far away enough.
//------------------------------------------------------------------------------
void CSkyBox::Draw(D3DXVECTOR3 &vEye)
{
	//Check whether pointer to D3D device is available
	if(NULL == m_pd3dDevice)
	{
		::MessageBox(NULL, "Null global D3D device pointer!", "Error During: Draw", MB_OK | MB_ICONSTOP);
		return;
	}
	
	//Backup original world matrix
	D3DXMATRIX oriWorldTransMatrix;	
	m_pd3dDevice->GetTransform(D3DTS_WORLD, &oriWorldTransMatrix);

	//Use to translation skybox. Always far away enough.
    D3DXMATRIX skyBoxWorldTransMatrix;    
	//Construct translation matrix. Use current camera position as center to construct.
    D3DXMatrixTranslation(&skyBoxWorldTransMatrix,vEye.x,vEye.y,vEye.z);
    //Transform.
    m_pd3dDevice->SetTransform(D3DTS_WORLD, &skyBoxWorldTransMatrix);

	//Disable z-depth buffer.
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	//Set SkyBoxVertex FVF type.
	m_pd3dDevice->SetFVF( SkyBoxVertex::FVF );
	//Binding the vertex buffer to D3D device data stream
    m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SkyBoxVertex));

	//Draw skybox side by side
	for (int i = 0; i < TOTAL_SIDES; ++i)
    {
		//Set the textures to be used
        m_pd3dDevice->SetTexture( 0, m_pTextureArray[i] );

		//Draw skybox
        m_pd3dDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP, //Primitive type
			i * VERTICES_PER_SIDE, //One side contains 4 vertices
			2); //One side contains 2 triangles
 
    }
	//Enable z-depth buffer.
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	//Recover original world matrix
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &oriWorldTransMatrix);
}