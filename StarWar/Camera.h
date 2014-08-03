//------------------------------------------------------------------------------
//           Name: Camera.h
//         Author: Dai Zhuofang
//    Description: Camera controller
//                 
//   Control Keys: W          - View moves forward
//                 S          - View moves backward
//                 A          - View strafes left
//                 D          - View strafes Right
//                 Left Mouse - Perform looking
//                 Mouse      - Look about the scene
//------------------------------------------------------------------------------
#ifndef _STARWAR_CAMERA_H_
#define _STARWAR_CAMERA_H_

#include "GameNode.h"

class CCamera
{
protected:
	CGameNode *m_target;
	D3DXVECTOR3 m_offsetPosition;
	D3DXVECTOR3 m_position;
	void UpdateViewMatrix();

public:
	D3DXVECTOR3 GetPosition(){return m_position;};
	CCamera(CGameNode *target, D3DXVECTOR3 &position);
	void SwitchTarget(CGameNode *target);
	void LateUpdate();
};

#endif