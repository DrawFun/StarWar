#include "Player.h"
#include "Platform.h"

CPlayer::CPlayer()
{	
	m_moveSpeed = 20;
	m_type = HUMAN;
}

void CPlayer::Update()
{
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();

	pd3dDevice->SetTransform(D3DTS_WORLD, &this->GetTransform()->GetWorldMatrix());		
	//m_transform.UpdateMatrix();
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


void CPlayer::CollidingCallback(CGameNode *collided)
{
	AllocConsole();
	_cprintf("%d->%d\n", this->m_type, collided->GetType());
	switch(collided->GetType())
	{
	case HUMAN:
		break;
	case ZERG:
	case ZERG_TELEPORT:
		//m_transform.Translate(D3DXVECTOR3(0, dynamic_cast<CPlatform *> (collided)->GetHeight(), 0));	
		m_transform.SetParents(collided->GetTransform());	
		collided->GetTransform()->AddChild(this->GetTransform());
		break;
	case MINE:
		break;
	case PLATFORM:
		//m_transform.Translate(D3DXVECTOR3(0, dynamic_cast<CPlatform *> (collided)->GetHeight(), 0));	
		m_transform.SetParents(collided->GetTransform());	
		collided->GetTransform()->AddChild(this->GetTransform());
		break;
	default:
		break;
	}
}
	

void CPlayer::CollidedCallback(CGameNode *colliding)
{
	AllocConsole();
	_cprintf("%d<-%d\n", this->m_type, colliding->GetType());
}