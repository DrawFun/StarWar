#include "Player.h"
#include "Platform.h"

CPlayer::CPlayer() 
{
	m_hp = CHitPoint(100);
	m_type = HUMAN; 
	m_moveSpeed = 20.0f;
	m_enableControl = false;
	m_enablePhysics = true;
	m_enableRender = true;
	m_isControlable = true; 
	m_isFlyable = false;
}

void CPlayer::Update()
{
}

void CPlayer::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{			
	if(m_enableRender)
	{
		pd3dDevice->SetTransform(D3DTS_WORLD, &m_transform.GetWorldMatrix());	
		pd3dDevice->SetMaterial(&m_pMeshMaterials);
		m_pMesh->DrawSubset(0);
	}
}

bool CPlayer::InitVertices()
{
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();
	D3DXCreateSphere(pd3dDevice, 2.5f, 20, 20, &m_pMesh, NULL);
	ZeroMemory(&m_pMeshMaterials, sizeof(D3DMATERIAL9));
	m_pMeshMaterials.Diffuse.r = 0;
	m_pMeshMaterials.Diffuse.g = 0;
	m_pMeshMaterials.Diffuse.b = 1;
	m_pMeshMaterials.Diffuse.a = 0.2;
	m_pMeshMaterials.Ambient.r = 0;
	m_pMeshMaterials.Ambient.g = 0;
	m_pMeshMaterials.Ambient.b = 1;
	m_pMeshMaterials.Ambient.a = 0.2;
	return true;
}

bool CPlayer::InitColliders()
{
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


void CPlayer::CollidingCallback(CGameNode *collided)
{
	AllocConsole();
	_cprintf("%d->%d\n", this->m_type, collided->GetType());
	switch(collided->GetType())
	{
	case ZERG:
		m_hp.Damage(ZERG_DAMAGE);
		if(m_hp.IsDead())
		{
			m_scene->EventCallBack(STARWAR_DESTROY, this);
		}
		break;
	case PLATFORM:
		m_transform.Translate(D3DXVECTOR3(0, dynamic_cast<CPlatform *> (collided)->GetHeight(), 0));		
		collided->GetTransform()->AddChild(&m_transform);
		break;
	case AIRPLANE:	
		this->SwitchRender(false);
		this->SwitchPhysics(false);
		m_scene->EventCallBack(STARWAR_IN_AIRPLANE, collided);
		break;
	case ZERG_TELEPORT:
		break;
	default:
		break;
	}
}
	

void CPlayer::CollidedCallback(CGameNode *colliding)
{

}