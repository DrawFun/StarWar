//------------------------------------------------------------------------------
//           Name: Camera.cpp
//         Author: Dai Zhuofang
//    Description: Camera controller
//                 
//   Control Keys: W          - View moves forward
//                 S          - View moves backward
//                 A          - View strafes left
//                 D          - View strafes Right
//                 Left Mouse - Perform looking
//                 Mouse      - Look about the scene
//------------------------------------------------------------------------------

#include "Camera.h"

extern D3DXVECTOR3 g_vEye;    // Eye Position
extern D3DXVECTOR3 g_vUp;    // Eye Position

CCamera::CCamera(CGameNode *target, D3DXVECTOR3 &position)
{	
	this->m_target = target;
	m_offsetPosition = position - target->GetPosition();    
}

void CCamera::UpdateViewMatrix()
{
	D3DXMATRIX view;
	D3DXVec3Normalize( &g_vUp, &g_vUp );
	D3DXVECTOR3 up(0,0,1);

	D3DXMATRIX matrixRotationX, matrixRotationY, matrixRotationZ, matrixRotation;

	D3DXVECTOR3 m_look = D3DXVECTOR3(0, 0, 1); 
	D3DXVECTOR3 m_right = D3DXVECTOR3(1, 0, 0);
	D3DXVECTOR3 m_up = D3DXVECTOR3(0, 1, 0);

	D3DXVECTOR3 rotation = m_target->GetRotation();
	
		float xx = rotation.x;
	if(xx>0.3) xx = 0.3;
	if(xx<-0.3) xx = -0.3;
	D3DXMatrixRotationX(&matrixRotationX, xx); 

	D3DXMatrixRotationY(&matrixRotationY, rotation.y); 

	D3DXVECTOR3 m_offsetPosition1 = m_offsetPosition;   
	
	matrixRotation = matrixRotationX * matrixRotationY;
	
	//D3DXVec3TransformCoord(&m_look, &m_look, &matrixRotation);
	//D3DXVec3TransformCoord(&m_right, &m_right, &matrixRotation);
	D3DXVec3TransformCoord(&m_offsetPosition1, &m_offsetPosition1, &matrixRotation);

	D3DXVec3Normalize( &m_offsetPosition1, &m_offsetPosition1 );

	g_vEye = m_target->GetPosition() + m_offsetPosition1 * 50;
	D3DXMatrixLookAtLH(&view, &g_vEye, &m_target->GetPosition(), &m_up);

	//D3DXMatrixIdentity( &view );

	//Re-calculate then normalize the right vector according the look and up directions
	//D3DXVec3Normalize( &g_vLook, &g_vLook );
	//D3DXVec3Cross( &g_vRight, &g_vUp, &g_vLook );
	//D3DXVec3Normalize( &g_vRight, &g_vRight );
	//D3DXVec3Cross( &g_vUp, &g_vLook, &g_vRight );
	//D3DXVec3Normalize( &g_vUp, &g_vUp );

	//view._11 = g_vRight.x;
 //   view._12 = g_vUp.x;
 //   view._13 = g_vLook.x;
	//view._14 = 0.0f;

	//view._21 = g_vRight.y;
 //   view._22 = g_vUp.y;
 //   view._23 = g_vLook.y;
	//view._24 = 0.0f;

	//view._31 = g_vRight.z;
 //   view._32 = g_vUp.z;
 //   view._33 = g_vLook.z;
	//view._34 = 0.0f;

	//view._41 = -D3DXVec3Dot( &g_vEye, &g_vRight );
	//view._42 = -D3DXVec3Dot( &g_vEye, &g_vUp );
	//view._43 = -D3DXVec3Dot( &g_vEye, &g_vLook );
	//view._44 =  1.0f;
	LPDIRECT3DDEVICE9 pd3dDevice = CDXEngine::Instance()->GetDxDevice();
	pd3dDevice->SetTransform( D3DTS_VIEW, &view ); 
}

void CCamera::LateUpdate()
{
	UpdateViewMatrix();
}