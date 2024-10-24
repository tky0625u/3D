#include "TitleScene.h"
#include "../SceneManager.h"
#include"../../GameObject/ObjectManager.h"

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
	m_ObjManager = std::make_shared<ObjectManager>();
	m_ObjManager->SceneCheck();
	m_ObjManager->SetTitleParam();
	m_ObjManager->SetGameParam();
	m_ObjManager->SetExitParam();
}
