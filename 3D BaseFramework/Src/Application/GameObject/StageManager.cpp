#include "StageManager.h"
#include"../Scene/SceneManager.h"
#include"ObjectManager.h"
#include"Character/Enemy/EnemyBase.h"

void StageManager::NextStage()
{
	m_IsWaveMax = false;
	m_nowStage++;
	m_MaxWave = 0;
	m_nowWave = 0;
	m_ObjectManager.lock()->CreateStage(shared_from_this());
	SceneManager::Instance().BlackAlphaChange(0.01f, false);
}

void StageManager::WaveCheck()
{
	if (m_MaxWave == m_nowWave && !m_IsWaveMax)
	{
		m_IsWaveMax = true;
		if (m_nowStage == m_MaxStage) { m_ObjectManager.lock()->GameClear(); }
		else { m_ObjectManager.lock()->NextStageLiberation(); }
	}
	else
	{
		m_nowWave++;
		std::string _filePath = ("Asset/Json/Game/Enemy/Stage") + (std::to_string(m_nowStage)) + (".json");
		m_ObjectManager.lock()->SetEnemyParam(_filePath, shared_from_this());
	}
}