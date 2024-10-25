#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../main.h"
#include"../../GameObject/ObjectManager.h"
#include"../../GameObject/UI/Title/Cursor/Cursor.h"
#include"../../GameObject/UI/Title/Exit/Exit.h"
#include"../../GameObject/UI/Title/Game/Game.h"
#include"../../GameObject/Camera/TitleCamera/TitleCamera.h"

void TitleScene::Update()
{
	BaseScene::Update();
	KdEffekseerManager::GetInstance().Update();
}

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
	m_ObjManager->SetPlayerParam();
	m_ObjManager->SetTitleParam();
	m_ObjManager->SetGameParam();
	m_ObjManager->SetExitParam();
	m_ObjManager->SetTitleGuideParam();
	m_ObjManager->SetCursorParam();
	m_ObjManager->SetObjectParam();
	KdEffekseerManager::GetInstance().Create(1280, 720);
	Math::Vector3 _pos = m_ObjManager->GetTitleCamera().lock()->GetPos();
	_pos.z -= 1.0f;
	_pos.y -= 5.0f;
	KdEffekseerManager::GetInstance().Play("Smoke.efkefc", _pos, 1.0f, 0.5f, true);
	m_ObjManager->GetTitleCamera().lock()->WorkCamera()->SetFocus(8, 5, 600);

	KdAudioManager::Instance().Play("Asset/Sound/Title/BGM/Formidable-Enemy.WAV", 0.01f, true);
}
