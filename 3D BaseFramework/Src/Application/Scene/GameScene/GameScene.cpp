#include "GameScene.h"
#include"../../main.h"
#include"../SceneManager.h"

//オブジェクトマネジャ
#include"../../GameObject/ObjectManager.h"

void GameScene::Event()
{
	ObjectManager::Instance().DeleteEnemyList();
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

void GameScene::PostUpdate()
{
	BaseScene::PostUpdate();
	
	if (ObjectManager::Instance().GetStopTime() > 0)
	{
		ObjectManager::Instance().StopTimeMinus();
	}
}

void GameScene::Init()
{
	//オブジェクトマネジャ
	ObjectManager::Instance().SetObjectParam();
	ObjectManager::Instance().SetPlayerParam();
	ObjectManager::Instance().SetEnemyParam();

	KdEffekseerManager::GetInstance().Create(1280, 720);
}