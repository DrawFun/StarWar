#include "GameNode.h"

D3DXVECTOR3 CGameNode::GetPosition()
{
	return m_position;
}

D3DXVECTOR3 CGameNode::GetScale()
{
	return m_scale;
}

D3DXVECTOR3 CGameNode::GetRotation()
{
	return m_rotation;	
}

std::vector<Collider> CGameNode::GetCollider()
{
	return m_colliders;
}

float CGameNode::GetMoveSpeed()
{
	return m_moveSpeed;
}

void CGameNode::SetPosition(D3DXVECTOR3 &position)
{
	m_position = position;
	UpdateMatrix();
}

void CGameNode::SetScale(D3DXVECTOR3 &scale)
{
	m_scale = scale;
	UpdateMatrix();
}

void CGameNode::SetRotation(D3DXVECTOR3 &rotation)
{
	m_rotation = rotation;
	UpdateMatrix();
}

void CGameNode::SetMoveSpeed(float speed)
{
	m_moveSpeed = speed;
}

bool CGameNode::InitPosition(const D3DXVECTOR3 &position, const D3DXVECTOR3 &scale, const D3DXVECTOR3 &rotation)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;
	UpdateMatrix();
	return true;
}

void CGameNode::Roll(float angle)
{
	m_rotation.z += angle;
	UpdateMatrix();
}

void CGameNode::Pitch(float angle)
{
	m_rotation.x += angle;
	UpdateMatrix();
}

void CGameNode::Yaw(float angle)
{
	m_rotation.y += angle;
	UpdateMatrix();
}

void CGameNode::Rotate(D3DXVECTOR3 &rotation)
{
	m_rotation += rotation;
	UpdateMatrix();
}

void CGameNode::Translate(D3DXVECTOR3 &position)
{
	m_position += position;
	UpdateMatrix();
}

void CGameNode::Scale(D3DXVECTOR3 &scale)
{
	m_scale.x *= scale.x;
	m_scale.y *= scale.y;
	m_scale.z *= scale.z;
	UpdateMatrix();
}

void CGameNode::UpdateMatrix()
{
	D3DXMATRIX matrixScale, matrixRotationY, matrixRotationX, matrixTranslation, matrixWorld;
	D3DXMatrixScaling(&matrixScale, m_scale.x, m_scale.y, m_scale.z);

	D3DXMatrixRotationY(&matrixRotationY, m_rotation.y);  
	D3DXMatrixRotationX(&matrixRotationX, m_rotation.x);  
	
	D3DXMatrixTranslation(&matrixTranslation, m_position.x, m_position.y, m_position.z);

	matrixWorld = matrixScale * matrixRotationY * matrixRotationX * matrixTranslation;

	//Transform the world according the generated matrix
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();
	pd3dDevice->SetTransform(D3DTS_WORLD, &matrixWorld);
}