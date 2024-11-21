#include "GameScene_GameOver.h"
#include"../GameScene.h"

#include"../../SceneManager.h"

void GameScene_GameOver::Event()
{
	if (SceneManager::Instance().GetBlackAlphaFlg())
	{
		if (SceneManager::Instance().GetBlackAlpha() >= 1.0f)
		{
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
		}
		return;
	}
	else
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			SceneManager::Instance().BlackAlphaChange(0.01f, true);
		}
	}
}

void GameScene_GameOver::ChangeState()
{
}
