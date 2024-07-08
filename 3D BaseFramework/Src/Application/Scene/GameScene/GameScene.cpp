#include "GameScene.h"
#include"../SceneManager.h"

//ステージ
#include"../../GameObject/StageObject/StageManager/StageManager.h"
//カメラ
#include"../../GameObject/Camera/TPSCamera/TPSCamera.h"

void GameScene::Event()
{
}

void GameScene::Init()
{
	//ステージ
	std::shared_ptr<StageManager> stage = std::make_shared<StageManager>();
	stage->Load(1);
	m_objList.push_back(stage);

	//カメラ
	std::shared_ptr<TPSCamera> camera = std::make_shared<TPSCamera>();
	m_objList.push_back(camera);
}
