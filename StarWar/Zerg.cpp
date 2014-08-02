#include "Zerg.h"

CZerg::CZerg(float width, float height, float depth, float moveSpeed, CGameNode *target) : 
		m_width(width), m_height(height), m_depth(depth), CTargetedBullet(target)
{
	m_type = ZERG; 	
	m_moveSpeed = moveSpeed;
	m_enableControl = false;
	m_enablePhysics = true;
	m_enableRender = true;
	m_isControlable = true; 
	m_isFlyable = true;
}

void CZerg::SetAlive()
{
	m_isAlive = true; 
	m_enableControl = false;
	m_enablePhysics = true;
	m_enableRender = true;
	m_isControlable = true; 
	m_isFlyable = true;
	m_transform.SetPosition(m_originalPosition);
	m_transform.SetPosition(m_originalPosition);
}

void CZerg::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{	
	if(m_enableRender)
	{
		pd3dDevice->SetTransform(D3DTS_WORLD, &m_transform.GetWorldMatrix());	
		pd3dDevice->SetMaterial(&m_pMeshMaterials);
		m_pMesh->DrawSubset(0);
	}
}

void CZerg::Update()
{
	if(!m_enableControl && m_isAlive)
	{
		MoveToTarget();
	}
}
	
bool CZerg::InitVertices()
{
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();

	D3DXCreateBox(pd3dDevice, m_width, m_height, m_depth, &m_pMesh, NULL);
	ZeroMemory(&m_pMeshMaterials, sizeof(D3DMATERIAL9));
	m_pMeshMaterials.Diffuse.r = 1;
	m_pMeshMaterials.Diffuse.g = 0;
	m_pMeshMaterials.Diffuse.b = 0;
	m_pMeshMaterials.Diffuse.a = 0.8;
	m_pMeshMaterials.Ambient.r = 1;
	m_pMeshMaterials.Ambient.g = 0;
	m_pMeshMaterials.Ambient.b = 0;
	m_pMeshMaterials.Ambient.a = 0.8;
	return true;
}

bool CZerg::InitColliders()
{
	m_originalPosition = m_transform.GetWorldPosition();
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

void CZerg::CollidingCallback(CGameNode *collided)
{
	switch(collided->GetType())
	{
	case PLATFORM:		
		break;
	case HUMAN:
	case ZERG:
		break;
	case ZERG_TELEPORT:
		break;
	case MINE:
		Recycle();
		break;
	default:
		break;
	}
}

void CZerg::CollidedCallback(CGameNode *colliding)
{
	switch(colliding->GetType())
	{
	case PLATFORM:		
		break;
	case HUMAN:
	case ZERG:
		break;
	case ZERG_TELEPORT:
		break;
	case MINE:
		Recycle();
		break;
	default:
		break;
	}
}