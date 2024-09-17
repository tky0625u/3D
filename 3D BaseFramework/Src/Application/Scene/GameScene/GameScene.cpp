#include "GameScene.h"
#include"../../main.h"
#include"../SceneManager.h"

//オブジェクトマネジャ
#include"../../GameObject/ObjectManager.h"

void GameScene::Event()
{
	ObjectManager::Instance().DeleteEnemyList();
	Application::Instance().m_log.AddLog("m_EnemyList=%d\n", ObjectManager::Instance().GetEnemyList().size());
}

void GameScene::Init()
{
	//オブジェクトマネジャ
	ObjectManager::Instance().SetObjectParam();
	ObjectManager::Instance().SetPlayerParam();
	ObjectManager::Instance().SetEnemyParam("Stage1");
}