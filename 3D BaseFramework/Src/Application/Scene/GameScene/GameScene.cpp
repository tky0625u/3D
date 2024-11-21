#include "GameScene.h"
#include"../../main.h"
#include"../SceneManager.h"

#include<mutex>

//オブジェクトマネジャ
#include"../../GameObject/ObjectManager.h"
#include"../../GameObject/StageManager.h"
#include"../../GameObject/Loading/Loading.h"
#include"../../GameObject/Character/Enemy/EnemyManager.h"
#include"../../GameObject/Character/Player/Player.h"
#include"../../GameObject/Camera/GameCamera/GameCamera.h"
#include"../../GameObject/Camera/GameCamera/GameCamera_ConText.h"

void GameScene::Event()
{
	if (m_NextState)
	{
		m_state = m_NextState;
		m_NextState.reset();
	}

	switch (m_flow)
	{
	case Flow::EnterType:
		m_state->Enter(this);
		break;
	case Flow::UpdateType:
		m_state->Update(this);
		break;
	case Flow::ExitType:
		m_state->Exit(this);
		break;
	}
	m_state->Update(this);

	KdShaderManager::Instance().WorkAmbientController().SetDirLight(Math::Vector3{ 0.5f,-1.0f,0.5f }, Math::Vector3{ 1.5f,1.5f,1.3f });
}

void GameScene::Update()
{
	BaseScene::Update();
	for (auto& effect : KdEffekseerManager::GetInstance().GetnowEffectPlayList())
	{
		float speed = effect->GetSpeed() * m_ObjManager->GetSlow();
		KdEffekseerManager::GetInstance().SetSpeed(effect->GetHandle(), speed);
	}
	KdEffekseerManager::GetInstance().Update();
}

void GameScene::PostUpdate()
{
	BaseScene::PostUpdate();
}

bool loop;
std::shared_ptr<ObjectManager> _ObjManager = nullptr;
std::shared_ptr<StageManager> _StageManager = nullptr;

void StageLoad()
{
	//オブジェクトマネジャ
	_ObjManager = std::make_shared<ObjectManager>();
	_StageManager = std::make_shared<StageManager>();
	_StageManager->SetObjectManager(_ObjManager);

	_ObjManager->SceneCheck();
	_ObjManager->SetGameCameraParam();
	_ObjManager->SetObjectParam(_StageManager);
	_ObjManager->SetPlayerParam(_StageManager);
	_ObjManager->SetEnemyParam("Asset/Json/Game/Enemy/Stage1.json",_StageManager);

	loop = false;
}

void LoadingTime()
{
	std::shared_ptr<Loading> _load = std::make_shared<Loading>();
	_load->Init();

	while (loop)
	{
		_load->Update();

		Application::Instance().KdBeginDraw(false);
		{
			_load->DrawSprite();

		}
		Application::Instance().KdPostDraw();
	}
}

void GameScene::Init()
{
	loop = true;
	m_EnemyManager = std::make_shared<EnemyManager>();

	std::thread th_Obj(StageLoad);
	std::thread th_Load(LoadingTime);

	th_Obj.join();
	th_Load.join();

	m_StageManager = _StageManager;
	m_ObjManager = _ObjManager;

	std::shared_ptr<Normal> _normal = std::make_shared<Normal>();
	m_state = _normal;

	ShowCursor(false);
	KdEffekseerManager::GetInstance().Create(1280, 720);
	KdAudioManager::Instance().Play("Asset/Sound/Game/BGM/orchestral_mission.WAV", 0.01f, true);

	SceneManager::Instance().BlackAlphaChange(0.01f, false);
}

void GameScene::DebugObject()
{
	m_ObjManager->DebugObject(m_StageManager);
}

void GameScene::Normal::Enter(GameScene* owner)
{
}

// Normal==========================================================================================
void GameScene::Normal::Update(GameScene* owner)
{
	if (SceneManager::Instance().GetBlackAlphaFlg())
	{
		if (SceneManager::Instance().GetBlackAlpha() >= 1.0f)
		{
			owner->m_StageManager->NextStage();
		}
		return;
	}

	owner->m_ObjManager->DeleteEnemyList();

	if (SceneManager::Instance().GetEnemyList().size() == 0)owner->m_StageManager->WaveCheck();

	if (SceneManager::Instance().GetEnemyList().size() == 0)
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			SceneManager::Instance().BlackAlphaChange(0.01f, true);
		}
	}

	ChangeState(owner);
}

void GameScene::Normal::Exit(GameScene* owner)
{
}

void GameScene::Normal::ChangeState(GameScene* owner)
{
	if (owner->m_player->GetParam().Hp <= 0)
	{
		std::shared_ptr<GameOver> _over = std::make_shared<GameOver>();
		owner->m_NextState = _over;
		owner->m_flow = GameScene::Flow::UpdateType;
		return;
	}
	else if (owner->m_StageManager->GetnowStage() == owner->m_StageManager->GetMaxStage() && SceneManager::Instance().GetEnemyList().size() == 0)
	{
		std::shared_ptr<Clear> _clear = std::make_shared<Clear>();
		owner->m_NextState = _clear;
		owner->m_flow = GameScene::Flow::EnterType;
		return;
	}
}
//=================================================================================================


void GameScene::GameOver::Enter(GameScene* owner)
{
}

// GameOver========================================================================================
void GameScene::GameOver::Update(GameScene* owner)
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

	ChangeState(owner);
}

void GameScene::GameOver::Exit(GameScene* owner)
{
}

void GameScene::GameOver::ChangeState(GameScene* owner)
{
}
//=================================================================================================


void GameScene::Clear::Enter(GameScene* owner)
{
	owner->m_player->GetCamera().lock()->GetConText()->ClearCamera();
	owner->m_flow = GameScene::Flow::UpdateType;
}

// Clear===========================================================================================
void GameScene::Clear::Update(GameScene* owner)
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

	ChangeState(owner);
}

void GameScene::Clear::Exit(GameScene* owner)
{
}

void GameScene::Clear::ChangeState(GameScene* owner)
{
}
//=================================================================================================