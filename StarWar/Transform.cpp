#include "Transform.h"

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
		m_matrixWorld = m_pParents->GetWorldMatrix() * m_matrixWorld;
		D3DXQUATERNION quaternion;
		D3DXMatrixDecompose(&m_scale, &quaternion, &m_position, &m_matrixWorld);
		m_rotation = QuaternionToEuler(quaternion);
	}

	for(auto childTransform : m_pChildrenList)
	{
		childTransform->UpdateMatrix();
	}
}

void CTransform::SetParents(CTransform *parents)
{
	m_pParents = parents; 
	//m_position -= parents->GetPosition();
}

void CTransform::AddChild(CTransform *child)
{
	m_pChildrenList.push_back(child);
	child->SetParents(this);
}
	
void CTransform::RemoveChild(CTransform *child)
{
	m_pChildrenList.remove(child);
	child->SetParents(NULL);
};