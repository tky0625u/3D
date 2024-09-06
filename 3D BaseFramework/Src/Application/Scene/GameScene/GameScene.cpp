#include "GameScene.h"
#include"../SceneManager.h"

//オブジェクトマネジャ
#include"../../GameObject/ObjectManager.h"

void GameScene::Event()
{

}

void GameScene::Init()
{
	//オブジェクトマネジャ
	std::shared_ptr<ObjectManager> obj = std::make_shared<ObjectManager>();

	obj->SetPlayerParam();
	obj->SetWeaponParam();
	obj->SetEnemyParam("Stage1");
	obj->SetObjectParam();
}