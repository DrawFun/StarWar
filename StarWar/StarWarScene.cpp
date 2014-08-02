#include "StarWarScene.h"

CStarWarScene::CStarWarScene()
{
	CDXEngine::Instance()->GetDxDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true); 
	CDXEngine::Instance()->GetDxDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); 
	CDXEngine::Instance()->GetDxDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); 
	CDXEngine::Instance()->GetDxDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	
	CDXEngine::Instance()->GetDxDevice()->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,TRUE);

	m_pSkyBox = new CSkyBox();
	m_pTerrain = new CTerrain(300, 300, -150, -150, 2, -2, 64);
	
	m_pPlayer = new CPlayer();
	CTransform playerTramsform(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pPlayer->InitTransform(playerTramsform);	

	rootNode.AddChild(m_pSkyBox->GetTransform());
	rootNode.AddChild(m_pTerrain->GetTransform());
	rootNode.AddChild(m_pPlayer->GetTransform());

	m_listRootGameNodes.push_back(m_pSkyBox);
	m_listRootGameNodes.push_back(m_pTerrain);
	m_listRootGameNodes.push_back(m_pPlayer);

	m_listCollisionGameNodes.push_back(m_pPlayer);

	CTransform platformTramsforms[PLATFORM_NUM] = 
		{D3DXVECTOR3(50.0f, 0.0f, -50.0f),
		D3DXVECTOR3(-50.0f, 0.0f, -50.0f), 
		D3DXVECTOR3(-50.0f, 0.0f,  50.0f)};	

	for(int i = 0; i < PLATFORM_NUM; ++i)
	{
		m_pArrayPlatform[i] = new CPlatform(20.0f, 2.0f, 20.0f, 0.02f, 0.00f);
		m_pArrayPlatform[i]->InitTransform(platformTramsforms[i]);
		rootNode.AddChild(m_pArrayPlatform[i]->GetTransform());
		m_listRootGameNodes.push_back(m_pArrayPlatform[i]);
		m_listCollisionGameNodes.push_back(m_pArrayPlatform[i]);
	}

	CTransform mineTramsforms[MINE_NUM] = 
		{CTransform(D3DXVECTOR3(50.0f, 1.0f, -50.0f), D3DXVECTOR3(0.05,0.05,0.05)),
		CTransform(D3DXVECTOR3(-50.0f, 1.0f, -50.0f), D3DXVECTOR3(0.05,0.05,0.05)), 
		CTransform(D3DXVECTOR3(-50.0f, 1.0f, 50.0f), D3DXVECTOR3(0.05,0.05,0.05))};	

	for(int i = 0; i < MINE_NUM; ++i)
	{
		m_pArrayMine[i] = new CMine(0.02f); 
		m_pArrayMine[i]->InitTransform(mineTramsforms[i]);
		m_pArrayPlatform[i]->GetTransform()->AddChild(m_pArrayMine[i]->GetTransform());
		m_listRootGameNodes.push_back(m_pArrayMine[i]);
		m_listCollisionGameNodes.push_back(m_pArrayMine[i]);
	}

	CTransform airplaneTramsforms[AIRPLANE_NUM] = 
		{CTransform(D3DXVECTOR3(50.0f, 1.0f, -50.0f) + D3DXVECTOR3(10.0f, 0.0f, 10.0f), D3DXVECTOR3(4,4,4)),
		CTransform(D3DXVECTOR3(-50.0f, 1.0f, -50.0f) + D3DXVECTOR3(10.0f, 0.0f, 10.0f), D3DXVECTOR3(4,4,4)), 
		CTransform(D3DXVECTOR3(-50.0f, 1.0f, 50.0f) + D3DXVECTOR3(10.0f, 0.0f, 10.0f), D3DXVECTOR3(4,4,4))};	

	for(int i = 0; i < AIRPLANE_NUM; ++i)
	{
		m_pArrayAirplane[i] = new CAirplane();
		m_pArrayAirplane[i]->InitTransform(airplaneTramsforms[i]);
		m_pArrayPlatform[i]->GetTransform()->AddChild(m_pArrayAirplane[i]->GetTransform());
		m_listRootGameNodes.push_back(m_pArrayAirplane[i]);
		m_listCollisionGameNodes.push_back(m_pArrayAirplane[i]);
	}

	m_pZergTeleport = new CZergTeleport(10.0f, 10.0f, 20.0f, 0.005f);		
	m_pZergTeleport->InitTransform(CTransform(D3DXVECTOR3(50.0f, 0.0f, 50.0f)));
	m_pZergTeleport->InitVertices();
	m_pZergTeleport->InitColliders();
	rootNode.AddChild(m_pZergTeleport->GetTransform());
	m_listRootGameNodes.push_back(m_pZergTeleport);
	m_listCollisionGameNodes.push_back(m_pZergTeleport);

	CTransform zergTramsforms[MAX_ZERG_NUM] = 
	   {m_pZergTeleport->GetTransform()->GetWorldPosition() + D3DXVECTOR3(-10.0f, 0.0f, -10.0f),
		m_pZergTeleport->GetTransform()->GetWorldPosition() + D3DXVECTOR3(-10.0f, 0.0f, 10.0f), 
		m_pZergTeleport->GetTransform()->GetWorldPosition() + D3DXVECTOR3(10.0f, 0.0f, -10.0f),
		m_pZergTeleport->GetTransform()->GetWorldPosition() + D3DXVECTOR3(10.0f, 0.0f, 10.0f),
		m_pZergTeleport->GetTransform()->GetWorldPosition() + D3DXVECTOR3(0.0f, 0.0f, 0.0f)};	

	for(int i = 0; i < MAX_ZERG_NUM; ++i)
	{
		m_pArrayZerg[i] = new CZerg(3, 3, 3, 0.5, m_pArrayMine[i % MINE_NUM]);
		m_pArrayZerg[i]->InitTransform(zergTramsforms[i]);
		rootNode.AddChild(m_pArrayZerg[i]->GetTransform());
		m_listRootGameNodes.push_back(m_pArrayZerg[i]);
		m_listCollisionGameNodes.push_back(m_pArrayZerg[i]);
	}


	for(auto gameNode : m_listRootGameNodes)
	{
		gameNode->SetScene(this);
		gameNode->InitVertices();		
		gameNode->InitColliders();
	}
	
	m_pMainCamera = new CCamera(m_pPlayer, D3DXVECTOR3(0, 0, -15));
	m_pSkyBox->SetCamera(m_pMainCamera);

	m_pController = new CController(m_pPlayer);

}

void CStarWarScene::Update(ControllerInput &input)
{
	m_pController->Control(input);
	
	rootNode.UpdateMatrix();

	for(auto collider : m_listCollisionGameNodes)
	{
		UpdatePhysics(collider);
	}

	for(auto gameNode : m_listRootGameNodes)
	{
		gameNode->Update();
	}

	for(auto gameNode : m_listRootGameNodes)
	{
		gameNode->Render(CDXEngine::Instance()->GetDxDevice());
	}

	m_pMainCamera->LateUpdate();
}

void CStarWarScene::UpdatePhysics(CGameNode *checking)
{
	for(auto checked : m_listCollisionGameNodes)
	{
		if(checking != checked && Collider::IsCollision(checking->GetCollider(), checked->GetCollider(), checking->GetTransform()->GetWorldPosition(), checked->GetTransform()->GetWorldPosition()))
		{
			checking->CollidingCallback(checked);
			checked->CollidedCallback(checking);
		}
	}
}

void CStarWarScene::CallBackEvent()
{
	CreateZergFromPool();
}

void CStarWarScene::CreateZergFromPool()
{
	for(int i = 0; i < MAX_ZERG_NUM; ++i)
	{
		if(!m_pArrayZerg[i]->IsAlive())
		{
			m_pArrayZerg[i]->SetAlive();
		}
	}
}