#include "ObjectManager.h"

#include<iostream>
#include<fstream>
#include<sstream>

#include"Character/CharacterBase.h"

//プレイヤー
#include"Character/Player/Player.h"
//ステージ
#include"StageObject/StageManager/StageManager.h"
//カメラ
#include"Camera/TPSCamera/TPSCamera.h"
#include"Camera/FPSCamera/FPSCamera.h"
//壁
#include"StageObject/Wall/Wall.h"
//床
#include"StageObject/Floor/Floor.h"

void ObjectManager::PreUpdate()
{
	for (auto& obj : m_ObjList)
	{
		if (!obj->IsExpired())obj->PreUpdate();
	}
}

void ObjectManager::Update()
{
	for (auto& obj : m_ObjList)
	{
		if (!obj->IsExpired())obj->Update();
	}
}

void ObjectManager::PostUpdate()
{
	for (auto& obj : m_ObjList)
	{
		if (!obj->IsExpired())obj->PostUpdate();
	}

	//リスト削除
	auto it = m_ObjList.begin();

	while (it != m_ObjList.end())
	{
		const bool Expired = (*it)->IsExpired();

		if (Expired)
		{
			it = m_ObjList.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void ObjectManager::GenerateDepthMapFromLight()
{
	for (auto& obj : m_ObjList)
	{
		if (!obj->IsExpired())
		{
			obj->GenerateDepthMapFromLight();
		}
	}
}

void ObjectManager::PreDraw()
{
	for (auto& obj : m_ObjList)
	{
		if (!obj->IsExpired())
		{
			obj->PreDraw();
		}
	}
}

void ObjectManager::DrawLit()
{
	for (auto& obj : m_ObjList)
	{
		if (!obj->IsExpired())
		{
			obj->DrawLit();
		}
	}
}

void ObjectManager::DrawUnLit()
{
	for (auto& obj : m_ObjList)
	{
		if (!obj->IsExpired())
		{
			obj->DrawUnLit();
		}
	}
}

void ObjectManager::DrawBright()
{
	for (auto& obj : m_ObjList)
	{
		if (!obj->IsExpired())
		{
			obj->DrawBright();
		}
	}
}

void ObjectManager::DrawSprite()
{
	for (auto& obj : m_ObjList)
	{
		if (!obj->IsExpired())
		{
			obj->DrawSprite();
		}
	}
}

void ObjectManager::Init()
{
	//プレイヤー
	std::shared_ptr<Player> player = std::make_shared<Player>();
	m_ObjList.push_back(player);

	//ステージ
	std::shared_ptr<StageManager> stage = std::make_shared<StageManager>();
	m_ObjList.push_back(stage);

	//カメラ
	std::shared_ptr<TPSCamera> camera = std::make_shared<TPSCamera>();
	//std::shared_ptr<FPSCamera> camera = std::make_shared<FPSCamera>();
	m_ObjList.push_back(camera);

	//情報セット
	player->SetCamera(camera);
	m_player = player;
	camera->SetTarget(player);
}

void ObjectManager::Load(int StageNumber)
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
				floor->Init();
				m_ObjList.push_back(floor);
				break;
			case ObjectType::WallType:  //壁
				wall = std::make_shared<Wall>();
				wall->SetPos(Math::Vector3{ float(m_ObjDistans * x),0.0f,float(m_ObjDistans * -z) });
				wall->Init();
				m_ObjList.push_back(wall);
				break;
			case ObjectType::PlayerType:  //プレイヤー
				if (m_player.expired() == false)
				{
					m_player.lock()->SetPos(Math::Vector3{ float(m_ObjDistans * x),1.0f,float(m_ObjDistans * -z) });
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
}
