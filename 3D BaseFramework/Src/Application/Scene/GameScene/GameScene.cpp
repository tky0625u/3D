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
	ObjectManager::Instance().SetPlayerParam();
	ObjectManager::Instance().SetWeaponParam();
	ObjectManager::Instance().SetEnemyParam("Stage1");
	ObjectManager::Instance().SetObjectParam();
}