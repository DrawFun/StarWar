#include "StarWarScene.h"

CStarWarScene::CStarWarScene()
{
	//��͸����Ⱦ
	CDXEngine::Instance()->GetDxDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true); 
	CDXEngine::Instance()->GetDxDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); 
	CDXEngine::Instance()->GetDxDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); 
	CDXEngine::Instance()->GetDxDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	
	CDXEngine::Instance()->GetDxDevice()->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,TRUE);

	//������Ʒ��ʼ��
	//�����
	m_pSkyBox = new CSkyBox();
	m_pTerrain = new CTerrain(300, 300, -150, -150, 2, -2, 16);
	
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
	
	//��Ϸ״̬��ʼ��
	m_enemyResource = 1;
	m_playerResource = MINE_NUM + 1;
	

	//�����ʼ��
	m_pMainCamera = new CCamera(m_pPlayer, D3DXVECTOR3(0, 5, -15));
	//��պа󶨵����
	m_pSkyBox->SetCamera(m_pMainCamera);

	//��������ɫ��
	m_pController = new CController(m_pPlayer);

	//������ʼ����������Ķ������������Ϊ���У�
	m_pPhysics = new CPhysics(m_listRootGameNodes);
}

//����
CStarWarScene::~CStarWarScene()
{
	delete m_pController;
	delete m_pMainCamera;

	for(auto node : m_listRootGameNodes)
	{
		delete node;
	}
}

//�Ӷ�����д�������Zerg��
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

//�Ӷ�����д������󣨵�����
void CStarWarScene::CreateMissileFromPool()
{
	for(int i = 0; i < MAX_MISSILE_NUM; ++i)
	{
		if(!m_pArrayMissile[i]->IsAlive())
		{	
			m_pArrayMissile[i]->GetTransform()->SetPosition
				(m_pController->GetControllerTarget()->GetTransform()->GetWorldPosition());
			m_pArrayMissile[i]->GetTransform()->SetWorldPosition
				(m_pController->GetControllerTarget()->GetTransform()->GetWorldPosition());
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

//���µ�ǰ����
void CStarWarScene::Update(ControllerInput &input)
{
	//���¿��Ʋ�
	m_pController->Control(input);
	
	//���¸��������Transform
	//��������ʽ������֯���ɸ��������εݹ����
	rootNode.UpdateMatrix();

	//���¸�������������
	//�����ͨ����ײ��ξ�������������Ž�����֯�Ż�
	m_pPhysics->CollisionCheck();

	//����Input�������Ľ������������Ϊ����
	for(auto gameNode : m_listRootGameNodes)
	{
		gameNode->Update();
	}

	//�������е����нڵ���л���
	//������ƣ����Խ��к�������ͬ�ڵ������Ƶ��Ż���ʩ��û����������
	for(auto gameNode : m_listRootGameNodes)
	{
		gameNode->Render(CDXEngine::Instance()->GetDxDevice());
	}

	//����View Matrix
	m_pMainCamera->LateUpdate();

	//�ж���Ϸ״̬
	WinOrLose();
}

//�����ص�����
//�ó����ж����ĳЩ�¼�������Ӧ�������Ĵ��������٣����ƶ����ת��
void CStarWarScene::EventCallBack(int triggerEvent, void *trigger)
{
	CGameNode *pGameNode;
	D3DXVECTOR3 adjustRotation;
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
			m_enemyResource = max(!m_pZergTeleport->GetHitPoint()->IsDead(), m_enemyResource);
			break;
		case HUMAN:
		case MINE:
			--m_playerResource;
			m_playerResource = max(!m_pPlayer->GetHitPoint()->IsDead(), m_playerResource);
			break;
		default:
			break;
		}
		break;
	case STARWAR_IN_AIRPLANE:
		//���ɻ��ĸ��ڵ�����Ϊ������ڵ�
		//���ɻ�����Ϊ���ƶ���
		//���ɻ�����Ϊ����������
		rootNode.AddChild(((CGameNode *)trigger)->GetTransform());
		m_pController->SwitchController((CGameNode *)trigger);
		m_pMainCamera->SwitchTarget((CGameNode *)trigger);
		break;
	case STARWAR_OUT_AIRPLANE:
		//���ݷɻ���Transform����Player��Transform������z��ƫ�ƣ���ΪPlayerû��Roll�������ɻ��У�
		pGameNode = (CGameNode *)trigger;
		//�·ɻ���һ����ƫ��������ֹ������ײ�ٴ��Ϸɻ�������
		m_pPlayer->GetTransform()->SetWorldPosition(pGameNode->GetTransform()->GetWorldPosition() + D3DXVECTOR3(10, 0, 10));
		adjustRotation = pGameNode->GetTransform()->GetRotation();
		adjustRotation.z = 0;
		m_pPlayer->GetTransform()->SetRotation(adjustRotation);
		pGameNode->GetTransform()->SetRotation(D3DXVECTOR3(0,0,0));

		//���ڵ�����Ϊ������ڵ�
		//������ƶ��󣬸������
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

//��Ϸ״̬�ж�
void CStarWarScene::WinOrLose()
{
	RECT rect = {20, 20, 300, 200};	
	char buffer[200];
	int length;

	if(m_pPlayer->GetHitPoint()->IsDead())
	{
		length = sprintf_s(buffer, "Lose...\nHuman HP: %d\nHuman Units: %d\nEnemy Units: %d\n", 
			m_pPlayer->GetHitPoint()->GetCurrentHP(), m_playerResource, m_enemyResource);
	}
	else if(m_enemyResource == 0)
	{
		length = sprintf_s(buffer, "Win!!!\nHuman HP: %d\nHuman Units: %d\nEnemy Units: %d\n", 
			m_pPlayer->GetHitPoint()->GetCurrentHP(), m_playerResource, m_enemyResource);
	}
	else
	{
		length = sprintf_s(buffer, "Battling\nHuman HP: %d\nHuman Units: %d\nEnemy Units: %d\n", 
			m_pPlayer->GetHitPoint()->GetCurrentHP(), m_playerResource, m_enemyResource);
	}
	CDXEngine::Instance()->GetFont()->DrawTextA(NULL, buffer, length, &rect, DT_TOP | DT_LEFT, 0xffffffff);
}