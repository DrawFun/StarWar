#include "Transform.h"

CTransform::CTransform(const D3DXVECTOR3 &position, const D3DXVECTOR3 &scale, const D3DXVECTOR3 &rotation) 
{
	m_pParents = NULL;
	m_worldPosition = m_position = position;
	m_worldScale = m_scale = scale;
	m_worldRotation = m_rotation = rotation;
	UpdateMatrix();
}	

CTransform::~CTransform()
{
	//TODO 
	if(m_pParents != NULL)
	{
		m_pParents->RemoveChild(this);
		for(auto childTransform : m_pChildrenList)
		{
			childTransform->SetParents(NULL);
		}
	}
}

void CTransform::UpdateMatrix()
{
	D3DXMATRIX matrixScale, matrixRotation, matrixTranslation;
	D3DXMatrixScaling(&matrixScale, m_scale.x, m_scale.y, m_scale.z);
	//D3DXMatrixRotationQuaternion(&matrixRotation, &m_rotation);
	D3DXMatrixRotationYawPitchRoll(&matrixRotation, m_rotation.y , m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&matrixTranslation, m_position.x, m_position.y, m_position.z);

	m_matrixWorld = matrixScale * matrixRotation * matrixTranslation;

	if(m_pParents != NULL)
	{
		m_matrixWorld =  m_matrixWorld * m_pParents->GetWorldMatrix();
		D3DXQUATERNION quaternion;
		D3DXMatrixDecompose(&m_worldScale, &quaternion, &m_worldPosition, &m_matrixWorld);
		m_worldRotation = QuaternionToEuler(quaternion);
	}

	for(auto childTransform : m_pChildrenList)
	{
		childTransform->UpdateMatrix();
	}
}

void CTransform::SetParents(CTransform *parents)
{
	m_pParents = parents; 
	m_position -= parents->GetPosition();
	UpdateMatrix();
}

void CTransform::AddChild(CTransform *child)
{
	m_pChildrenList.push_back(child);
	child->SetParents(this);
	UpdateMatrix();
}
	
void CTransform::RemoveChild(CTransform *child)
{
	m_pChildrenList.remove(child);
	child->SetParents(NULL);
	UpdateMatrix();
};