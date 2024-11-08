#include "StageManager.h"
#include"../Scene/SceneManager.h"
#include"ObjectManager.h"
#include"Character/Enemy/EnemyBase.h"
void StageManager::Load()
{
	m_ObjManager.lock()->SceneCheck();
	m_ObjManager.lock()->SetGameCameraParam();
	m_ObjManager.lock()->SetObjectParam();
	m_ObjManager.lock()->SetPlayerParam();
	m_ObjManager.lock()->SetEnemyParam("Asset/Json/Game/Enemy/Stage1.json",shared_from_this());
}
void StageManager::Clear()
{
	if (m_MaxStage == m_nowStage)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
		);
	}
	else
	{
		m_ObjManager.lock()->SetStageParam(shared_from_this());
	}
}

void StageManager::WaveCheck()
{
	for (auto& enemy : m_StageEnemyList)
	{
		if (!enemy[0].lock()->IsExpired())return;
	}

	auto enemy = m_StageEnemyList.begin();
	enemy = m_StageEnemyList.erase(enemy);

	if (m_StageEnemyList.size() == 0)Clear();
	else
	{
		for (auto& _enemy : m_StageEnemyList[0])
		{
			SceneManager::Instance().AddEnemy(_enemy.lock());
		}
	}
}