#include "GameScene.h"
#include"../../main.h"
#include"../SceneManager.h"

//オブジェクトマネジャ
#include"../../GameObject/ObjectManager.h"

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

void GameScene::Init()
{
	//オブジェクトマネジャ
	m_ObjManager = std::make_shared<ObjectManager>();

	m_ObjManager->SceneCheck();
	m_ObjManager->SetGameCameraParam();
	m_ObjManager->SetObjectParam();
	m_ObjManager->SetPlayerParam();
	m_ObjManager->SetEnemyParam("Json/Game/Enemy/Stage1.json");

	KdEffekseerManager::GetInstance().Create(1280, 720);
}