#ifndef _STARWAR_TRANSFORM_H_
#define _STARWAR_TRANSFORM_H_

#include "DXEngine.h"
#include <list>
#include <math.h>

class CTransform
{
private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_scale;
	D3DXVECTOR3 m_rotation;	

	D3DXMATRIX m_matrixWorld;
	CTransform *m_pParents;
	std::list<CTransform *> m_pChildrenList;

public:
	CTransform(const D3DXVECTOR3 &position = D3DXVECTOR3(0, 0, 0), 
	 const D3DXVECTOR3 &scale = D3DXVECTOR3(1, 1, 1),
	 const D3DXVECTOR3 &rotation = D3DXVECTOR3(0, 0, 0)) 
	 : m_position(position), m_scale(scale), m_rotation(rotation), m_pParents(NULL){};	
	~CTransform();
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
	CTransform *GetParents()	{return m_pParents;};
	const D3DXMATRIX &GetWorldMatrix()	{return m_matrixWorld;};

	void SetPosition(D3DXVECTOR3 &position)	{m_position = position;};
	void SetScale(D3DXVECTOR3 &scale)	{m_scale = scale;};
	void SetRotation(D3DXVECTOR3 &rotation)	{m_rotation = rotation;};

	void SetParents(CTransform *parents);

	void AddChild(CTransform *child);
	void RemoveChild(CTransform *child);

	static D3DXVECTOR3 QuaternionToEuler(D3DXQUATERNION &q)
	{
		D3DXVECTOR3 ret;
		double test = q.x*q.y + q.z*q.w;
		if (test > 0.499) { // singularity at north pole
			ret.y = 2 * atan2(q.x,q.w);
			ret.z = 1.57079632679489661923;
			ret.x = 0;
			return ret;
		}
		if (test < -0.499) { // singularity at south pole
			ret.y = -2 * atan2(q.x,q.w);
			ret.z = -1.57079632679489661923;
			ret.x = 0;
			return ret;
		}
		double sqx = q.x*q.x;
		double sqy = q.y*q.y;
		double sqz = q.z*q.z;
		ret.y = atan2(2*q.y*q.w-2*q.x*q.z , 1 - 2*sqy - 2*sqz);
		ret.z = asin(2*test);
		ret.x = atan2(2*q.x*q.w-2*q.y*q.z , 1 - 2*sqx - 2*sqz);
		return ret;
  };
};

#endif