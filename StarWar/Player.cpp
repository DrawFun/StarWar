#include "Player.h"

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
	m_onPlatform = false;
}

void CPlayer::Update()
{
	//没有碰撞分离的Trigger
	//通过和平台的距离判定是否在平台上
	//提升处理效率
	if(m_onPlatform && D3DXVec3Length(&m_transform.GetPosition()) > 20 / 1.414)
	{
		m_onPlatform = false;
		m_transform.ResetTransform(GetScene()->GetRootNode());
	}
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
	switch(collided->GetType())
	{
	case ZERG:
		m_hp.Damage(ZERG_DAMAGE);
		collided->CollidedCallback(this);
		if(m_hp.IsDead())
		{
			m_scene->EventCallBack(STARWAR_DESTROY, this);
		}
		break;
	case PLATFORM:			
		collided->GetTransform()->AddChild(&m_transform);
		m_onPlatform = true;
		break;
	case AIRPLANE:	
		this->SwitchRender(false);
		this->SwitchPhysics(false);
		m_scene->EventCallBack(STARWAR_IN_AIRPLANE, collided);
		m_onPlatform = false;
		break;
	case ZERG_TELEPORT:
		break;
	default:
		break;
	}
}
	

void CPlayer::CollidedCallback(CGameNode *colliding)
{	
	//当前未定义行为
	//如被撞击的逻辑写在主动撞击中（撞击是相互的，因此两个挑一个进行处理）
	//该接口尚保留，若碰撞检测开销过大，可以改成单方向判定，而非如今的双层循环（存在一半冗余）
	
	//已经改为单向判定，但没时间删接口了，先文档先T_T
}