#include "GameScene.h"
#include"../SceneManager.h"

//オブジェクトマネージャ
#include"../../GameObject/ObjectManager.h"

void GameScene::Event()
{
}

void GameScene::Init()
{
	//オブジェクトマネジャ
	std::shared_ptr<ObjectManager> object = std::make_shared<ObjectManager>();
	object->Load(1);
	m_objList.push_back(object);
}
