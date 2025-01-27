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
		float speed = effect->GetSpeed() * SceneManager::Instance().GetSlow();
		// スピード更新
		KdEffekseerManager::GetInstance().SetSpeed(effect->GetHandle(), speed);
	}
	KdEffekseerManager::GetInstance().Update();
}


// ローディング====================================================================================
bool Gloop; // ループフラグ
std::shared_ptr<ObjectManager> _GObjManager = nullptr;  // オブジェクトマネジャ
std::shared_ptr<StageManager> _GStageManager = nullptr; // ステージマネジャ

// ゲーム開始時のロード
void StageLoad()
{
	// オブジェクトマネジャ
	_GObjManager = std::make_shared<ObjectManager>();
	// ステージマネジャ
	_GStageManager = std::make_shared<StageManager>();

	// 現在のシーンの確認
	_GObjManager->SceneCheck();
	// 3Dモデル読み込み
	_GObjManager->ModelLoad();
	// カメラ
	_GStageManager->SetCamera(_GObjManager->SetGameCameraParam());
	// オブジェクト
	// 地面
	_GObjManager->SetGroundParam(_GStageManager->GetnowStage());
	// 魔法陣の台
	_GObjManager->SetCircleParam();
	// 魔法陣
	_GStageManager->SetMagicPolygon(_GObjManager->SetMagicPolygonParam());
	// 空
	_GObjManager->SetSkyBoxParam();
	// 壁
	_GObjManager->SetWallParam();
	// プレイヤー
	_GStageManager->SetPlayer(_GObjManager->SetPlayerParam(_GStageManager));
	// 敵
	_GStageManager->SetMaxWave(_GObjManager->SetEnemyParam("Asset/Json/Game/Enemy/Stage1.json"));
	// ステージ数
	_GStageManager->SetMaxStage();

	// ループフラグをOFF
	Gloop = false;
}

// ステージ移動時のロード
void NextStage()
{
	_GStageManager->NextStage();
	_GStageManager->SetMaxWave(_GObjManager->CreateStage(_GStageManager->GetnowStage()));

	Gloop = false;
}

// ローディング画面
void GameLoadingTime()
{
	// ローディング
	std::shared_ptr<Loading> _load = std::make_shared<Loading>(); // 生成
	_load->GameChange(); // ゲーム用ローディング画面
	_load->Init(); // 初期化

	while (Gloop) // ループフラグがONの間ローディング画面を描画
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
	Gloop = true;
	m_EnemyManager = std::make_shared<EnemyManager>();

	// ローディング=============================
	std::thread th_Obj(StageLoad);
	std::thread th_Load(GameLoadingTime);

	th_Obj.join();
	th_Load.join();
	//==========================================

	// ステージマネジャ
	m_StageManager = _GStageManager;
	// オブジェクトマネジャ
	m_ObjManager = _GObjManager;

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
	if(m_StageManager)m_StageManager->DebugStage();
	m_ObjManager->DebugObject();
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
		_GStageManager = owner->m_StageManager;
		_GObjManager = owner->m_ObjManager;

		Gloop = true;

		std::thread th_Obj(NextStage);
		std::thread th_Load(GameLoadingTime);

		th_Obj.join();
		th_Load.join();

		owner->m_StageManager = _GStageManager;
		owner->m_ObjManager = _GObjManager;

		// 画面を明るく
		SceneManager::Instance().BlackAlphaChange(0.01f, false);
		return;
	}

	// 敵リスト削除
	owner->m_ObjManager->DeleteEnemyList();

	// ウェーブ確認
	if (SceneManager::Instance().GetEnemyList().size() == 0)
	{
		bool _waveFlg = owner->m_StageManager->WaveCheck();

		if (_waveFlg)
		{
			if (owner->m_StageManager->GetnowStage() != owner->m_StageManager->GetMaxStage())
			{
				owner->m_StageManager->NextStageLiberation();
			}
		}
		else
		{
			owner->m_ObjManager->SetEnemyParam(("Asset/Json/Game/Enemy/Stage")+(std::to_string(owner->m_StageManager->GetnowStage())) + (".json"), owner->m_StageManager->GetnowWave());
		}
	}

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
	// スロー切り替え
	if (SceneManager::Instance().GetSlowFlg())SceneManager::Instance().SlowChange();
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
	// スロー切り替え
	if (SceneManager::Instance().GetSlowFlg())SceneManager::Instance().SlowChange();
	// ゲームクリア準備
	owner->m_StageManager->GameClear();
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