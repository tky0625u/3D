#include "TitleScene.h"
#include "../SceneManager.h"
#include"../../GameObject/ObjectManager.h"
#include"../../GameObject/UI/Title/Title.h"

void TitleScene::Event()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Game
		);
	}
}

void TitleScene::Init()
{
	ObjectManager::Instance().SceneCheck();
	ObjectManager::Instance().SetTitleParam();
}
