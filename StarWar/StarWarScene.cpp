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
		m_pArrayPlatform[i] = new CPlatform(20.0f, 0.5f, 20.0f, 0.02f, 0.00f);
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
		m_pArrayZerg[i] = new CZerg(3, 3, 3, 0.05, m_pArrayMine[i % MINE_NUM]);
		m_pArrayZerg[i]->InitTransform(zergTramsforms[i]);
		rootNode.AddChild(m_pArrayZerg[i]->GetTransform());
		m_listRootGameNodes.push_back(m_pArrayZerg[i]);
		m_listCollisionGameNodes.push_back(m_pArrayZerg[i]);
		m_pArrayZerg[i]->Recycle();		
	}

	for(int i = 0; i < MAX_MISSILE_NUM; ++i)
	{
		m_pArrayMissile[i] = new CMissile(0.5, 0.05, 1, 5, m_pZergTeleport);
		m_pArrayMissile[i]->InitTransform(CTransform());
		rootNode.AddChild(m_pArrayMissile[i]->GetTransform());
		m_listRootGameNodes.push_back(m_pArrayMissile[i]);
		m_listCollisionGameNodes.push_back(m_pArrayMissile[i]);	
		m_pArrayMissile[i]->Recycle();
	}

	for(auto gameNode : m_listRootGameNodes)
	{
		gameNode->SetScene(this);
		gameNode->InitVertices();		
		gameNode->InitColliders();
	}
	
	m_enemyResource = 1;
	m_playerResource = MINE_NUM + 1;
	

	m_pMainCamera = new CCamera(m_pPlayer, D3DXVECTOR3(0, 5, -15));
	m_pSkyBox->SetCamera(m_pMainCamera);

	m_pController = new CController(m_pPlayer);

	m_pPhysics = new CPhysics(m_listRootGameNodes);
}

void CStarWarScene::CreateZergFromPool()
{
	for(int i = 0; i < MAX_ZERG_NUM; ++i)
	{
		if(!m_pArrayZerg[i]->IsAlive())
		{
			m_pArrayZerg[i]->SetAlive();
			++m_enemyResource;
			return;
		}
	}
}

void CStarWarScene::CreateMissileFromPool()
{
	for(int i = 0; i < MAX_MISSILE_NUM; ++i)
	{
		if(!m_pArrayMissile[i]->IsAlive())
		{			
			m_pArrayMissile[i]->GetTransform()->SetPosition
				(m_pController->GetControllerTarget()->GetTransform()->GetPosition());
			m_pArrayMissile[i]->GetTransform()->SetRotation
				(m_pController->GetControllerTarget()->GetTransform()->GetRotation());
			m_pArrayMissile[i]->SetAlive();

			for(int j = 0; j < MAX_ZERG_NUM; ++j)
			{
				if(m_pArrayZerg[j]->IsAlive())
				{
					m_pArrayMissile[i]->SetTarget(m_pArrayZerg[j]);
					return;
				}
			}
			m_pArrayMissile[i]->SetTarget(m_pZergTeleport);
			return;
		}
	}
}

void CStarWarScene::Update(ControllerInput &input)
{
	m_pController->Control(input);
	
	rootNode.UpdateMatrix();

	m_pPhysics->CollisionCheck();

	for(auto gameNode : m_listRootGameNodes)
	{
		gameNode->Update();
	}

	for(auto gameNode : m_listRootGameNodes)
	{
		gameNode->Render(CDXEngine::Instance()->GetDxDevice());
	}

	m_pMainCamera->LateUpdate();

	WinOrLose();
}

void CStarWarScene::EventCallBack(int triggerEvent, void *trigger)
{
	switch(triggerEvent)
	{
	case STARWAR_CREATE:
		switch (((CGameNode *)trigger)->GetType())
		{
		case ZERG_TELEPORT:
			CreateZergFromPool();
			break;
		case AIRPLANE:
			CreateMissileFromPool();
			break;
		default:
			break;
		}
		break;
	case STARWAR_DESTROY:
		switch (((CGameNode *)trigger)->GetType())
		{
		case ZERG_TELEPORT:
		case ZERG:
			--m_enemyResource;
			break;
		case HUMAN:
		case MINE:
			--m_playerResource;
			break;
		default:
			break;
		}
		break;
	case STARWAR_IN_AIRPLANE:
		rootNode.AddChild(((CGameNode *)trigger)->GetTransform());
		m_pController->SwitchController((CGameNode *)trigger);
		m_pMainCamera->SwitchTarget((CGameNode *)trigger);
		break;
	case STARWAR_OUT_AIRPLANE:
		m_pPlayer->GetTransform()->SetWorldPosition(((CGameNode *)trigger)->GetTransform()->GetWorldPosition() + D3DXVECTOR3(10, 0, 10));
		m_pPlayer->GetTransform()->SetRotation(((CGameNode *)trigger)->GetTransform()->GetRotation());
		rootNode.AddChild(((CGameNode *)trigger)->GetTransform());		
		rootNode.AddChild(m_pPlayer->GetTransform());
		m_pPlayer->SwitchRender(true);
		m_pPlayer->SwitchPhysics(true);
		m_pController->SwitchController(m_pPlayer);
		m_pMainCamera->SwitchTarget(m_pPlayer);
		break;
	default:
		break;
	}
}

void CStarWarScene::WinOrLose()
{
	if(m_pPlayer->GetHitPoint()->IsDead())
	{
		;
	}
	if(m_enemyResource == 0)
	{
		;
	}
}