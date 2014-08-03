#include "Mine.h"

CMine::CMine(float rotateSpeed = 0) : m_rotateSpeed(rotateSpeed) 
{
	m_hp = CHitPoint(20);
	m_type = MINE;	
	m_enableControl = false;
	m_enablePhysics = true;
	m_enableRender = true;
	m_isControlable = true; 
	m_isFlyable = false;	
}

void CMine::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if(m_enableRender)
	{
		pd3dDevice->SetTransform(D3DTS_WORLD, &m_transform.GetWorldMatrix());	

		//Set texture on stage 0
		pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		//Set RGB mix method
		pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	
		//Set texture on stage 1
		pd3dDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		pd3dDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		//Set RGB mix method
		pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
		pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);

		for( unsigned long i = 0; i < snowManNumMaterials; ++i )
		{
			pd3dDevice->SetMaterial(&pSnowmanMeshMaterials[i]);
		
			//Set multiple textures
			pd3dDevice->SetTexture(0, pSnowmanTexture0);
			pd3dDevice->SetTexture(1, pSnowmanTexture1);
			m_pMesh->DrawSubset(i);
		}
	}
}

void CMine::Update()
{
	m_transform.Yaw(m_rotateSpeed);
}
	
bool CMine::InitVertices()
{			
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();
	
	//Init snowman x model
	LPD3DXBUFFER pD3DXMtrlBuffer;

    D3DXLoadMeshFromX( "Resource//Snowman.x", D3DXMESH_SYSTEMMEM, 
                       pd3dDevice, NULL, 
                       &pD3DXMtrlBuffer, NULL, &snowManNumMaterials, 
                       &m_pMesh );

    D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    pSnowmanMeshMaterials = new D3DMATERIAL9[snowManNumMaterials];
    for( unsigned long i = 0; i < snowManNumMaterials; ++i )
    {
        //Set materials
        pSnowmanMeshMaterials[i] = d3dxMaterials[i].MatD3D;
        //Set the ambient color for the material
        pSnowmanMeshMaterials[i].Ambient = pSnowmanMeshMaterials[i].Diffuse;
    }

    pD3DXMtrlBuffer->Release();
	//TODO: NULL POINT FAIL
	//LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();

	//Create textures from files
	D3DXCreateTextureFromFile( pd3dDevice, "Resource//Snowman1.jpg", &pSnowmanTexture0);
	D3DXCreateTextureFromFile( pd3dDevice, "Resource//Snowman2.jpg", &pSnowmanTexture1);
		
	return true;	
}
	
bool CMine::InitColliders()
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


void CMine::CollidingCallback(CGameNode *collided)
{
	//AllocConsole();
	//_cprintf("%d->%d\n", this->m_type, collided->GetType());
}
	

void CMine::CollidedCallback(CGameNode *colliding)
{
	switch(colliding->GetType())
	{
	case ZERG:
		m_hp.Damage(ZERG_DAMAGE);
		if(m_hp.IsDead())
		{
			m_rotateSpeed = 0;
			m_scene->EventCallBack(STARWAR_DESTROY, this);
		}
		break;
	default:
		break;
	}
}