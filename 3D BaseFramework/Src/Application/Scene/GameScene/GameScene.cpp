#include "GameScene.h"
#include"../../main.h"
#include"../SceneManager.h"

//オブジェクトマネジャ
#include"../../GameObject/ObjectManager.h"

void GameScene::Event()
{
	//デバッグ用
	if (GetAsyncKeyState('E') & 0x8000)
	{
		ObjectManager::Instance().AddBone();
	}

	ObjectManager::Instance().DeleteEnemyList();

	//デバッグ用
	//Application::Instance().m_log.AddLog("m_EnemyList=%d\n", m_ObjManager->GetEnemyList().size());
}

void GameScene::Update()
{
	if (ObjectManager::Instance().GetStopTime() > 0)return;

	BaseScene::Update();
	for (auto& effect : KdEffekseerManager::GetInstance().GetnowEffectPlayList())
	{
		float speed = effect->GetSpeed() * ObjectManager::Instance().GetSlow();
		KdEffekseerManager::GetInstance().SetSpeed(effect->GetHandle(), speed);
	}
	KdEffekseerManager::GetInstance().Update();
}

void GameScene::Init()
{
	//オブジェクトマネジャ
	ObjectManager::Instance().SetObjectParam();
	ObjectManager::Instance().SetPlayerParam();
	ObjectManager::Instance().SetEnemyParam("Stage1");

	KdEffekseerManager::GetInstance().Create(1280, 720);
}