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
		m_ObjManager->SetEnemyParam("Stage1");
	}

	m_ObjManager->DeleteEnemyList();

	//デバッグ用
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