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

void TitleScene::DebugObject()
{
	m_ObjManager->DebugObject();
}

void TitleScene::Event()
{

}

void TitleScene::Init()
{
	m_ObjManager = std::make_shared<ObjectManager>();
	m_ObjManager->SceneCheck();
	m_ObjManager->SetTitleCamera();
	m_ObjManager->SetTitleParam();
	m_ObjManager->SetGameParam();
	m_ObjManager->SetExitParam();
	m_ObjManager->SetSelectParam();
	m_ObjManager->SetCursorParam();
	m_ObjManager->SetObjectParam();
	KdEffekseerManager::GetInstance().Create(1280, 720);
	Math::Vector3 _pos = Math::Vector3::Zero;
	_pos.y = SceneManager::Instance().GetCamera()->GetPos().y - 50.0f;
	KdEffekseerManager::GetInstance().Play("Smoke/Smoke.efkefc", _pos, Math::Vector3{20.0f,1.0f,20.0f}, 0.1f, true);
	SceneManager::Instance().GetCamera()->WorkCamera()->SetFocus(8, 5, 600);

	KdAudioManager::Instance().Play("Asset/Sound/Title/BGM/Formidable-Enemy.WAV", 0.01f, true);

	SceneManager::Instance().BlackAlphaChange(0.01f, false);

	ShowCursor(false);
}
