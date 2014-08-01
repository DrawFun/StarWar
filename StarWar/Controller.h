#ifndef _STARWAR_CONTROLLER_H_
#define _STARWAR_CONTROLLER_H_

#include "GameNode.h"

struct ControllerInput
{
	bool isLButtonDown;
	float elpasedTime;
	POINT currentMousePosition;
	unsigned char keys[256];
	ControllerInput(){};
};

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

public:
	CController(CGameNode *target);
	void Control(const ControllerInput &input);

private:
	void AdjustTransform();
};

#endif