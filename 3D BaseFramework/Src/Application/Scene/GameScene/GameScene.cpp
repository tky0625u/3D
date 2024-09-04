#include "GameScene.h"
#include"../SceneManager.h"

#include"../tinygltf/json.hpp"
#include<fstream>
#include<sstream>

//プレイヤー
#include"../../GameObject/Character/Player/Player.h"
//敵
#include"../../GameObject/Character/Enemy/Bone/Bone.h"
//カメラ
#include"../../GameObject/Camera/TPSCamera/TPSCamera.h"
//床
#include"../../GameObject/StageObject/Floor/Floor.h"
//空
#include"../../GameObject/SkyBox/SkyBox.h"
//ステージ
#include"../../GameObject/Stage/Stage.h"
//武器
#include"../../GameObject/Weapon/Weapon.h"
//オブジェクトマネジャ
#include"../../GameObject/ObjectManager.h"

void GameScene::Event()
{

}

void GameScene::Init()
{
	//オブジェクトマネジャ
	std::shared_ptr<ObjectManager> obj = std::make_shared<ObjectManager>();

	//プレイヤー
	std::shared_ptr<Player> player = std::make_shared<Player>(); 
	player->Init();
	
	//武器
	std::shared_ptr<Weapon> weapon = std::make_shared<Weapon>();
	weapon->SetTarget(player);

	//敵
	std::shared_ptr<Bone> bone = std::make_shared<Bone>();
	bone->Init();
	bone->SetPlayer(player);

	//カメラ
	std::shared_ptr<TPSCamera> camera = std::make_shared<TPSCamera>();
	m_objList.push_back(camera);
	//KdShaderManager::Instance().WorkAmbientController().SetFogEnable(false, true);
	//KdShaderManager::Instance().WorkAmbientController().SetheightFog({ 0.0f,0.0f,0.0f }, 10.0f, 200.0f, 30.0f);

	//空
	std::shared_ptr<SkyBox> skyBox = std::make_shared<SkyBox>();

	//ステージ
	std::shared_ptr<Stage> stage = std::make_shared<Stage>();
	//obj->SetObjectParam("Stage1", "Stage", stage);
	obj->SetObjectParam("Stage1", "Stage", stage);

	//情報セット
	player->SetCamera(camera);
	m_player = player;
	skyBox->SetTarget(player);
	camera->SetTarget(player);

}