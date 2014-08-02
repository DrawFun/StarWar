#include "TargetedBullet.h"

void CTargetedBullet::MoveToTarget()
{
	if(m_isAlive)
	{
		D3DXVECTOR3 direction = 
			 m_target->GetTransform()->GetWorldPosition() - m_transform.GetWorldPosition();
		
		D3DXVec3Normalize(&direction, &direction);
		m_transform.Translate(direction * m_moveSpeed);
	}
}

void CTargetedBullet::Recycle()
{
	m_enableControl = false;
	m_enablePhysics = false;
	m_enableRender = false;
	m_isControlable = false; 
	m_isFlyable = false;
	m_isAlive = false;	
}