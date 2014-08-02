#include "StarWarScene.h"


CStarWarScene::CStarWarScene()
{	
	m_pSkyBox = new CSkyBox();
	m_pTerrain = new CTerrain(300, 300, -150, -150, 2, -2, 2);
	
	m_pPlayer = new CPlayer();
	CTransform playerTramsform(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.05,0.05,0.05));
	m_pPlayer->InitTransform(playerTramsform);	

	rootNode.AddChild(m_pSkyBox->GetTransform());
	rootNode.AddChild(m_pTerrain->GetTransform());
	rootNode.AddChild(m_pPlayer->GetTransform());

	m_listRootGameNodes.push_back(m_pSkyBox);
	m_listRootGameNodes.push_back(m_pTerrain);
	m_listRootGameNodes.push_back(m_pPlayer);

	CTransform platformTramsforms[PLATFORM_NUM] = 
		{D3DXVECTOR3(50.0f, 0.0f, -50.0f),
		D3DXVECTOR3(-50.0f, 0.0f, -50.0f), 
		D3DXVECTOR3(-50.0f, 0.0f,  50.0f)};	

	for(int i = 0; i < PLATFORM_NUM; ++i)
	{
		m_pArrayPlatform[i] = new CPlatform(20.0f, 1.0f, 20.0f, 0.02f, 0.005f);
		m_pArrayPlatform[i]->InitTransform(platformTramsforms[i]);
		rootNode.AddChild(m_pArrayPlatform[i]->GetTransform());
		m_listRootGameNodes.push_back(m_pArrayPlatform[i]);
	}

	CTransform mineTramsforms[MINE_NUM] = 
		{CTransform(D3DXVECTOR3(50.0f, 1.0f, -50.0f), D3DXVECTOR3(0.05,0.05,0.05)),
		CTransform(D3DXVECTOR3(-50.0f, 1.0f, -50.0f), D3DXVECTOR3(0.05,0.05,0.05)), 
		CTransform(D3DXVECTOR3(-50.0f, 1.0f, 50.0f), D3DXVECTOR3(0.05,0.05,0.05))};	

	for(int i = 0; i < MINE_NUM; ++i)
	{
		m_pArrayMine[i] = new CMine(0.0f); 
		m_pArrayMine[i]->InitTransform(mineTramsforms[i]);
		m_pArrayPlatform[i]->GetTransform()->AddChild(m_pArrayMine[i]->GetTransform());
		m_listRootGameNodes.push_back(m_pArrayMine[i]);
	}

	CTransform airplaneTramsforms[MINE_NUM] = 
		{CTransform(D3DXVECTOR3(50.0f, 1.0f, -50.0f) + D3DXVECTOR3(10.0f, 0.0f, 10.0f), D3DXVECTOR3(4,4,4)),
		CTransform(D3DXVECTOR3(-50.0f, 1.0f, -50.0f) + D3DXVECTOR3(10.0f, 0.0f, 10.0f), D3DXVECTOR3(4,4,4)), 
		CTransform(D3DXVECTOR3(-50.0f, 1.0f, 50.0f) + D3DXVECTOR3(10.0f, 0.0f, 10.0f), D3DXVECTOR3(4,4,4))};	

	for(int i = 0; i < AIRPLANE_NUM; ++i)
	{
		m_pArrayAirplane[i] = new CAirplane();
		m_pArrayAirplane[i]->InitTransform(airplaneTramsforms[i]);
		m_pArrayPlatform[i]->GetTransform()->AddChild(m_pArrayAirplane[i]->GetTransform());
		m_listRootGameNodes.push_back(m_pArrayAirplane[i]);
	}

	m_pZergTeleport = new CZergTeleport();		
	m_pZergTeleport->InitTransform(CTransform(D3DXVECTOR3(50.0f, 0.0f, 50.0f)));
	m_pZergTeleport->InitVertices();
	m_pZergTeleport->InitColliders();
	rootNode.AddChild(m_pZergTeleport->GetTransform());
	m_listRootGameNodes.push_back(m_pZergTeleport);

	for(auto gameNode : m_listRootGameNodes)
	{
		gameNode->InitVertices();		
		gameNode->InitColliders();
	}
	
	m_pMainCamera = new CCamera(m_pArrayMine[0], D3DXVECTOR3(0, 0, -15));
	m_pSkyBox->SetCamera(m_pMainCamera);

	m_pController = new CController(m_pArrayMine[0]);

}

void CStarWarScene::Update(ControllerInput &input)
{
	m_pController->Control(input);
	
	rootNode.UpdateMatrix();

	for(auto gameNode : m_listRootGameNodes)
	{
		gameNode->Update();
	}

	m_pMainCamera->LateUpdate();
}

void UpdatePhysics()
{
	
}