#include "Missile.h"

CMissile::CMissile(float width, float height, float depth, float moveSpeed, CGameNode *target) : 
		m_width(width), m_height(height), m_depth(depth), CTargetedBullet(target)
{
	m_type = MISSILE; 	
	m_moveSpeed = moveSpeed;
	m_enableControl = false;
	m_enablePhysics = true;
	m_enableRender = true;
	m_isControlable = true; 
	m_isFlyable = true;
}

void CMissile::SetAlive()
{
	m_isAlive = true; 
	m_destroyCounter = 0; 
	m_enableControl = false;
	m_enablePhysics = true;
	m_enableRender = true;
	m_isControlable = true; 
	m_isFlyable = true;
}

void CMissile::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{	
	if(m_enableRender)
	{
		pd3dDevice->SetTransform(D3DTS_WORLD, &m_transform.GetWorldMatrix());	
		pd3dDevice->SetMaterial(&m_pMeshMaterials);
		m_pMesh->DrawSubset(0);
	}
}

void CMissile::Update()
{
	if(!m_enableControl && m_isAlive)
	{
		if(m_destroyCounter < DESTROY_FRAME_PERIOD)
		{
			//m_transform.Yaw(m_rotationSpeed);
			++m_destroyCounter;
		}
		else
		{
			Recycle();
			m_destroyCounter = 0;
		}
		MoveToTarget();
	}
}
	
bool CMissile::InitVertices()
{
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();

	D3DXCreateCylinder(pd3dDevice, 0.5, 0.05, 1, 10, 10, &m_pMesh, NULL);
	ZeroMemory(&m_pMeshMaterials, sizeof(D3DMATERIAL9));
	m_pMeshMaterials.Diffuse.r = 0;
	m_pMeshMaterials.Diffuse.g = 0;
	m_pMeshMaterials.Diffuse.b = 0;
	m_pMeshMaterials.Diffuse.a = 0.8;
	m_pMeshMaterials.Ambient.r = 0;
	m_pMeshMaterials.Ambient.g = 0;
	m_pMeshMaterials.Ambient.b = 0;
	m_pMeshMaterials.Ambient.a = 0.8;
	return true;
}

bool CMissile::InitColliders()
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

void CMissile::CollidingCallback(CGameNode *collided)
{
	AllocConsole();
	_cprintf("Missile colliding\n");
}

void CMissile::CollidedCallback(CGameNode *colliding)
{
	AllocConsole();
	_cprintf("Missile collided\n");
	switch(colliding->GetType())
	{
	case PLATFORM:		
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