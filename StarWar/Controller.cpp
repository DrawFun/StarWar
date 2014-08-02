#include "Controller.h"
#include "Mine.h"
#include "Platform.h"

//extern CMine *mine;
//extern CPlatform *platform;

CController::CController(CGameNode *target)
{
	assert(target != NULL);
	m_target = target;
	m_position = target->GetTransform()->GetPosition();
	m_rotation = target->GetTransform()->GetRotation();

	AdjustTransform();
}

void CController::Control(const ControllerInput &input)
{	
	float moveSpeed = m_target->GetMoveSpeed();
	m_position = m_target->GetTransform()->GetPosition();
	m_rotation = m_target->GetTransform()->GetRotation();
	float xAngle = 0, yAngle = 0;
	POINT ptCurrentMousePosit;
	ptCurrentMousePosit.x = input.currentMousePosition.x;
    ptCurrentMousePosit.y = input.currentMousePosition.y;
	D3DXVECTOR3 attemptPosition = m_position;

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
		attemptPosition -= (m_look * -moveSpeed) * input.elpasedTime;

	// Down Arrow Key - View moves backward
	if( input.keys['S'] & 0x80 )
		attemptPosition += (m_look * -moveSpeed) * input.elpasedTime;

	// Left Arrow Key - View side-steps or strafes to the left
	if( input.keys['A'] & 0x80 )
		attemptPosition -= (m_right * moveSpeed) * input.elpasedTime;

	// Right Arrow Key - View side-steps or strafes to the right
	if( input.keys['D'] & 0x80 )
		attemptPosition += (m_right * moveSpeed) * input.elpasedTime;
	Util::Clip(0, 2048, attemptPosition.y);

	//if(Collider::IsCollision(m_target->GetCollider(), mine->GetCollider(), attemptPosition, mine->GetTransform().GetPosition()))
	//{		
	//	m_target->CollidingCallback(mine);
	//	mine->CollidedCallback(m_target);
	//	//m_position = m_target->GetPosition();
	//}
	//else if(Collider::IsCollision(m_target->GetCollider(), platform->GetCollider(), attemptPosition, platform->GetTransform().GetPosition()))
	//{
	//	if(m_target->GetParents() != platform)
	//	{
	//		m_target->CollidingCallback(platform);
	//		platform->CollidedCallback(m_target);
	//	}
	//}
	//else
	//{
	//	m_position = attemptPosition;
	//	m_target->GetTransform().SetPosition(m_position);		
	//}


	//DELETE
	m_position = attemptPosition;
	m_target->GetTransform()->SetPosition(m_position);

	m_rotation += D3DXVECTOR3(xAngle, yAngle, 0);
	Util::Clip(-CAMERA_PITCH_LIMITATION, CAMERA_PITCH_LIMITATION, m_rotation.x);
	m_target->GetTransform()->SetRotation(m_rotation);
		//AllocConsole();//注意检查返回值
		//_cprintf("%f, %f, %f\n", m_position.x, m_position.y, m_position.z);
		//AllocConsole();//注意检查返回值
		//_cprintf("%f, %f, %f\n", m_rotation.x, m_rotation.y, m_rotation.z);
	AdjustTransform();
}

void CController::AdjustTransform()
{
	D3DXMATRIX matrixRotationX, matrixRotationY, matrixRotationZ, matrixRotation;

	m_look = D3DXVECTOR3(0, 0, 1); 
	m_right = D3DXVECTOR3(1, 0, 0);
	m_up = D3DXVECTOR3(0, 1, 0);

	D3DXVECTOR3 rotation = m_target->GetTransform()->GetRotation();
	D3DXMatrixRotationX(&matrixRotationX, rotation.x); 
	D3DXMatrixRotationY(&matrixRotationY, rotation.y); 
	D3DXMatrixRotationZ(&matrixRotationZ, rotation.z); 

	matrixRotation = matrixRotationX * matrixRotationY *  matrixRotationZ;

	D3DXVec3TransformCoord(&m_look, &m_look, &matrixRotation);
	D3DXVec3TransformCoord(&m_right, &m_right, &matrixRotation);
	D3DXVec3TransformCoord(&m_up, &m_up, &matrixRotation);
}