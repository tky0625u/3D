#include "ObjectManager.h"
#include"../Scene/SceneManager.h"
#include"../tinygltf/json.hpp"
#include"Character/CharacterBase.h"
#include<fstream>
#include<sstream>
//
//void ObjectManager::RegisterObj(std::shared_ptr<KdGameObject> Object,const UINT16& a_ClassId)
//{
//	auto Obj = m_ObjectMap.find(a_ClassId);
//
//	if (Obj == m_ObjectMap.end())
//	{
//		std::weak_ptr<KdGameObject> _obj = Object;
//		m_ObjectMap[a_ClassId].push_back(Object);
//	}
//}

void ObjectManager::SetObjectParam(std::string _StageNUM, std::string _ObjectName, std::shared_ptr<KdGameObject> _Object)
{
	//jsonファイル
	std::string fileName = "Json/GameObject.json";

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& stage : _json[_StageNUM]["Object"])
	{
				//座標
				Math::Vector3 _pos = Math::Vector3::Zero;
				_pos.x = stage["PosX"];
				int a = 0;
				//_pos.y = obj["PosY"];
				//_pos.z = obj["PosZ"];

				////大きさ
				//float _size = 1.0f;
				//_size = obj["Size"];

				////角度
				//float _angleY = 0.0f;
				//_angleY = obj["Angle"];

				//_Object->SetPos(_pos);
				//_Object->SetScale(_size);
				//_Object->SetAngleY(_angleY);

				SceneManager::Instance().AddObject(_Object);
	}
}