#include "Player.h"

//Vectors for view and position

//Snowman related variables
D3DXVECTOR3	snowmanPos(10.0f, -30.0f, 10.0f); //Position
LPDIRECT3DTEXTURE9 pSnowmanTexture0 = NULL; //First texture
LPDIRECT3DTEXTURE9 pSnowmanTexture1 = NULL; //Second texture
LPDIRECT3DVERTEXBUFFER9 pSnowmanVertexBuffer = NULL; //D3D vertext buffer 
LPD3DXMESH pSnowmanMesh = NULL; //Point to snowman mesh
D3DMATERIAL9 *pSnowmanMeshMaterials = NULL; //Point to snowman mesh material
unsigned long snowManNumMaterials = 0L; //Number of material
D3DXMATRIX snowmanRX, snowmanRY, snowmanRZ;
float rx, ry, rz;

CPlayer::CPlayer(HWND hWnd)
{
	m_hWnd = hWnd;
	m_position = D3DXVECTOR3(0.0f, -25.0f, -10.0f);
	m_moveSpeed = 20;
}

void CPlayer::Update()
{
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();
	
	UpdateMatrix();

	//Set texture on stage 0
	pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//Set RGB mix method
	pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	
	//Set texture on stage 1
	pd3dDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//Set RGB mix method
	pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
	pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);

    for( unsigned long i = 0; i < snowManNumMaterials; ++i )
    {
        pd3dDevice->SetMaterial(&pSnowmanMeshMaterials[i]);
		
		//Set multiple textures
		pd3dDevice->SetTexture(0, pSnowmanTexture0);
		pd3dDevice->SetTexture(1, pSnowmanTexture1);
        pSnowmanMesh->DrawSubset(i);
    }
}

bool CPlayer::InitVertices()
{
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();

	//Init snowman x model
	LPD3DXBUFFER pD3DXMtrlBuffer;

    D3DXLoadMeshFromX( "Resource//Snowman.x", D3DXMESH_SYSTEMMEM, 
                       pd3dDevice, NULL, 
                       &pD3DXMtrlBuffer, NULL, &snowManNumMaterials, 
                       &pSnowmanMesh );

    D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    pSnowmanMeshMaterials = new D3DMATERIAL9[snowManNumMaterials];
    for( unsigned long i = 0; i < snowManNumMaterials; ++i )
    {
        //Set materials
        pSnowmanMeshMaterials[i] = d3dxMaterials[i].MatD3D;
        //Set the ambient color for the material
        pSnowmanMeshMaterials[i].Ambient = pSnowmanMeshMaterials[i].Diffuse;
    }

    pD3DXMtrlBuffer->Release();

	//Create textures from files
	D3DXCreateTextureFromFile( pd3dDevice, "Resource//Snowman1.jpg", &pSnowmanTexture0);
	D3DXCreateTextureFromFile( pd3dDevice, "Resource//Snowman2.jpg", &pSnowmanTexture1);
	
	return true;
}

bool CPlayer::InitColliders()
{
	D3DXVECTOR3 pMin, pMax;
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	pSnowmanMesh->GetDeclaration( decl );
	LPVOID pVB;
	pSnowmanMesh->LockVertexBuffer( D3DLOCK_READONLY, &pVB );
	UINT uStride = D3DXGetDeclVertexSize( decl, 0 );
	D3DXComputeBoundingBox(( const D3DXVECTOR3* )pVB, pSnowmanMesh->GetNumVertices(), uStride, &pMin, &pMax);		
	pMin *= 0.05;
	pMax *= 0.05;
	Collider col(pMin, pMax);
	m_colliders.push_back(col);
	return true;
}





