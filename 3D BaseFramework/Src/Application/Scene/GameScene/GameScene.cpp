#include "GameScene.h"
#include"../SceneManager.h"

//カメラ
#include"../../GameObject/Camera/TPSCamera/TPSCamera.h"

void GameScene::Event()
{
}

void GameScene::Init()
{
	//カメラ
	std::shared_ptr<TPSCamera> camera = std::make_shared<TPSCamera>();
	m_objList.push_back(camera);
}
