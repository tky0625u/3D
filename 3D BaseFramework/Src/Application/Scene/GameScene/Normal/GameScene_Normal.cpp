#include "GameScene_Normal.h"
#include"../GameScene.h"

#include"../../SceneManager.h"
#include"../../../GameObject/ObjectManager.h"
#include"../../../GameObject/StageManager.h"
#include"../../../GameObject/Character/Player/Player.h"

void GameScene_Normal::Event()
{
	if (SceneManager::Instance().GetBlackAlphaFlg())
	{
		if (SceneManager::Instance().GetBlackAlpha() >= 1.0f)
		{
			m_target.lock()->GetStageManager()->NextStage();
		}
		return;
	}

	m_target.lock()->GetObjectManager()->DeleteEnemyList();

	if (SceneManager::Instance().GetEnemyList().size() == 0)m_target.lock()->GetStageManager()->WaveCheck();

	if (SceneManager::Instance().GetEnemyList().size() == 0 )
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			SceneManager::Instance().BlackAlphaChange(0.01f, true);
		}
	}
}

void GameScene_Normal::ChangeState()
{
	if (m_target.lock()->GetPlayer()->GetParam().Hp <= 0)
	{
		GameOver();
		return;
	}
	else if (m_target.lock()->GetStageManager()->GetnowStage() == m_target.lock()->GetStageManager()->GetMaxStage() && SceneManager::Instance().GetEnemyList().size() == 0)
	{
		Clear();
		return;
	}
}
