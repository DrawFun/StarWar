#ifndef _STARWAR_CONTROLLER_H_
#define _STARWAR_CONTROLLER_H_

#include "GameNode.h"

class CController
{
protected:	
	CGameNode *m_target;
	POINT m_lastMousePosition; //Last mouse position

	D3DXVECTOR3	m_position;    // Position
	D3DXVECTOR3 m_rotation; //Rotation
	D3DXVECTOR3	m_look;  // Look Vector
	D3DXVECTOR3	m_up;      // Up Vector
	D3DXVECTOR3	m_right;   // Right Vector

	float m_missileColdDownTimer;
	int m_rollEffectCounter;

public:
	CController(CGameNode *target);
	void SwitchController(CGameNode *target);
	CGameNode *GetControllerTarget(){return m_target;};
	void Control(const ControllerInput &input);

private:
	void AdjustTransform();
};

#endif