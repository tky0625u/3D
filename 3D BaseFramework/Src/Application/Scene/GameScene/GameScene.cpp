#include "GameScene.h"
#include"../../main.h"
#include"../SceneManager.h"

//オブジェクトマネジャ
#include"../../GameObject/ObjectManager.h"

void GameScene::Event()
{
	m_ObjManager->DeleteEnemyList();
	Application::Instance().m_log.AddLog("m_EnemyList=%d\n", m_ObjManager->GetEnemyList().size());
}

void GameScene::Init()
{
	//オブジェクトマネジャ
	m_ObjManager = std::make_shared<ObjectManager>();
	m_ObjManager->SetObjectParam();
	m_ObjManager->SetPlayerParam();
	m_ObjManager->SetEnemyParam("Stage1");
}