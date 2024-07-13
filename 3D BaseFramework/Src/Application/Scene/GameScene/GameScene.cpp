#include "GameScene.h"
#include"../SceneManager.h"


//プレイヤー
#include"../../GameObject/Character/Player/Player.h"
//カメラ
#include"../../GameObject/Camera/FPSCamera/FPSCamera.h"
#include"../../GameObject/Camera/TPSCamera/TPSCamera.h"
//壁
#include"../../GameObject/StageObject/Wall/Wall.h"
//床
#include"../../GameObject/StageObject/Floor/Floor.h"

void GameScene::Event()
{
}

void GameScene::Init()
{
	//床
	m_floorModel = std::make_shared<KdModelData>();
	m_floorModel->Load("Asset/Models/StageObject/Floor/Floor.gltf");

	//壁
	m_wallModel = std::make_shared<KdModelData>();
	m_wallModel->Load("Asset/Models/StageObject/Wall/Wall.gltf");

	Load(1);
}

void GameScene::Load(int StageNumber)
{
	std::string filePath;

	switch (StageNumber)
	{
	case 1:
		filePath = ("CSV/Stage/Stage1/Stage1.csv");
		break;
	case 2:

		break;
	case 3:

		break;
	case 4:

		break;
	case 5:

		break;
	case 6:

		break;
	case 7:

		break;
	case 8:

		break;
	case 9:

		break;
	default:
		break;
	}

	//オブジェクト配置
	std::ifstream ifs(filePath);

	if (!ifs.is_open())return;

	std::string lineString;
	int z = 0;  //Z軸
	int x = 0;  //X軸

	std::shared_ptr<Player> player = std::make_shared<Player>(); //プレイヤー

	//カメラ
	std::shared_ptr<TPSCamera> camera = std::make_shared<TPSCamera>();
	//std::shared_ptr<FPSCamera> camera = std::make_shared<FPSCamera>();

	//情報セット
	player->SetCamera(camera);
	m_player = player;
	camera->SetTarget(player);

	std::shared_ptr<Floor> floor;  //床
	std::shared_ptr<Wall>  wall;   //壁

	while (getline(ifs, lineString))
	{
		std::istringstream iss(lineString);
		std::string commaString;
		std::vector<int> tmpData;

		while (getline(iss, commaString, ','))
		{
			int commaData = stoi(commaString);

			switch (commaData)
			{
			case ObjectType::FloorType:  //床
				floor = std::make_shared<Floor>();
				floor->SetPos(Math::Vector3{ float(m_ObjDistans * x),0.0f,float(m_ObjDistans * -z) });
				floor->SetModel(m_floorModel);
				floor->Init();
				m_objList.push_back(floor);
				break;
			case ObjectType::WallType:  //壁
				wall = std::make_shared<Wall>();
				wall->SetPos(Math::Vector3{ float(m_ObjDistans * x),0.0f,float(m_ObjDistans * -z) });
				wall->SetModel(m_wallModel);
				wall->Init();
				m_objList.push_back(wall);
				break;
			case ObjectType::PlayerType:  //プレイヤー
				//床　※地面に穴を開けないため
				floor = std::make_shared<Floor>();
				floor->SetPos(Math::Vector3{ float(m_ObjDistans * x),0.0f,float(m_ObjDistans * -z) });
				floor->SetModel(m_floorModel);
				floor->Init();
				m_objList.push_back(floor);

				//プレイヤー
				player->SetPos(Math::Vector3{ float(m_ObjDistans * x),1.0f,float(m_ObjDistans * -z) });
				m_objList.push_back(player);
				m_objList.push_back(camera);
				break;
			default:
				break;
			}
			x++;
		}
		x = 0;
		z++;
	}

	ifs.close();
}
