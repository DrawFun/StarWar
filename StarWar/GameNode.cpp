#include "GameNode.h"

std::vector<Collider> CGameNode::GetCollider()
{
	return m_colliders;
}

float CGameNode::GetMoveSpeed()
{
	return m_moveSpeed;
}

GameNodeType CGameNode::GetType()
{
	return m_type;
}

void CGameNode::SetMoveSpeed(float speed)
{
	m_moveSpeed = speed;
}

bool CGameNode::InitTransform(const CTransform &transform)
{
	m_transform = transform;
	//m_transform.UpdateMatrix();
	return true;
}

CTransform* CGameNode::GetTransform()
{
	return &m_transform;
}