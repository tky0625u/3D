#include "TitleScene.h"

// メイン
#include "../../main.h"
// シーンマネジャ
#include "../SceneManager.h"
// オブジェクトマネジャ
#include"../../GameObject/ObjectManager.h"
// タイトルカメラ
#include"../../GameObject/Camera/TitleCamera/TitleCamera.h"

// デバッグウィンドウ
void TitleScene::DebugObject()
{
	m_ObjManager->DebugObject();
}

void TitleScene::Event()
{
	// エフェクト更新
	KdEffekseerManager::GetInstance().Update();
}

void TitleScene::Init()
{
	// オブジェクトマネジャ生成
	m_ObjManager = std::make_shared<ObjectManager>();
	// 現在のシーンを確認
	m_ObjManager->SceneCheck();
	// タイトルシーン
	m_ObjManager->SetTitleCamera();
	// タイトル
	m_ObjManager->SetTitleParam();
	// GameUI
	m_ObjManager->SetGameParam();
	// ExitUI
	m_ObjManager->SetExitParam();
	// セレクトUI
	m_ObjManager->SetSelectParam();
	// カーソル
	m_ObjManager->SetCursorParam();
	// オブジェクト
	m_ObjManager->SetObjectParam();

	// エフェクト生成
	KdEffekseerManager::GetInstance().Create(1280, 720);

	// 霧
	Math::Vector3 _pos = Math::Vector3::Zero;
	_pos.y = SceneManager::Instance().GetCamera()->GetPos().y - 50.0f;
	// 再生
	KdEffekseerManager::GetInstance().Play("Smoke/Smoke.efkefc", _pos, Math::Vector3{20.0f,1.0f,20.0f}, 0.1f, true);
	
	// ピント
	SceneManager::Instance().GetCamera()->WorkCamera()->SetFocus(8, 5, 600);

	// BGM
	KdAudioManager::Instance().Play("Asset/Sound/Title/BGM/Formidable-Enemy.WAV", 0.1f, true);

	// カーソル非表示
	ShowCursor(false);

	// 画面を明るく
	SceneManager::Instance().BlackAlphaChange(0.01f, false);

}
