#include "Zerg.h"

CZerg::CZerg(float width, float height, float depth, float moveSpeed, CGameNode *target) : 
		m_width(width), m_height(height), m_depth(depth), CTargetedBullet(target)
{
	m_hp = CHitPoint(100);

	m_type = ZERG; 	
	m_moveSpeed = moveSpeed;
	m_enableControl = false;
	m_enablePhysics = true;
	m_enableRender = true;
	m_isControlable = true; 
	m_isFlyable = true;
}

CZerg::~CZerg()
{
	if( m_pMesh != NULL )
        m_pMesh->Release(); 
}


void CZerg::SetAlive()
{
	m_transform.SetPosition(m_originalPosition);
	m_transform.SetPosition(m_originalPosition);
	m_isAlive = true; 
	m_enableControl = false;
	m_enablePhysics = true;
	m_enableRender = true;
	m_isControlable = true; 
	m_isFlyable = true;
}

void CZerg::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{	
	if(m_enableRender)
	{
		pd3dDevice->SetTransform(D3DTS_WORLD, &m_transform.GetWorldMatrix());	
		pd3dDevice->SetMaterial(&m_meshMaterials);
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
	ZeroMemory(&m_meshMaterials, sizeof(D3DMATERIAL9));
	m_meshMaterials.Diffuse.r = 1;
	m_meshMaterials.Diffuse.g = 0;
	m_meshMaterials.Diffuse.b = 0;
	m_meshMaterials.Diffuse.a = 0.8;
	m_meshMaterials.Ambient.r = 1;
	m_meshMaterials.Ambient.g = 0;
	m_meshMaterials.Ambient.b = 0;
	m_meshMaterials.Ambient.a = 0.8;
	return true;
}

bool CZerg::InitColliders()
{
	m_originalPosition = m_transform.GetWorldPosition();
	D3DXVECTOR3 pCenter;
	float pRadius;
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	m_pMesh->GetDeclaration(decl);
	LPVOID pVB;
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, &pVB);
	UINT uStride = D3DXGetDeclVertexSize(decl, 0);	
	D3DXComputeBoundingSphere((const D3DXVECTOR3* )pVB, m_pMesh->GetNumVertices(), uStride, &pCenter, &pRadius);
	Collider collider(pCenter, pRadius);
	m_colliders.push_back(collider);
	return true;
}

void CZerg::CollidingCallback(CGameNode *collided)
{
	switch(collided->GetType())
	{
	case HUMAN:
	case MINE:		
		Recycle();
		m_hp.Reborn();
		m_scene->EventCallBack(STARWAR_DESTROY, this);		
		break;
	case MISSILE:
		m_hp.Damage(MISSILE_DAMAGE);
		if(m_hp.IsDead())
		{
			m_scene->EventCallBack(STARWAR_DESTROY, this);
			m_hp.Reborn();
			Recycle();	
		}
		break;
	default:
		break;
	}
}

void CZerg::CollidedCallback(CGameNode *colliding)
{
	switch(colliding->GetType())
	{
	case MISSILE:	
		m_hp.Damage(MISSILE_DAMAGE);
		if(m_hp.IsDead())
		{
			m_scene->EventCallBack(STARWAR_DESTROY, this);
			m_hp.Reborn();
			Recycle();	
		}
		break;
	case MINE:		
	case HUMAN:
		Recycle();
		m_hp.Reborn();
		m_scene->EventCallBack(STARWAR_DESTROY, this);
		break;
	default:
		break;
	}
}