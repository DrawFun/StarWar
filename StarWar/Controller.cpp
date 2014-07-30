#include "Controller.h"

extern D3DXVECTOR3	g_vEye;  // Look Vector
extern D3DXVECTOR3	g_vLook;  // Look Vector
extern D3DXVECTOR3	g_vUp;      // Up Vector
extern D3DXVECTOR3	g_vRight;   // Right Vector

CController::CController(CGameNode *target)
{
	assert(target != NULL);
	m_target = target;
	D3DXMATRIX matrixRotationX, matrixRotationY, matrixRotationZ, matrixRotation;

	m_position = target->GetPosition();
	m_look = D3DXVECTOR3(0, 0, 1); 
	m_right = D3DXVECTOR3(1, 0, 0);
	m_up = D3DXVECTOR3(0, 1, 0);

	D3DXVECTOR3 rotation = m_target->GetRotation();
	D3DXMatrixRotationX(&matrixRotationX, rotation.x); 
	D3DXMatrixRotationY(&matrixRotationY, rotation.y); 
	D3DXMatrixRotationZ(&matrixRotationZ, rotation.z); 
	
	matrixRotation = matrixRotationZ * matrixRotationY * matrixRotationX;
	
	D3DXVec3TransformCoord(&m_look, &m_look, &matrixRotation);
	D3DXVec3TransformCoord(&m_right, &m_right, &matrixRotation);
	D3DXVec3TransformCoord(&m_up, &m_up, &matrixRotation);
}

void CController::Control(const ControllerInput &input)
{
	m_right = g_vRight;
	m_position = m_position;
	m_look = g_vLook;
	m_up = g_vUp;

	POINT ptCurrentMousePosit;
	ptCurrentMousePosit.x = input.currentMousePosition.x;
    ptCurrentMousePosit.y = input.currentMousePosition.y;

	//Rotation matrix used to tranformer the world
	D3DXMATRIX matRotation;

	if(input.isLButtonDown)
    {
		int nXDiff = (ptCurrentMousePosit.x - m_lastMousePosition.x);
        int nYDiff = (ptCurrentMousePosit.y - m_lastMousePosition.y);
        
        if(nYDiff != 0)
		{
			//First calculate the radian according the distance
			//Then rotate the radian and get the rotation matrix
			float xAngle = D3DXToRadian((float)nYDiff / 6.0f);
			//m_rotation.x += xAngle;
			m_target->Pitch(xAngle);
			D3DXMatrixRotationAxis(&matRotation, &m_right, D3DXToRadian((float)nYDiff / 6.0f));
			D3DXVec3TransformCoord(&m_look, &m_look, &matRotation);
			D3DXVec3TransformCoord(&m_up, &m_up, &matRotation);			
		}

		if(nXDiff != 0)
		{
			float yAngle = D3DXToRadian((float)nXDiff / 6.0f);
			//m_rotation.y += yAngle;
			m_target->Yaw(yAngle);
			D3DXMatrixRotationAxis( &matRotation, &D3DXVECTOR3(0,1,0), D3DXToRadian((float)nXDiff / 6.0f) );		
			D3DXVec3TransformCoord( &m_look, &m_look, &matRotation );
			D3DXVec3TransformCoord( &m_up, &m_up, &matRotation );
		}
    }

	//Store current position for next process
    m_lastMousePosition.x = ptCurrentMousePosit.x;
    m_lastMousePosition.y = ptCurrentMousePosit.y;

	//Get ket input
	unsigned char keys[256];
	GetKeyboardState(keys);
    
    D3DXVECTOR3 tmpLook  = m_look;
	D3DXVECTOR3 tmpRight = m_right;

	//Move camera position according the direction and distance(time * speed)

	// Up Arrow Key - View moves forward
	if( keys['W'] & 0x80 )
		m_position -= (tmpLook * -20) * input.elpasedTime;

	// Down Arrow Key - View moves backward
	if( keys['S'] & 0x80 )
		m_position += (tmpLook * -20) * input.elpasedTime;

	// Left Arrow Key - View side-steps or strafes to the left
	if( keys['A'] & 0x80 )
		m_position -= (tmpRight * 20) * input.elpasedTime;

	// Right Arrow Key - View side-steps or strafes to the right
	if( keys['D'] & 0x80 )
		m_position += (tmpRight * 20) * input.elpasedTime;

	//TODO 随地形运动
	//Wandering space constaint
	//Above the terrain 
	if(m_position.y < -28.0f)	m_position.y = -28.0f;

	////Inside the map
	//if(m_position.x <  mapMinX)	m_position.x = mapMinX;
	//if(m_position.x >  mapMaxX)	m_position.x = mapMaxX;
	//if(m_position.z <  mapMinZ)	m_position.z = mapMinZ;
	//if(m_position.z >  mapMaxZ)	m_position.z = mapMaxZ;

	m_target->SetPosition(m_position);

	g_vEye = m_position;
	g_vLook = m_look;
	g_vRight = m_right;
	g_vUp = m_up;
}