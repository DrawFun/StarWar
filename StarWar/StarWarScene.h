#ifndef _STARWAR_SCENEWARSCENE_H_
#define _STARWAR_SCENEWARSCENE_H_

#include "GameNode.h"
#include <list>

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

class CStarWarScene
{
private:
	static const int PLATFORM_NUM = 3;
	static const int AIRPLANE_NUM = 3;	
	static const int MINE_NUM = 3;
	static const int MAX_ZERG_NUM = 5;

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
	std::list<CZerg*> m_listZergs;
	std::list<CMissile*> m_listMissile;

	CController *m_pController;
	
protected:
	std::list<CGameNode*> m_listRootGameNodes;
public:	
	CStarWarScene();
	void Update(ControllerInput &input);
	void UpdatePhysics();

};

#endif