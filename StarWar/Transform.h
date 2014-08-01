#ifndef _STARWAR_TRANSFORM_H_
#define _STARWAR_TRANSFORM_H_

#include "DXEngine.h"

class CTransform
{
private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_scale;
	D3DXVECTOR3 m_rotation;	

	D3DXVECTOR3 m_lastParentsPosition;
	D3DXVECTOR3 m_lastParentsScale;
	D3DXVECTOR3 m_lastParentsRotation;	

	CTransform *m_parents;	

public:
	CTransform(const D3DXVECTOR3 &position = D3DXVECTOR3(0, 0, 0), 
	 const D3DXVECTOR3 &scale = D3DXVECTOR3(1, 1, 1),
	 const D3DXVECTOR3 &rotation = D3DXVECTOR3(0, 0, 0)) 
	: m_position(position), m_scale(scale), m_rotation(rotation), m_parents(NULL) {};	

	void UpdateMatrix();

	void Roll(float angle)	{m_rotation.z += angle;};
	void Pitch(float angle)	{m_rotation.x += angle;};
	void Yaw(float angle)	{m_rotation.y += angle;};
	void Rotate(D3DXVECTOR3 &rotation)	{m_rotation += rotation;};
	void Translate(D3DXVECTOR3 &direction)	{m_position += direction;};
	void Scale(D3DXVECTOR3 &scale)	{m_scale.x *= scale.x;	m_scale.y *= scale.y;	m_scale.z *= scale.z;};

	D3DXVECTOR3 GetPosition()	{return m_position;};
	D3DXVECTOR3 GetRotation()	{return m_rotation;};
	D3DXVECTOR3 GetScale()	{return m_scale;};
	CTransform *GetParents()	{return m_parents;};

	void SetPosition(D3DXVECTOR3 &position)	{m_position = position;};
	void SetScale(D3DXVECTOR3 &scale)	{m_scale = scale;};
	void SetRotation(D3DXVECTOR3 &rotation)	{m_rotation = rotation;};
	void SetParents(CTransform *parents);
};

#endif