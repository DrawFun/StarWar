#include "Physics.h"

//碰撞系统
CPhysics::CPhysics(std::list<CGameNode*> listRootGameNodes)
{

	//定义类似U3D的碰撞矩阵
	//由于游戏物件种类少，因此没有采用U3D中的分层的矩阵，直接细化到各种物件
	for(int i = 0; i < GameNodeType::LENGTH; ++i)
	{
		std::vector<bool> row;
		for(int j = 0; j < GameNodeType::LENGTH; ++j)
		{
			row.push_back(false);
		}
		m_layerMatrix.push_back(row);
	}

	//定义是否判定碰撞
	//单向定义
	m_layerMatrix[GameNodeType::HUMAN][GameNodeType::PLATFORM] = true;
	m_layerMatrix[GameNodeType::HUMAN][GameNodeType::ZERG] = true;
	m_layerMatrix[GameNodeType::HUMAN][GameNodeType::ZERG_TELEPORT] = true;
	m_layerMatrix[GameNodeType::HUMAN][GameNodeType::MINE] = true;
	m_layerMatrix[GameNodeType::HUMAN][GameNodeType::AIRPLANE] = true;
	
	m_layerMatrix[GameNodeType::AIRPLANE][GameNodeType::PLATFORM] = true;
	m_layerMatrix[GameNodeType::AIRPLANE][GameNodeType::MINE] = true;

	m_layerMatrix[GameNodeType::MISSILE][GameNodeType::ZERG] = true;
	m_layerMatrix[GameNodeType::MISSILE][GameNodeType::ZERG_TELEPORT] = true;

	m_layerMatrix[GameNodeType::ZERG][GameNodeType::MINE] = true;

	//根据类别存储为链表
	for(auto node : listRootGameNodes)
	{
		m_collisionGameNodes[node->GetType()].push_back(node);
	}
}

void CPhysics::CollisionCheck()
{
	//检查每一类
	for(int i = 0; i < GameNodeType::LENGTH; ++i)
	{
		//同类中链表的每一个
		for(auto checking : m_collisionGameNodes[i])
		{			
			if(!checking->IsEnablePhysics())
			{
				continue;
			}

			//双层循环，检查其他类
			for(int j = 0; j <GameNodeType::LENGTH; ++j)
			{
				//矩阵判定是否需要检查，由于矩阵是单向定义，因此不存在双层循环的检查开销
				//以此简化编码
				if(m_layerMatrix[i][j])
				{
					//同类链表遍历每个
					for(auto checked : m_collisionGameNodes[j])
					{
						if(checked->IsEnablePhysics() &&
							Collider::IsCollision(checking->GetCollider(), checked->GetCollider(), checking->GetTransform()->GetWorldPosition(), checked->GetTransform()->GetWorldPosition()))
						{
							checking->CollidingCallback(checked);
							checked->CollidedCallback(checking);
						}
					}
				}
			}
		}

	}
}