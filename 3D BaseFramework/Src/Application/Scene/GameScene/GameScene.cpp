#include "GameScene.h"
#include"../../main.h"
#include"../SceneManager.h"

#include<mutex>

//オブジェクトマネジャ
#include"../../GameObject/ObjectManager.h"
#include"../../GameObject/StageManager.h"
#include"../../GameObject/Loading/Loading.h"
#include"../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include"../../GameObject/Character/Enemy/EnemyManager.h"

void GameScene::Event()
{
	m_ObjManager->DeleteEnemyList();
	m_StageManager->WaveCheck();

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

	SceneManager::Instance().SetObjectManager(_ObjManager);
	SceneManager::Instance().SetStageManager(_StageManager);

	_StageManager->Load();

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

	ShowCursor(false);
	KdEffekseerManager::GetInstance().Create(1280, 720);
	KdAudioManager::Instance().Play("Asset/Sound/Game/BGM/orchestral_mission.WAV", 0.01f, true);

	SceneManager::Instance().BlackAlphaChange(0.01f);
}
