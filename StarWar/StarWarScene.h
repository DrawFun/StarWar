#ifndef _STARWAR_SCENEWARSCENE_H_
#define _STARWAR_SCENEWARSCENE_H_

#include "GameNode.h"
#include <list>

#include "Physics.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Camera.h"

#include "Player.h"
#include "Mine.h"
#include "Platform.h"
#include "Airplane.h"
#include "Zerg.h"
#include "ZergTeleport.h"
#include "Missile.h"

#include "Controller.h"

class CStarWarScene : public CScene
{
private:
	static const int PLATFORM_NUM = 3;
	static const int AIRPLANE_NUM = 3;	
	static const int MINE_NUM = 3;
	static const int MAX_ZERG_NUM = 5;
	static const int MAX_MISSILE_NUM = 10;

	CPhysics *m_pPhysics;

	//Skybox pointer
	CSkyBox *m_pSkyBox;
	//Terrain pointer
	CTerrain *m_pTerrain;
	CCamera *m_pMainCamera;	
	CPlayer *m_pPlayer;
	CZergTeleport *m_pZergTeleport;

	CPlatform *m_pArrayPlatform[PLATFORM_NUM];
	CMine *m_pArrayMine[MINE_NUM];
	CAirplane *m_pArrayAirplane[AIRPLANE_NUM];
	CZerg *m_pArrayZerg[MAX_ZERG_NUM];	
	CMissile *m_pArrayMissile[MAX_MISSILE_NUM];

	CController *m_pController;

	int m_enemyResource;
	int m_playerResource;
	
protected:
	std::list<CGameNode*> m_listRootGameNodes;
	std::list<CGameNode*> m_listCollisionGameNodes;
public:	
	CStarWarScene();
	void Update(ControllerInput &input);
	void EventCallBack(int triggerEvent, void *trigger);


private:
	void CreateZergFromPool();
	void CreateMissileFromPool();
	void WinOrLose();
};

#endif