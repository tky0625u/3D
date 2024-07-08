#include "StageManager.h"

#include<iostream>
#include<fstream>
#include<sstream>

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
		if (obj->IsExpired())
		{
			obj->GenerateDepthMapFromLight();
		}
	}
}

void StageManager::PreDraw()
{
	for (auto& obj : m_StgObjList)
	{
		if (obj->IsExpired())
		{
			obj->PreDraw();
		}
	}
}

void StageManager::DrawLit()
{
	for (auto& obj : m_StgObjList)
	{
		if (obj->IsExpired())
		{
			obj->DrawLit();
		}
	}
}

void StageManager::DrawUnLit()
{
	for (auto& obj : m_StgObjList)
	{
		if (obj->IsExpired())
		{
			obj->DrawUnLit();
		}
	}
}

void StageManager::DrawBright()
{
	for (auto& obj : m_StgObjList)
	{
		if (obj->IsExpired())
		{
			obj->DrawBright();
		}
	}
}

void StageManager::DrawSprite()
{
	for (auto& obj : m_StgObjList)
	{
		if (obj->IsExpired())
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

	std::ifstream ifs(filePath);

	std::string lineString;
	int z = 0;  //Z軸
	int x = 0;  //X軸
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
			case ObjectType::Wall:

				break;
			case ObjectType::Floor:

				break;
			default:
				break;
			}
			x++;
		}
		z++;
	}
}
