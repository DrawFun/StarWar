#include "Airplane.h"

CAirplane::CAirplane()
{
	m_type = AIRPLANE;
	m_enableControl = false;
	m_enablePhysics = true;
	m_enableRender = true;
	m_isControlable = true; 
	m_isFlyable = true;	
	m_moveSpeed = 20.0f;
	m_isFlying = false;
}

CAirplane::~CAirplane()
{
	delete m_pMesh;
	if( m_pMeshMaterials != NULL )
        delete[] m_pMeshMaterials;

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
	if(m_enableControl && m_transform.GetWorldPosition().y > 1)
	{
		m_isFlying = true;
	}

	if(m_isFlying && m_transform.GetWorldPosition().y < 1)
	{
		m_scene->EventCallBack(STARWAR_OUT_AIRPLANE, this);
		m_isFlying = false;
	}
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
	D3DXVECTOR3 pCenter;
	float pRadius;
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	m_pMesh->GetDeclaration(decl);
	LPVOID pVB;
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, &pVB);
	UINT uStride = D3DXGetDeclVertexSize(decl, 0);	
	D3DXComputeBoundingSphere((const D3DXVECTOR3* )pVB, m_pMesh->GetNumVertices(), uStride, &pCenter, &pRadius);
	//TODO:目前假设球体三个方向scale值相等
	pRadius *= m_transform.GetScale().x;
	Collider collider(pCenter, pRadius);
	m_colliders.push_back(collider);
	return true;
}

void CAirplane::CollidingCallback(CGameNode *collided)
{
	switch(collided->GetType())
	{
	case PLATFORM:	
		if(m_isFlying)
		{		
			collided->GetTransform()->AddChild(&m_transform);
		}
		break;
	default:
		break;
	}

}

void CAirplane::CollidedCallback(CGameNode *colliding)
{
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