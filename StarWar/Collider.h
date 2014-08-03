#ifndef _STARWAR_COLLIDER_H_
#define _STARWAR_COLLIDER_H_

#include "Util.h"

//基础的两类碰撞体
enum ColliderType{BOX_COLLIDER = 0, SPHERE_COLLIDER = 1};

class Collider
{
protected:
	ColliderType m_type;
	D3DXVECTOR3 m_pMin;
	D3DXVECTOR3 m_pMax;
	D3DXVECTOR3 m_pCenter;
	float m_radius;

public:
	//盒状碰撞体
	Collider(D3DXVECTOR3 &pMin, D3DXVECTOR3 &pMax) : m_type(BOX_COLLIDER), m_pMin(pMin), m_pMax(pMax)
	{
		m_radius = D3DXVec3Length(&(m_pMax - m_pMin)) / 2;
	};

	//球状碰撞体
	Collider(D3DXVECTOR3 &pCenter, float radius) : m_type(SPHERE_COLLIDER), m_pCenter(pCenter), m_radius(radius){};

	//每个物体可能由多个基础碰撞体组合而成，因此这里用的容器（但项目里都是最逗比的单体，设计理想很丰满，现实很骨感）
	//碰撞体和位置信息，循环遍历进行检测
	static bool IsCollision(std::vector<Collider> colliders1, std::vector<Collider> colliders2, 
		D3DXVECTOR3 position1, D3DXVECTOR3 position2)
	{		
		for(int i = 0; i < colliders1.size(); ++i)
		{
			for(int j = 0; j < colliders2.size(); ++j)
			{
				if(IsCollision(&colliders1[i], &colliders2[j], position1, position2))
				{
					return true;
				}
			}
		}
		return false;
	}

private:
	//盒装碰撞体将两个立方体映射到三个平面上进行检测
	//球状检测半径和球心
	//盒装和球状又偷懒了。。。
	static bool IsCollision(Collider *collider1, Collider *collider2, 
		D3DXVECTOR3 position1, D3DXVECTOR3 position2)
	{
		float centerDistance = D3DXVec3Length(&(position1 - position2));
		if(centerDistance > collider1->m_radius + collider2->m_radius)
		{
			return false;
		}
		if(collider1->m_type == BOX_COLLIDER && collider2->m_type == BOX_COLLIDER)
		{	
			RECT r1, r2, r3, r4, r5, r6;
			D3DXVECTOR3 c1PMin = collider1->m_pMin + position1;
			D3DXVECTOR3 c1PMax = collider1->m_pMax + position1;
			D3DXVECTOR3 c2PMin = collider2->m_pMin + position2;
			D3DXVECTOR3 c2PMax = collider2->m_pMax + position2;
			r1.bottom = c1PMin.x; r1.left = c1PMin.y; r1.top = c1PMax.x; r1.right = c1PMax.y;
			r2.bottom = c2PMin.x; r2.left = c2PMin.y; r2.top = c2PMax.x; r2.right = c2PMax.y;
			r3.bottom = c1PMin.x; r3.left = c1PMin.z; r3.top = c1PMax.x; r3.right = c1PMax.z;
			r4.bottom = c2PMin.x; r4.left = c2PMin.z; r4.top = c2PMax.x; r4.right = c2PMax.z;
			r5.bottom = c1PMin.y; r5.left = c1PMin.z; r5.top = c1PMax.y; r5.right = c1PMax.z;
			r6.bottom = c2PMin.y; r6.left = c2PMin.z; r6.top = c2PMax.y; r6.right = c2PMax.z;
			if(IsRectInsection(r1, r2) && IsRectInsection(r3, r4) && IsRectInsection(r5, r6))
			{
				return true;
			}
		}
		else if(collider1->m_type == SPHERE_COLLIDER && collider2->m_type == SPHERE_COLLIDER)
		{
			return (centerDistance <= collider1->m_radius + collider2->m_radius);			
		}
		else
		{
			return (centerDistance <= collider1->m_radius + collider2->m_radius);
		}

		return false;
	}

	//矩形是否相交
	static inline bool IsRectInsection(RECT r1, RECT r2)
	{				
		if(abs((r1.left+r1.right)/2-(r2.left+r2.right)/2)<((r1.right+r2.right-r1.left-r2.left)/2) && abs((r1.bottom+r1.top)/2-(r2.bottom+r2.top)/2)<((r1.top+r2.top-r1.bottom-r2.bottom)/2))
		{
			return true;
		}
		return false;
	}
};

#endif