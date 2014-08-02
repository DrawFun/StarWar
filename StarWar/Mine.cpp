#include "Mine.h"

void CMine::Update()
{
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();
	pd3dDevice->SetTransform(D3DTS_WORLD, &this->GetTransform()->GetWorldMatrix());	
	m_transform.Yaw(m_rotateSpeed);
	//m_transform.UpdateMatrix();
	
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
	
bool CMine::InitVertices()
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
	//TODO: NULL POINT FAIL
	//LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();

	//Create textures from files
	D3DXCreateTextureFromFile( pd3dDevice, "Resource//Snowman1.jpg", &pSnowmanTexture0);
	D3DXCreateTextureFromFile( pd3dDevice, "Resource//Snowman2.jpg", &pSnowmanTexture1);
		
	return true;	
}
	
bool CMine::InitColliders()
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


void CMine::CollidingCallback(CGameNode *collided)
{
	//AllocConsole();
	//_cprintf("%d->%d\n", this->m_type, collided->GetType());
}
	

void CMine::CollidedCallback(CGameNode *colliding)
{
	//AllocConsole();
	//_cprintf("%d<-%d\n", this->m_type, colliding->GetType());
	switch(colliding->GetType())
	{
	case MINE:		
		break;
	case HUMAN:
		break;
	case ZERG_TELEPORT:
	case ZERG:
		break;
	case PLATFORM:
		break;
	default:
		break;
	}
}