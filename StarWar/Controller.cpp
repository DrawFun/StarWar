#include "Controller.h"
#include "Mine.h"

//extern D3DXVECTOR3	g_vEye;  // Look Vector
//extern D3DXVECTOR3	g_vLook;  // Look Vector
//extern D3DXVECTOR3	g_vUp;      // Up Vector
//extern D3DXVECTOR3	g_vRight;   // Right Vector
extern CMine *mine;

CController::CController(CGameNode *target)
{
	assert(target != NULL);
	m_target = target;
	m_position = target->GetPosition();
	m_rotation = target->GetRotation();

	AdjustTrasform();
}

void CController::Control(const ControllerInput &input)
{	
	float moveSpeed = m_target->GetMoveSpeed();
	float xAngle = 0, yAngle = 0;
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
			xAngle = D3DXToRadian((float)nYDiff / 6.0f);
			//m_target->Pitch(xAngle);
			D3DXMatrixRotationAxis(&matRotation, &m_right, xAngle);
			D3DXVec3TransformCoord(&m_look, &m_look, &matRotation);
			D3DXVec3TransformCoord(&m_up, &m_up, &matRotation);	
		}

		if(nXDiff != 0)
		{
			yAngle = D3DXToRadian((float)nXDiff / 6.0f);
			//m_target->Yaw(yAngle);
			D3DXMatrixRotationAxis( &matRotation, &D3DXVECTOR3(0,1,0), yAngle);		
			D3DXVec3TransformCoord( &m_look, &m_look, &matRotation );
			D3DXVec3TransformCoord( &m_up, &m_up, &matRotation );
		}
    }

	//Store current position for next process
    m_lastMousePosition.x = ptCurrentMousePosit.x;
    m_lastMousePosition.y = ptCurrentMousePosit.y;

	//Move camera position according the direction and distance(time * speed)

	// Up Arrow Key - View moves forward
	if( input.keys['W'] & 0x80 )
		m_position -= (m_look * -moveSpeed) * input.elpasedTime;

	// Down Arrow Key - View moves backward
	if( input.keys['S'] & 0x80 )
		m_position += (m_look * -moveSpeed) * input.elpasedTime;

	// Left Arrow Key - View side-steps or strafes to the left
	if( input.keys['A'] & 0x80 )
		m_position -= (m_right * moveSpeed) * input.elpasedTime;

	// Right Arrow Key - View side-steps or strafes to the right
	if( input.keys['D'] & 0x80 )
		m_position += (m_right * moveSpeed) * input.elpasedTime;

	if(!Collider::IsCollision(m_target->GetCollider(), mine->GetCollider(), m_position, mine->GetPosition()))
	{
		m_target->SetPosition(m_position);
	}
	else
	{
		m_position = m_target->GetPosition();
	}

	m_rotation += D3DXVECTOR3(xAngle, yAngle, 0);
	Util::Clip(-CAMERA_PITCH_LIMITATION, CAMERA_PITCH_LIMITATION, m_rotation.x);
	m_target->SetRotation(m_rotation);

	AdjustTrasform();
}

void CController::AdjustTrasform()
{
	D3DXMATRIX matrixRotationX, matrixRotationY, matrixRotationZ, matrixRotation;

	m_look = D3DXVECTOR3(0, 0, 1); 
	m_right = D3DXVECTOR3(1, 0, 0);
	m_up = D3DXVECTOR3(0, 1, 0);

	D3DXVECTOR3 rotation = m_target->GetRotation();
	D3DXMatrixRotationX(&matrixRotationX, rotation.x); 
	D3DXMatrixRotationY(&matrixRotationY, rotation.y); 
	D3DXMatrixRotationZ(&matrixRotationZ, rotation.z); 
	
	matrixRotation = matrixRotationX * matrixRotationY *  matrixRotationZ;
	
	D3DXVec3TransformCoord(&m_look, &m_look, &matrixRotation);
	D3DXVec3TransformCoord(&m_right, &m_right, &matrixRotation);
	D3DXVec3TransformCoord(&m_up, &m_up, &matrixRotation);
}