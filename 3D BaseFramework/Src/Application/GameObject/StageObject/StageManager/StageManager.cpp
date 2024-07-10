#include "StageManager.h"

#include<iostream>
#include<fstream>
#include<sstream>

//壁
#include"../Wall/Wall.h"
//床
#include"../Floor/Floor.h"

void StageManager::PreUpdate()
{
	for (auto& obj : m_StgObjList)
	{
		if (!obj->IsExpired())obj->PreUpdate();
	}
}

void StageManager::Update()
{
	for (auto& obj : m_StgObjList)
	{
		if (!obj->IsExpired())obj->Update();
	}
}

void StageManager::PostUpdate()
{
	for (auto& obj : m_StgObjList)
	{
		if (!obj->IsExpired())obj->PostUpdate();
	}

	//リスト削除
	auto it = m_StgObjList.begin();

	while (it != m_StgObjList.end())
	{
		const bool Expired = (*it)->IsExpired();

		if (Expired)
		{
			it = m_StgObjList.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void StageManager::GenerateDepthMapFromLight()
{
	for (auto& obj : m_StgObjList)
	{
		if (!obj->IsExpired())
		{
			obj->GenerateDepthMapFromLight();
		}
	}
}

void StageManager::PreDraw()
{
	for (auto& obj : m_StgObjList)
	{
		if (!obj->IsExpired())
		{
			obj->PreDraw();
		}
	}
}

void StageManager::DrawLit()
{
	for (auto& obj : m_StgObjList)
	{
		if (!obj->IsExpired())
		{
			obj->DrawLit();
		}
	}
}

void StageManager::DrawUnLit()
{
	for (auto& obj : m_StgObjList)
	{
		if (!obj->IsExpired())
		{
			obj->DrawUnLit();
		}
	}
}

void StageManager::DrawBright()
{
	for (auto& obj : m_StgObjList)
	{
		if (!obj->IsExpired())
		{
			obj->DrawBright();
		}
	}
}

void StageManager::DrawSprite()
{
	for (auto& obj : m_StgObjList)
	{
		if (!obj->IsExpired())
		{
			obj->DrawSprite();
		}
	}
}

void StageManager::Init()
{
}

void StageManager::Load(int StageNumber)
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

	if(!ifs.is_open())return;

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
				m_StgObjList.push_back(floor);
				break;
			case ObjectType::WallType:  //壁
				wall = std::make_shared<Wall>();
				wall->SetPos(Math::Vector3{ float(m_ObjDistans * x),0.0f,float(m_ObjDistans * -z) });
				wall->Init();
				m_StgObjList.push_back(wall);
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
