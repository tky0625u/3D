#include "EnemyManager.h"

void EnemyManager::DeleteEnemyList()
{
	auto enemy = m_EnemyList.begin();

	while (enemy != m_EnemyList.end())
	{
		if (enemy->expired())
		{
			enemy = m_EnemyList.erase(enemy);
		}
		else
		{
			++enemy;
		}
	}
}
