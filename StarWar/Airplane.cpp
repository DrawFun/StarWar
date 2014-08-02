#include "Airplane.h"

CAirplane::CAirplane()
{
	m_type = AIRPLANE;
	m_enableControl = false;
	m_enablePhysics = true;
	m_enableRender = true;
	m_isControlable = true; 
	m_isFlyable = true;	
}

void CAirplane::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if(m_enableRender)
	{
		pd3dDevice->SetTransform(D3DTS_WORLD, &m_transform.GetWorldMatrix());
		for( unsigned long i = 0; i < m_materialsNum; ++i )
		{
			pd3dDevice->SetMaterial(&m_pMeshMaterials[i]);
			m_pMesh->DrawSubset(i);
		}
	}
}

void CAirplane::Update()
{
	
}
	
bool CAirplane::InitVertices()
{
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();	

	//Init snowman x model
	LPD3DXBUFFER pD3DXMtrlBuffer;

    D3DXLoadMeshFromX( "Resource//Jet.x", D3DXMESH_SYSTEMMEM, 
                       pd3dDevice, NULL, 
					   &pD3DXMtrlBuffer, NULL, &m_materialsNum, 
					   &m_pMesh );

    D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	m_pMeshMaterials = new D3DMATERIAL9[m_materialsNum];
    for( unsigned long i = 0; i < m_materialsNum; ++i )
    {
        //Set materials
        m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
        //Set the ambient color for the material
        m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;
    }

    pD3DXMtrlBuffer->Release();
	return true;
}

bool CAirplane::InitColliders()
{
	D3DXVECTOR3 pMin, pMax;
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	m_pMesh->GetDeclaration( decl );
	LPVOID pVB;
	m_pMesh->LockVertexBuffer( D3DLOCK_READONLY, &pVB );
	UINT uStride = D3DXGetDeclVertexSize( decl, 0 );
	D3DXComputeBoundingBox(( const D3DXVECTOR3* )pVB, m_pMesh->GetNumVertices(), uStride, &pMin, &pMax);		
	Collider col(pMin, pMax);
	m_colliders.push_back(col);
	return true;
}

void CAirplane::CollidingCallback(CGameNode *collided)
{
	//AllocConsole();
	//_cprintf("%d->%d\n", this->m_type, collided->GetType());
}

void CAirplane::CollidedCallback(CGameNode *colliding)
{
	//AllocConsole();
	//_cprintf("%d<-%d\n", this->m_type, colliding->GetType());
	switch(colliding->GetType())
	{
	case PLATFORM:		
		break;
	case HUMAN:		
		break;
	case ZERG:
		break;
	case ZERG_TELEPORT:
	case MINE:
		break;
	default:
		break;
	}
}