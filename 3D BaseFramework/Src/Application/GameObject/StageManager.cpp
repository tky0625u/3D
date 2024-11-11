#include "StageManager.h"
#include"../Scene/SceneManager.h"
#include"ObjectManager.h"
#include"Character/Enemy/EnemyBase.h"
void StageManager::Load()
{
	SceneManager::Instance().GetObjectManager()->SceneCheck();
	SceneManager::Instance().GetObjectManager()->SetGameCameraParam();
	SceneManager::Instance().GetObjectManager()->SetObjectParam();
	SceneManager::Instance().GetObjectManager()->SetPlayerParam();
	SceneManager::Instance().GetObjectManager()->SetEnemyParam("Asset/Json/Game/Enemy/Stage1.json");
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
		m_IsWaveMax = false;
		m_nowStage++;
		m_MaxWave = 0;
		m_nowWave = 0;
		SceneManager::Instance().GetObjectManager()->SetStageParam();
	}
}

void StageManager::WaveCheck()
{
	if (SceneManager::Instance().GetEnemyList().size() > 0)return;

	if (m_MaxWave == m_nowWave)m_IsWaveMax = true;
	else
	{
		m_nowWave++;
		std::string _filePath = ("Asset/Json/Game/Enemy/Stage") + (std::to_string(m_nowStage)) + (".json");
		SceneManager::Instance().GetObjectManager()->SetEnemyParam(_filePath);
	}
}