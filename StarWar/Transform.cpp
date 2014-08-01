#include "Transform.h"

void CTransform::UpdateMatrix()
{
	if(m_parents != NULL)
	{
		m_position += (m_parents->GetPosition() - m_lastParentsPosition);
		m_rotation += (m_parents->GetRotation() - m_lastParentsRotation);
		
		m_lastParentsPosition = m_parents->GetPosition();
		m_lastParentsRotation = m_parents->GetRotation();
		m_lastParentsScale = m_parents->GetScale();
	}
	D3DXMATRIX matrixScale, matrixRotation, matrixTranslation, matrixWorld;
	D3DXMatrixScaling(&matrixScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&matrixRotation, m_rotation.y , m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&matrixTranslation, m_position.x, m_position.y, m_position.z);

	matrixWorld = matrixScale * matrixRotation * matrixTranslation;

	//Transform the world according the generated matrix
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();
	pd3dDevice->SetTransform(D3DTS_WORLD, &matrixWorld);	
}


void CTransform::SetParents(CTransform *parents)
{
	m_parents = parents; 
	m_lastParentsPosition = parents->GetPosition();
	m_lastParentsRotation = parents->GetRotation();
	m_lastParentsScale = parents->GetScale();
}