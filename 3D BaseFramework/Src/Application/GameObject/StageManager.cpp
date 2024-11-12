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
void StageManager::NextStage()
{
	m_IsWaveMax = false;
	m_nowStage++;
	m_MaxWave = 0;
	m_nowWave = 0;
	SceneManager::Instance().GetObjectManager()->CreateStage();
}

void StageManager::WaveCheck()
{
	if (m_MaxWave == m_nowWave && !m_IsWaveMax)
	{
		m_IsWaveMax = true;
		if (m_nowStage == m_MaxStage) { SceneManager::Instance().GetObjectManager()->GameClear(); }
		else { SceneManager::Instance().GetObjectManager()->NextStageLiberation(); }
	}
	else
	{
		m_nowWave++;
		std::string _filePath = ("Asset/Json/Game/Enemy/Stage") + (std::to_string(m_nowStage)) + (".json");
		SceneManager::Instance().GetObjectManager()->SetEnemyParam(_filePath);
	}
}