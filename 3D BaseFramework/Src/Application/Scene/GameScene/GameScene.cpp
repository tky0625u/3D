#include "GameScene.h"
#include"../SceneManager.h"

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

void GameScene::Event()
{

}

void GameScene::Init()
{
	//プレイヤー
	std::shared_ptr<Player> player = std::make_shared<Player>(); 
	player->Init();
	m_objList.push_back(player);

	//武器
	std::shared_ptr<Weapon> weapon = std::make_shared<Weapon>();
	weapon->SetTarget(player);
	m_objList.push_back(weapon);

	//敵
	std::shared_ptr<Bone> bone = std::make_shared<Bone>();
	bone->Init();
	bone->SetPlayer(player);
	m_objList.push_back(bone);

	//カメラ
	std::shared_ptr<TPSCamera> camera = std::make_shared<TPSCamera>();
	m_objList.push_back(camera);
	//KdShaderManager::Instance().WorkAmbientController().SetFogEnable(false, true);
	//KdShaderManager::Instance().WorkAmbientController().SetheightFog({ 0.0f,0.0f,0.0f }, 10.0f, 200.0f, 30.0f);

	//空
	std::shared_ptr<SkyBox> skyBox = std::make_shared<SkyBox>();
	m_objList.push_back(skyBox);

	//ステージ
	std::shared_ptr<Stage> stage = std::make_shared<Stage>();
	m_objList.push_back(stage);

	//情報セット
	player->SetCamera(camera);
	m_player = player;
	skyBox->SetTarget(player);
	camera->SetTarget(player);



	//Load(1, 0, 0);
	//MapLoad("CSV/Map/Map.csv");
}

void GameScene::MapLoad(std::string _filePath)
{
	std::ifstream ifs(_filePath);

	if (!ifs.is_open())return;

	std::vector<int>              WideList;
	std::vector<std::vector<int>> MapList;

	std::string lineString;
	int Z = 0;
	while (getline(ifs, lineString))
	{
		std::istringstream iss(lineString);
		std::string commaString;

		while (getline(iss, commaString, ','))
		{
			int Data = std::stoi(commaString);
			WideList.push_back(Data);
		}
		Z++;
		MapList.push_back(WideList);
		WideList.clear();
	}

	for (unsigned int z = 0; z < MapList.size(); ++z)
	{
		for (unsigned int x = 0; x < MapList[z].size(); ++x)
		{
			Load(MapList[z][x],z,x);
		}
	}

	ifs.close();
}

void GameScene::Load(int StageNumber,int Z, int X)
{
	std::string filePath;

	switch (StageNumber)
	{
	case 1:
		filePath = ("CSV/Stage/Stage1.csv");
		break;
	case 2:
		filePath = ("CSV/Stage/Stage2.csv");
		break;
	case 3:
		filePath = ("CSV/Stage/Stage3.csv");
		break;
	case 4:
		filePath = ("CSV/Stage/Stage4.csv");
		break;
	case 5:
		filePath = ("CSV/Stage/Stage5.csv");
		break;
	case 6:
		filePath = ("CSV/Stage/Stage6.csv");
		break;
	case 7:
		filePath = ("CSV/Stage/Stage7.csv");
		break;
	case 8:
		filePath = ("CSV/Stage/Stage8.csv");
		break;
	case 9:
		filePath = ("CSV/Stage/Stage9.csv");
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

	std::shared_ptr<Floor> floor;  //床
	std::shared_ptr<KdModelData> floorModel = std::make_shared<KdModelData>();
	floorModel->Load("Asset/Models/StageObject/Floor/Floor.gltf");

	float _StageDistaceX = m_StageDistans * float(X);
	float _StageDistaceZ = m_StageDistans * float(Z);

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
				floor->SetPos(Math::Vector3{ float(m_ObjDistans * x) + _StageDistaceX,0.0f,(float(m_ObjDistans * z) + _StageDistaceZ) * -1 });
				floor->SetModel(floorModel);
				floor->Init();
				m_objList.push_back(floor);
				break;
			case ObjectType::PlayerType:  //プレイヤー
				//床　※地面に穴を開けないため
				floor = std::make_shared<Floor>();
				floor->SetPos(Math::Vector3{ float(m_ObjDistans * x) + _StageDistaceX,0.0f,(float(m_ObjDistans * z) + _StageDistaceZ) * -1 });
				floor->SetModel(floorModel);
				floor->Init();
				m_objList.push_back(floor);

				//プレイヤー
				if (m_player.expired() == false)
				{
					m_player.lock()->SetPos(Math::Vector3{ float(m_ObjDistans * x) + _StageDistaceX,0.0f,(float(m_ObjDistans * z) + _StageDistaceZ) * -1 });
				}
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
