#include "GameScene.h"
#include"../../main.h"
#include"../SceneManager.h"

#include<mutex>

//オブジェクトマネジャ
#include"../../GameObject/ObjectManager.h"
#include"../../GameObject/Loading/Loading.h"

void GameScene::Event()
{
	m_ObjManager->DeleteEnemyList();
	if(m_ObjManager->GetEnemyList().size()==0 && !m_ObjManager->IsWaveMax())m_ObjManager->SetEnemyParam();

	KdShaderManager::Instance().WorkAmbientController().SetDirLight(Math::Vector3{ 0.5f,-1.0f,0.5f }, Math::Vector3{ 1.5f,1.5f,1.3f });
}

void GameScene::Update()
{
	if (m_ObjManager->GetStopTime() > 0)return;

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
	
	if (m_ObjManager->GetStopTime() > 0)
	{
		m_ObjManager->StopTimeMinus();
	}
}

bool loop;
std::shared_ptr<ObjectManager> _ObjManager = nullptr;

void ObjLoad()
{
	//オブジェクトマネジャ
	_ObjManager = std::make_shared<ObjectManager>();

	_ObjManager->SceneCheck();
	_ObjManager->SetGameCameraParam();
	_ObjManager->SetObjectParam();
	_ObjManager->SetPlayerParam();
	_ObjManager->SetEnemyParam("Json/Game/Enemy/Stage1.json");

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

	std::thread th_Obj(ObjLoad);
	std::thread th_Load(LoadingTime);

	th_Obj.join();
	th_Load.join();

	m_ObjManager = _ObjManager;
	KdEffekseerManager::GetInstance().Create(1280, 720);
	KdAudioManager::Instance().Play("Asset/Sound/Game/BGM/orchestral_mission.WAV", 0.01f, true);
}
