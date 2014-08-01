#include "Platform.h"

void CPlatform::Update()
{
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();
	if(m_moveCounter < PLATFORM_MOVE_FRAME_PERIOD)
	{
		m_transform.Yaw(m_rotationSpeed);
		m_transform.Translate(D3DXVECTOR3(m_horizontalSpeed, 0.0f, m_horizontalSpeed));
		++m_moveCounter;
	}
	else
	{
		m_moveCounter = 0;
		m_rotationSpeed = -m_rotationSpeed;
		m_horizontalSpeed = -m_horizontalSpeed;
	}
	m_transform.UpdateMatrix();
    pd3dDevice->SetMaterial(&m_pMeshMaterials);
	m_pMesh->DrawSubset(0);

}
	
bool CPlatform::InitVertices()
{
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();

	D3DXCreateBox(pd3dDevice, m_width, m_height, m_depth, &m_pMesh, NULL);
	ZeroMemory(&m_pMeshMaterials, sizeof(D3DMATERIAL9));
	m_pMeshMaterials.Diffuse.r = 0;
	m_pMeshMaterials.Diffuse.g = 0;
	m_pMeshMaterials.Diffuse.b = 1;
	m_pMeshMaterials.Diffuse.a = 0.5;
	m_pMeshMaterials.Ambient.r = 0;
	m_pMeshMaterials.Ambient.g = 0;
	m_pMeshMaterials.Ambient.b = 1;
	m_pMeshMaterials.Ambient.a = 0.5;
	return true;
}

bool CPlatform::InitColliders()
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

void CPlatform::CollidingCallback(CGameNode *collided)
{

}

void CPlatform::CollidedCallback(CGameNode *colliding)
{
	switch(colliding->GetType())
	{
	case PLATFORM:
		assert(0);
		break;
	case HUMAN:
	case ZERG:
		break;
	case ZERG_TELEPORT:
	case MINE:
		break;
	default:
		break;
	}
}