//#include "StageManager.h"
//#include"../Scene/SceneManager.h"
//#include"ObjectManager.h"
//#include"Character/Enemy/EnemyBase.h"
//void StageManager::Clear()
//{
//	if (m_MaxStage == m_nowStage)
//	{
//		SceneManager::Instance().SetNextScene
//		(
//			SceneManager::SceneType::Title
//		);
//	}
//	else
//	{
//		m_ObjManager.lock()->SetStageParam();
//	}
//}
//
//void StageManager::AddEnemy()
//{
//	for (auto& enemy : m_StageEnemyList)
//	{
//		if (!enemy[0].lock()->IsExpired())return;
//	}
//
//	auto enemy = m_StageEnemyList.begin();
//	enemy = m_StageEnemyList.erase(enemy);
//
//	m_ObjManager.lock()->SetEnemyParam()
//}
//
//bool StageManager::IsWaveMax()
//{
//	if (m_nowWave == m_StageEnemyList.size())return true;
//	return false;
//}
