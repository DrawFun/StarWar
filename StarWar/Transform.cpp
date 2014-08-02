#include "Transform.h"

CTransform::CTransform(const D3DXVECTOR3 &position, const D3DXVECTOR3 &scale, const D3DXVECTOR3 &rotation) 
{
	m_pParents = NULL;
	m_worldPosition = m_position = position;
	m_worldScale = m_scale = scale;
	m_worldRotation = m_rotation = rotation;
	D3DXQuaternionRotationYawPitchRoll(&m_qWorldRotation, m_worldRotation.y, m_worldRotation.x, m_worldRotation.z);	
	UpdateMatrix();
}	

CTransform::~CTransform()
{
	if(m_pParents != NULL)
	{
		m_pParents->RemoveChild(this);
		for(auto childTransform : m_pChildrenList)
		{
			childTransform->ResetTransform(NULL);
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
		m_qWorldRotation = quaternion;
		m_worldRotation = QuaternionToEuler(quaternion);
	}

	for(auto childTransform : m_pChildrenList)
	{
		childTransform->UpdateMatrix();
	}
}

void CTransform::ResetTransform(CTransform *parents)
{
	m_pParents = parents;
	if(m_pParents != NULL)
	{
		//Reset position
		m_position = m_worldPosition - m_pParents->GetWorldPosition();

		//Reset rotation
		//D3DXQUATERNION qLocal, qParents, qChild;
		//qChild = this->GetWorldRotation();	
		//qParents = parents->GetWorldRotation();
		//D3DXQuaternionNormalize(&qChild, &qChild);
		//D3DXQuaternionNormalize(&qParents, &qParents);
		//D3DXQuaternionInverse(&qParents, &qParents);
		//qLocal = qChild * qParents;
		//D3DXQuaternionNormalize(&qLocal, &qLocal);
		//m_rotation = QuaternionToEuler(qLocal);
		//m_qWorldRotation = qLocal;

		//Reset scale
		m_scale.x = m_worldScale.x / m_pParents->GetWorldScale().x;
		m_scale.y = m_worldScale.y / m_pParents->GetWorldScale().y;
		m_scale.z = m_worldScale.z / m_pParents->GetWorldScale().z;
	}
	UpdateMatrix();
}

void CTransform::AddChild(CTransform *child)
{
	if(child->GetParents() != NULL)
	{
		CTransform *oldParents = child->GetParents();
		oldParents->RemoveChild(child);
	}
	m_pChildrenList.push_back(child);
	
	child->ResetTransform(this);
}
	
void CTransform::RemoveChild(CTransform *child)
{
	m_pChildrenList.remove(child);
};