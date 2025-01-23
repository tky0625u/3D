#include "GameScene.h"

#include<mutex>

// メイン
#include"../../main.h"
// シーンマネジャ
#include"../SceneManager.h"
// オブジェクトマネジャ
#include"../../GameObject/ObjectManager.h"
// ステージマネジャ
#include"../../GameObject/StageManager.h"
// ローディング
#include"../../GameObject/Loading/Loading.h"
// ゲームカメラ
#include"../../GameObject/Camera/GameCamera/GameCamera.h"
// エネミーマネジャ
#include"../../GameObject/Character/Enemy/EnemyManager.h"
// プレイヤー
#include"../../GameObject/Character/Player/Player.h"

void GameScene::Event()
{
	if (m_NextState) // 次のステートが決まっていたら
	{
		m_state = m_NextState; // 切り替え
		m_NextState.reset();   // 切り替えが終了したらリセット
	}

	// ステート更新
	m_state->StateUpdate(shared_from_this());

	// エフェクトスピード制御
	for (auto& effect : KdEffekseerManager::GetInstance().GetnowEffectPlayList())
	{
		// スピード計算      再生速度                 スロー値
		float speed = effect->GetSpeed() * m_ObjManager->GetSlow();
		// スピード更新
		KdEffekseerManager::GetInstance().SetSpeed(effect->GetHandle(), speed);
	}
	KdEffekseerManager::GetInstance().Update();
}

// ローディング====================================================================================
bool loop; // ループフラグ
std::shared_ptr<ObjectManager> _ObjManager = nullptr;  // オブジェクトマネジャ
std::shared_ptr<StageManager> _StageManager = nullptr; // ステージマネジャ

// ロード
void StageLoad()
{
	// オブジェクトマネジャ
	_ObjManager = std::make_shared<ObjectManager>();
	// ステージマネジャ
	_StageManager = std::make_shared<StageManager>();
	_StageManager->SetObjectManager(_ObjManager);

	// 現在のシーンの確認
	_ObjManager->SceneCheck();
	// 3Dモデル読み込み
	_ObjManager->ModelLoad();
	// カメラ
	_ObjManager->SetGameCameraParam(_StageManager);
	// オブジェクト
	_ObjManager->SetObjectParam(_StageManager);
	// プレイヤー
	_ObjManager->SetPlayerParam(_StageManager);
	// 敵
	_ObjManager->SetEnemyParam("Asset/Json/Game/Enemy/Stage1.json",_StageManager);

	// ループフラグをOFF
	loop = false;
}

// ローディング画面
void LoadingTime()
{
	// ローディング
	std::shared_ptr<Loading> _load = std::make_shared<Loading>(); // 生成
	_load->Init(); // 初期化

	while (loop) // ループフラグがONの間ローディング画面を描画
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


void GameScene::Init()
{
	loop = true;
	m_EnemyManager = std::make_shared<EnemyManager>();

	// ローディング=============================
	std::thread th_Obj(StageLoad);
	std::thread th_Load(LoadingTime);

	th_Obj.join();
	th_Load.join();
	//==========================================

	// ステージマネジャ
	m_StageManager = _StageManager;
	// オブジェクトマネジャ
	m_ObjManager = _ObjManager;

	// ステート
	std::shared_ptr<Normal> _normal = std::make_shared<Normal>();
	m_state = _normal;

	// 環境光
	KdShaderManager::Instance().WorkAmbientController().SetDirLight(Math::Vector3{ 0.5f,-1.0f,0.5f }, Math::Vector3{ 1.0f,1.0,0.8f });

	// カーソル非表示
	ShowCursor(false);
	// エフェクト生成
	KdEffekseerManager::GetInstance().Create(1280, 720);
	// BGM
	KdAudioManager::Instance().Play("Asset/Sound/Game/BGM/orchestral_mission.WAV", 0.1f, true);

	// 画面を明るく
	SceneManager::Instance().BlackAlphaChange(0.01f, false);
}

// デバッグウィンドウ
void GameScene::DebugObject()
{
	m_ObjManager->DebugObject(m_StageManager);
}

// ステート更新
void GameScene::StateBase::StateUpdate(std::shared_ptr<GameScene> owner)
{
	switch (owner->m_flow)
	{
	case Flow::EnterType:
		Enter(owner);
		break;
	case Flow::UpdateType:
		Update(owner);
		break;
	case Flow::ExitType:
		Exit(owner);
		break;
	}
}

// Normal==========================================================================================
void GameScene::Normal::Enter(std::shared_ptr<GameScene> owner)
{
}

void GameScene::Normal::Update(std::shared_ptr<GameScene> owner)
{
	// 次のステージ読み込み
	if (SceneManager::Instance().GetBlackAlphaFlg() && owner->m_player->GetActionType() == Player::Action::TeleportType && SceneManager::Instance().GetBlackAlpha() >= 1.0f)
	{
		owner->m_StageManager->NextStage();
		return;
	}

	// 敵リスト削除
	owner->m_ObjManager->DeleteEnemyList();

	// ウェーブ確認
	if (SceneManager::Instance().GetEnemyList().size() == 0)owner->m_StageManager->WaveCheck();

	// ステート切り替え
	ChangeState(owner);
}

void GameScene::Normal::Exit(std::shared_ptr<GameScene> owner)
{
}

void GameScene::Normal::ChangeState(std::shared_ptr<GameScene> owner)
{
	// プレイヤーのHPが0になったらゲームオーバー
	if (owner->m_player->GetParam().Hp <= 0)
	{
		std::shared_ptr<GameOver> _over = std::make_shared<GameOver>();
		owner->m_NextState = _over;
		owner->m_flow = GameScene::Flow::EnterType;
		return;
	}
	// 最終ステージをクリアしたらクリア
	else if (owner->m_StageManager->GetnowStage() == owner->m_StageManager->GetMaxStage() && SceneManager::Instance().GetEnemyList().size() == 0)
	{
		std::shared_ptr<Clear> _clear = std::make_shared<Clear>();
		owner->m_NextState = _clear;
		owner->m_flow = GameScene::Flow::EnterType;
		return;
	}
}
//=================================================================================================


// GameOver========================================================================================
void GameScene::GameOver::Enter(std::shared_ptr<GameScene> owner)
{
	// ゲームオーバーUI表示
	owner->m_ObjManager->SetGameStateParam(false);
	owner->m_flow = GameScene::Flow::UpdateType;
	return;
}

void GameScene::GameOver::Update(std::shared_ptr<GameScene> owner)
{
	// 画面暗転フラグがONだったら
	if (SceneManager::Instance().GetBlackAlphaFlg())
	{
		// 画面が暗くなったら
		if (SceneManager::Instance().GetBlackAlpha() >= 1.0f)
		{
			// タイトルに戻る
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
		}
		return;
	}
	else
	{
		// 左クリックを押したら暗転開始
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			SceneManager::Instance().BlackAlphaChange(0.01f, true);
		}
	}

	ChangeState(owner);
}

//=================================================================================================


// Clear===========================================================================================
void GameScene::Clear::Enter(std::shared_ptr<GameScene> owner)
{
	// ゲームクリアUI表示
	owner->m_ObjManager->SetGameStateParam(true);
	// BGM
	KdAudioManager::Instance().StopAllSound();
	KdAudioManager::Instance().Play("Asset/Sound/Game/BGM/遥かなる大地_The_distant_TERRA_.wav", 0.1f, true);
	owner->m_flow = GameScene::Flow::UpdateType;
}

void GameScene::Clear::Update(std::shared_ptr<GameScene> owner)
{
	// 画面暗転フラグがONだったら
	if (SceneManager::Instance().GetBlackAlphaFlg())
	{
		// 画面が暗くなったら
		if (SceneManager::Instance().GetBlackAlpha() >= 1.0f)
		{
			// タイトルに戻る
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
		}
		return;
	}
	else
	{
		// 左クリックを押したら暗転開始
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			SceneManager::Instance().BlackAlphaChange(0.01f, true);
		}
	}

	ChangeState(owner);
}

//=================================================================================================