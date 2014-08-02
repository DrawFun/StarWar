#include "GameNode.h"

std::vector<Collider> CGameNode::GetCollider()
{
	return m_colliders;
}

GameNodeType CGameNode::GetType()
{
	return m_type;
}

bool CGameNode::InitTransform(const CTransform &transform)
{
	m_transform = transform;
	return true;
}

CTransform* CGameNode::GetTransform()
{
	return &m_transform;
}