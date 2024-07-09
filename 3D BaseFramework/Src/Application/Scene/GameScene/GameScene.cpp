#include "GameScene.h"
#include"../SceneManager.h"

//プレイヤー
#include"../../GameObject/Character/Player/Player.h"
//ステージ
#include"../../GameObject/StageObject/StageManager/StageManager.h"
//カメラ
#include"../../GameObject/Camera/TPSCamera/TPSCamera.h"

void GameScene::Event()
{
}

void GameScene::Init()
{
	//プレイヤー
	std::shared_ptr<Player> player = std::make_shared<Player>();
	m_objList.push_back(player);

	//ステージ
	std::shared_ptr<StageManager> stage = std::make_shared<StageManager>();
	stage->Load(1);
	m_objList.push_back(stage);

	//カメラ
	std::shared_ptr<TPSCamera> camera = std::make_shared<TPSCamera>();
	m_objList.push_back(camera);

	//情報セット
	player->SetCamera(camera);
	camera->SetTarget(player);
}
