#include "Platform.h"

CPlatform::CPlatform(float width, float height, float depth, 
					 float horizonlSpeed, float rotationSpeed) : 
	m_width(width), m_height(height), m_depth(depth), m_horizontalSpeed(horizonlSpeed), m_rotationSpeed(rotationSpeed)
{
	m_type = PLATFORM; 
	m_moveCounter = 0; 
	m_enableControl = false;
	m_enablePhysics = true;
	m_enableRender = true;
	m_isControlable = true; 
	m_isFlyable = false;	
}

CPlatform::~CPlatform()
{
	delete m_pMesh;
}

void CPlatform::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{	
	if(m_enableRender)
	{
		pd3dDevice->SetTransform(D3DTS_WORLD, &m_transform.GetWorldMatrix());	
		pd3dDevice->SetMaterial(&m_meshMaterials);
		m_pMesh->DrawSubset(0);
	}
}

void CPlatform::Update()
{
	if(m_moveCounter < PLATFORM_MOVE_FRAME_PERIOD)
	{
		//m_transform.Yaw(m_rotationSpeed);
		m_transform.Translate(D3DXVECTOR3(m_horizontalSpeed, 0.0f, m_horizontalSpeed));
		++m_moveCounter;
	}
	else
	{
		m_moveCounter = 0;
		m_rotationSpeed = -m_rotationSpeed;
		m_horizontalSpeed = -m_horizontalSpeed;
	}
}
	
bool CPlatform::InitVertices()
{
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();

	D3DXCreateBox(pd3dDevice, m_width, m_height, m_depth, &m_pMesh, NULL);
	ZeroMemory(&m_meshMaterials, sizeof(D3DMATERIAL9));
	m_meshMaterials.Diffuse.r = 0;
	m_meshMaterials.Diffuse.g = 1;
	m_meshMaterials.Diffuse.b = 1;
	m_meshMaterials.Diffuse.a = 0.5;
	m_meshMaterials.Ambient.r = 0;
	m_meshMaterials.Ambient.g = 1;
	m_meshMaterials.Ambient.b = 1;
	m_meshMaterials.Ambient.a = 0.5;
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
	//当前未定义行为
}

void CPlatform::CollidedCallback(CGameNode *colliding)
{
	//当前未定义行为
}