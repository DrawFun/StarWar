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

//析构，处理同父子节点的关系
//在父节点中移除自己
//标记子节点父亲为空，更新子节点Transform
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

//更新当前Transform
void CTransform::UpdateMatrix()
{
	D3DXMATRIX matrixScale, matrixRotation, matrixTranslation;
	D3DXMatrixScaling(&matrixScale, m_scale.x, m_scale.y, m_scale.z);
	//D3DXMatrixRotationQuaternion(&matrixRotation, &m_rotation);
	D3DXMatrixRotationYawPitchRoll(&matrixRotation, m_rotation.y , m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&matrixTranslation, m_position.x, m_position.y, m_position.z);

	m_matrixWorld = matrixScale * matrixRotation * matrixTranslation;

	//如果挂载在其他节点上则根据父节点世界矩阵重算自己的世界矩阵
	//并Decompose出Position，Rotation和Scale
	if(m_pParents != NULL)
	{
		m_matrixWorld =  m_matrixWorld * m_pParents->GetWorldMatrix();
		D3DXQUATERNION quaternion;
		D3DXMatrixDecompose(&m_worldScale, &quaternion, &m_worldPosition, &m_matrixWorld);
		m_qWorldRotation = quaternion;
		m_worldRotation = QuaternionToEuler(quaternion);
	}

	//递归更新
	for(auto childTransform : m_pChildrenList)
	{
		childTransform->UpdateMatrix();
	}
}

//重设当前的Transform的父节点，递归更新其所有子节点
void CTransform::ResetTransform(CTransform *parents)
{
	m_pParents = parents;
	if(m_pParents != NULL)
	{
		//Reset position
		m_position = m_worldPosition - m_pParents->GetWorldPosition();

		//Reset rotation
		D3DXQUATERNION qLocal, qParents, qChild;
		qChild = this->GetWorldRotation();	
		qParents = parents->GetWorldRotation();
		D3DXQuaternionNormalize(&qChild, &qChild);
		D3DXQuaternionNormalize(&qParents, &qParents);
		D3DXQuaternionInverse(&qParents, &qParents);
		qLocal = qChild * qParents;
		D3DXQuaternionNormalize(&qLocal, &qLocal);
		m_rotation = QuaternionToEuler(qLocal);
		m_qWorldRotation = qLocal;

		//Reset scale
		m_scale.x = m_worldScale.x / m_pParents->GetWorldScale().x;
		m_scale.y = m_worldScale.y / m_pParents->GetWorldScale().y;
		m_scale.z = m_worldScale.z / m_pParents->GetWorldScale().z;
	}
	UpdateMatrix();
}

//当前的Transform增加子节点
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

//当前的Transform移除子节点
void CTransform::RemoveChild(CTransform *child)
{
	m_pChildrenList.remove(child);
};