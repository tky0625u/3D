﻿#include "GameScene.h"
#include"../../main.h"
#include"../SceneManager.h"

//オブジェクトマネジャ
#include"../../GameObject/ObjectManager.h"

void GameScene::Event()
{
	ObjectManager::Instance().DeleteEnemyList();
	if(ObjectManager::Instance().GetEnemyList().size()==0 && !ObjectManager::Instance().IsWaveMax())ObjectManager::Instance().SetEnemyParam();

	KdShaderManager::Instance().WorkAmbientController().SetDirLight(Math::Vector3{ 0.0f,-1.0f,0.0f }, Math::Vector3{ 1.5f,1.5f,1.3f });
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
	ObjectManager::Instance().SetCameraParam();
	ObjectManager::Instance().SetObjectParam();
	ObjectManager::Instance().SetPlayerParam();
	ObjectManager::Instance().SetEnemyParam("Json/Enemy/Stage1.json");

	KdEffekseerManager::GetInstance().Create(1280, 720);
}