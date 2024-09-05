#include "ObjectManager.h"
#include"../Scene/SceneManager.h"
#include"../tinygltf/json.hpp"
#include"Character/CharacterBase.h"
#include<fstream>
#include<sstream>

//ステージ
#include"../GameObject/Stage/Stage.h"
//スカイボックス
#include"../GameObject/SkyBox/SkyBox.h"
//武器
#include"../GameObject/Weapon/Weapon.h"
//プレイヤー
#include"../GameObject/Character/Player/Player.h"
//カメラ
#include"../GameObject/Camera/TPSCamera/TPSCamera.h"
//敵
#include"../GameObject/Character/Enemy/Bone/Bone.h"

void ObjectManager::SetObjectParam()
{
	//jsonファイル
	std::string fileName = "Json/Object/Object.json";

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& stage : _json)
	{
		Math::Vector3 _pos = Math::Vector3::Zero;
		_pos.x = stage["PosX"];
		_pos.y = stage["PosY"];
		_pos.z = stage["PosZ"];

		float _size = 0.0f;
		_size = stage["Size"];

		float _angleY = 0.0f;
		_angleY = stage["Angle"];

		std::shared_ptr<KdGameObject> obj;
		if (stage["Name"] == "Stage")
		{
			obj = std::make_shared<Stage>();
		}
		if (stage["Name"] == "SkyBox")
		{
			obj = std::make_shared<SkyBox>();
		}

		obj->SetPos(_pos);
		obj->SetScale(_size);
		obj->SetAngleY(_angleY);

		SceneManager::Instance().AddObject(obj);
	}

	ifs.close();
}

void ObjectManager::SetPlayerParam()
{
	//jsonファイル
	std::string fileName = "Json/Player/Player.json";

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& stage : _json)
	{
		Math::Vector3 _pos = Math::Vector3::Zero;
		_pos.x = stage["PosX"];
		_pos.y = stage["PosY"];
		_pos.z = stage["PosZ"];

		float _size = 0.0f;
		_size = stage["Size"];

		float _angleY = 0.0f;
		_angleY = stage["Angle"];

		int _hp = 0;
		_hp = stage["HP"];

		int _atk = 0;
		_atk = stage["ATK"];

		float _speed = 0.0f;
		_speed = stage["Speed"];

		int _stamina = 0;
		_stamina = stage["Stamina"];

		float _atkRange = 0.0f;
		_atkRange = stage["ATKRange"];

		Math::Vector3 _forword = Math::Vector3::Zero;
		_forword.x = stage["ForwordX"];
		_forword.y = stage["ForwordY"];
		_forword.z = stage["ForwordZ"];

		std::shared_ptr<Player> player = std::make_shared<Player>();
		std::shared_ptr<TPSCamera> camera = std::make_shared<TPSCamera>();
		player->SetCamera(camera);
		player->Init();
		player->SetPos(_pos);
		player->SetParam(_hp, _atk, _speed, _stamina, _angleY, _size, _atkRange, _forword);

		SceneManager::Instance().AddObject(player);
		m_player = player;

		camera->SetTarget(player);
		SceneManager::Instance().AddObject(camera);
	}

	ifs.close();
}

void ObjectManager::SetWeaponParam()
{
	//jsonファイル
	std::string fileName = "Json/Weapon/Weapon.json";

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& stage : _json)
	{
		Math::Vector3 _pos = Math::Vector3::Zero;
		_pos.x = stage["PosX"];
		_pos.y = stage["PosY"];
		_pos.z = stage["PosZ"];

		float _size = 0.0f;
		_size = stage["Size"];

		float _angleY = 0.0f;
		_angleY = stage["Angle"];

		std::shared_ptr<Weapon> weapon = std::make_shared<Weapon>();

		weapon->SetPos(_pos);
		weapon->SetScale(_size);
		weapon->SetAngleY(_angleY);
		if (m_player.expired() == false)
		{
			weapon->SetTarget(m_player.lock());
		}

		SceneManager::Instance().AddObject(weapon);
	}

	ifs.close();
}

void ObjectManager::SetEnemyParam(std::string _StageNum)
{
	//jsonファイル
	std::string fileName = "Json/Enemy/Enemy.json";

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& stage : _json[_StageNum])
	{
		Math::Vector3 _pos = Math::Vector3::Zero;
		_pos.x = stage["PosX"];
		_pos.y = stage["PosY"];
		_pos.z = stage["PosZ"];

		float _size = 0.0f;
		_size = stage["Size"];

		float _angleY = 0.0f;
		_angleY = stage["Angle"];

		int _hp = 0;
		_hp = stage["HP"];

		int _atk = 0;
		_atk = stage["ATK"];

		float _speed = 0.0f;
		_speed = stage["Speed"];

		int _stamina = 0;
		_stamina = stage["Stamina"];

		float _atkRange = 0.0f;
		_atkRange = stage["ATKRange"];

		Math::Vector3 _forword = Math::Vector3::Zero;
		_forword.x = stage["ForwordX"];
		_forword.y = stage["ForwordY"];
		_forword.z = stage["ForwordZ"];

		float _chaseRange = 0.0f;
		_chaseRange = stage["ChaseRange"];

		std::shared_ptr<Bone> bone = std::make_shared<Bone>();
		bone->Init();
		bone->SetPos(_pos);
		bone->SetParam(_hp, _atk, _speed, _stamina, _angleY, _size, _atkRange, _forword);
		bone->SetChaseRange(_chaseRange);
		if (m_player.expired() == false)
		{
			bone->SetPlayer(m_player.lock());
		}

		SceneManager::Instance().AddObject(bone);

	}

	ifs.close();
}
