#include "ZergTeleport.h"
#include "StarWarScene.h"
CZergTeleport::CZergTeleport(float width, float height, float depth, float rotationSpeed) : 
		m_width(width), m_height(height), m_depth(depth), m_rotationSpeed(rotationSpeed)
{
	m_type = ZERG_TELEPORT; 	
	m_enableControl = false;
	m_enablePhysics = true;
	m_enableRender = true;
	m_isControlable = true; 
	m_isFlyable = false;
	m_generationCounter = 0;
}

void CZergTeleport::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{	
	if(m_enableRender)
	{
		pd3dDevice->SetTransform(D3DTS_WORLD, &m_transform.GetWorldMatrix());	
		pd3dDevice->SetMaterial(&m_pMeshMaterials);
		m_pMesh->DrawSubset(0);
	}
}

void CZergTeleport::Update()
{
	m_transform.Yaw(m_rotationSpeed);
	if(m_generationCounter < GENERATION_FRAME_PERIOD)
	{
		++m_generationCounter;
	}
	else
	{
		((CStarWarScene*)m_scene)->CallBackEvent();
		m_generationCounter = 0;
	}
}
	
bool CZergTeleport::InitVertices()
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

bool CZergTeleport::InitColliders()
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

void CZergTeleport::CollidingCallback(CGameNode *collided)
{
	AllocConsole();
	_cprintf("ZERGTELEPORT colliding\n");
}

void CZergTeleport::CollidedCallback(CGameNode *colliding)
{
	AllocConsole();
	_cprintf("ZERGTELEPORT collided\n");
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
		break;
	default:
		break;
	}
}