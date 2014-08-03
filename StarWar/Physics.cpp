#include "Physics.h"

CPhysics::CPhysics(std::list<CGameNode*> listRootGameNodes)
{
	for(int i = 0; i < GameNodeType::LENGTH; ++i)
	{
		std::vector<bool> row;
		for(int j = 0; j < GameNodeType::LENGTH; ++j)
		{
			row.push_back(false);
		}
		m_layerMatrix.push_back(row);
	}

	m_layerMatrix[GameNodeType::HUMAN][GameNodeType::PLATFORM] = true;
	m_layerMatrix[GameNodeType::HUMAN][GameNodeType::ZERG] = true;
	m_layerMatrix[GameNodeType::HUMAN][GameNodeType::ZERG_TELEPORT] = true;
	m_layerMatrix[GameNodeType::HUMAN][GameNodeType::MINE] = true;
	m_layerMatrix[GameNodeType::HUMAN][GameNodeType::AIRPLANE] = true;
	
	m_layerMatrix[GameNodeType::AIRPLANE][GameNodeType::PLATFORM] = true;
	m_layerMatrix[GameNodeType::AIRPLANE][GameNodeType::MINE] = true;

	m_layerMatrix[GameNodeType::MISSILE][GameNodeType::ZERG] = true;
	m_layerMatrix[GameNodeType::MISSILE][GameNodeType::ZERG_TELEPORT] = true;

	m_layerMatrix[GameNodeType::ZERG][GameNodeType::HUMAN] = true;
	m_layerMatrix[GameNodeType::ZERG][GameNodeType::MINE] = true;

	for(auto node : listRootGameNodes)
	{
		m_collisionGameNodes[node->GetType()].push_back(node);
	}
}

void CPhysics::CollisionCheck()
{
	for(int i = 0; i < GameNodeType::LENGTH; ++i)
	{

		for(auto checking : m_collisionGameNodes[i])
		{
			if(!checking->IsEnablePhysics())
			{
				continue;
			}

			for(int j = 0; j <GameNodeType::LENGTH; ++j)
			{
				if(m_layerMatrix[i][j])
				{
					for(auto checked : m_collisionGameNodes[j])
					{
						if(Collider::IsCollision(checking->GetCollider(), checked->GetCollider(), checking->GetTransform()->GetWorldPosition(), checked->GetTransform()->GetWorldPosition()))
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