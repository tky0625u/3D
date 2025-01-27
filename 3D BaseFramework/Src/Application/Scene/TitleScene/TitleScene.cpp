#include "TitleScene.h"

#include<mutex>

// メイン
#include "../../main.h"
// シーンマネジャ
#include "../SceneManager.h"
// オブジェクトマネジャ
#include"../../GameObject/ObjectManager.h"
// タイトルカメラ
#include"../../GameObject/Camera/TitleCamera/TitleCamera.h"
// ローディング
#include"../../GameObject/Loading/Loading.h"

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

// ローディング====================================================================================
std::shared_ptr<StageManager>  _TStageManager = nullptr;  // ステージマネジャ
std::shared_ptr<ObjectManager> _TObjManager = nullptr; // オブジェクトマネジャ
bool Tloop = true; // ループフラグ

void ObjLoad()
{
	// オブジェクトマネジャ生成
	_TObjManager = std::make_shared<ObjectManager>();
	// 現在のシーンを確認
	_TObjManager->SceneCheck();
	// タイトルシーン
	_TObjManager->SetTitleCamera();
	// タイトル
	_TObjManager->SetTitleParam();
	// GameUI
	_TObjManager->SetGameParam();
	// ExitUI
	_TObjManager->SetExitParam();
	// セレクトUI
	_TObjManager->SetSelectParam();
	// カーソル
	_TObjManager->SetCursorParam();
	// オブジェクト
	_TObjManager->SetSkyBoxParam();
	_TObjManager->SetWallParam();

	Tloop = false;
}

void TitleLoadingTime()
{
	std::shared_ptr<Loading> _load = std::make_shared<Loading>();
	_load->TitleChange(); // タイトル用ローディング画面
	_load->Init();        // 初期化

	while (Tloop)
	{
		// 更新
		_load->Update();

		// 描画
		Application::Instance().KdBeginDraw(false);
		{
			_load->DrawSprite();
		}
		Application::Instance().KdPostDraw();
	}
}

//=================================================================================================

void TitleScene::Init()
{
	std::thread th_Obj(ObjLoad);
	std::thread th_Load(TitleLoadingTime);

	th_Obj.join();
	th_Load.join();

	m_StageManager = _TStageManager;
	m_ObjManager = _TObjManager;

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
