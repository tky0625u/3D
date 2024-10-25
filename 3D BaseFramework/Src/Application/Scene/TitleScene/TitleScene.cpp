#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../main.h"
#include"../../GameObject/ObjectManager.h"
#include"../../GameObject/UI/Title/Cursor/Cursor.h"
#include"../../GameObject/UI/Title/Exit/Exit.h"
#include"../../GameObject/UI/Title/Game/Game.h"

void TitleScene::Event()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		if (m_ObjManager->GetCursor().lock()->GetVector2Pos() == m_ObjManager->GetGame().lock()->GetVector2Pos())
		{
			SceneManager::Instance().SetNextScene
			(
				SceneManager::SceneType::Game
			);
		}
		else if (m_ObjManager->GetCursor().lock()->GetVector2Pos() == m_ObjManager->GetExit().lock()->GetVector2Pos())
		{
			Application::Instance().End();
		}
	}
}

void TitleScene::Init()
{
	m_ObjManager = std::make_shared<ObjectManager>();
	m_ObjManager->SceneCheck();
	m_ObjManager->SetTitleCamera();
	m_ObjManager->SetTitleParam();
	m_ObjManager->SetGameParam();
	m_ObjManager->SetExitParam();
	m_ObjManager->SetCursorParam();
	m_ObjManager->SetObjectParam();
}
