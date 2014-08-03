#include "Physics.h"

//��ײϵͳ
CPhysics::CPhysics(std::list<CGameNode*> listRootGameNodes)
{

	//��������U3D����ײ����
	//������Ϸ��������٣����û�в���U3D�еķֲ�ľ���ֱ��ϸ�����������
	for(int i = 0; i < GameNodeType::LENGTH; ++i)
	{
		std::vector<bool> row;
		for(int j = 0; j < GameNodeType::LENGTH; ++j)
		{
			row.push_back(false);
		}
		m_layerMatrix.push_back(row);
	}

	//�����Ƿ��ж���ײ
	//������
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

	//�������洢Ϊ����
	for(auto node : listRootGameNodes)
	{
		m_collisionGameNodes[node->GetType()].push_back(node);
	}
}

void CPhysics::CollisionCheck()
{
	//���ÿһ��
	for(int i = 0; i < GameNodeType::LENGTH; ++i)
	{
		//ͬ���������ÿһ��
		for(auto checking : m_collisionGameNodes[i])
		{			
			if(!checking->IsEnablePhysics())
			{
				continue;
			}

			//˫��ѭ�������������
			for(int j = 0; j <GameNodeType::LENGTH; ++j)
			{
				//�����ж��Ƿ���Ҫ��飬���ھ����ǵ����壬��˲�����˫��ѭ���ļ�鿪��
				//�Դ˼򻯱���
				if(m_layerMatrix[i][j])
				{
					//ͬ���������ÿ��
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