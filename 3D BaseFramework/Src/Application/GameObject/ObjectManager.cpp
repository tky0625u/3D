#include "ObjectManager.h"
#include"../Scene/SceneManager.h"
#include"../tinygltf/json.hpp"
#include"Character/CharacterBase.h"
#include<fstream>
#include<sstream>

//地面
#include"../GameObject/Stage/Ground/Ground.h"
//壁
#include"../GameObject/Stage/Wall/Wall.h"
//スカイボックス
#include"../GameObject/SkyBox/SkyBox.h"
//剣
#include"../GameObject/Weapon/Sword/Sword.h"
//盾
#include"../GameObject/Weapon/Shield/Shield.h"
//プレイヤー
#include"../GameObject/Character/Player/Player.h"
//カメラ
#include"../GameObject/Camera/TPSCamera/TPSCamera.h"
//敵
#include"../GameObject/Character/Enemy/Bone/Bone.h"

void ObjectManager::DeleteEnemyList()
{
	auto bone = m_BoneList.begin();

	while (bone != m_BoneList.end())
	{
		if (bone->expired())
		{
			bone = m_BoneList.erase(bone);
		}
		else
		{
			++bone;
		}
	}

	auto enemy = m_EnemyList.begin();

	while (enemy != m_EnemyList.end())
	{
		if (enemy->expired())
		{
			enemy = m_EnemyList.erase(enemy);
		}
		else
		{
			++enemy;
		}
	}
}

void ObjectManager::SlowChange()
{
	if (m_slowFlg)
	{
		m_slowFlg = false;
		m_slow = 1.0f;
	}
	else
	{
		m_slowFlg = true;
		m_slow = 0.5f;
	}
}

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
		if (stage["Name"] == "Ground")
		{
			obj = std::make_shared<Ground>();
		}
		if (stage["Name"] == "Wall")
		{
			obj = std::make_shared<Wall>();
		}
		if (stage["Name"] == "SkyBox")
		{
			obj = std::make_shared<SkyBox>();
		}

		obj->SetPos(_pos);
		obj->SetScale(_size);
		obj->SetAngleY(_angleY);
		obj->SetID(m_id);
		m_id++;

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

		Math::Vector3 _dir = Math::Vector3::Zero;
		_dir.x = stage["DirX"];
		_dir.y = stage["DirY"];
		_dir.z = stage["DirZ"];

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

		int _stopTime = 0;
		_stopTime = stage["StopTime"];

		std::shared_ptr<Player> player = std::make_shared<Player>();
		std::shared_ptr<TPSCamera> camera = std::make_shared<TPSCamera>();
		player->SetCamera(camera);
		player->SetParam(_hp, _atk, _speed, _stamina, _pos, _dir, _angleY, _size, _atkRange, _forword);
		player->Init();
		player->SetStopTime(_stopTime);
		player->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddObject(player);
		m_player = player;

		if (stage["Sword"])SetWeaponParam("Json/Weapon/Sword/Sword.json", stage["SwordID"]);
		if (stage["Shield"])SetWeaponParam("Json/Weapon/Shield/Shield.json", stage["ShieldID"]);

		camera->SetTarget(player);
		camera->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddObject(camera);
		KdEffekseerManager::GetInstance().SetCamera(camera->GetCamera());
	}

	ifs.close();
}

void ObjectManager::SetWeaponParam(std::string _filePath,int _id)
{
	//jsonファイル
	std::string fileName = _filePath;

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	int weaponID = 0;

	for (auto& stage : _json)
	{
		if (weaponID != _id)
		{
			weaponID++;
			continue;
		}

		Math::Vector3 _pos = Math::Vector3::Zero;
		_pos.x = stage["PosX"];
		_pos.y = stage["PosY"];
		_pos.z = stage["PosZ"];

		float _size = 0.0f;
		_size = stage["Size"];

		float _angleY = 0.0f;
		_angleY = stage["Angle"];

		std::shared_ptr<WeaponBase> weapon = nullptr;
		int weaponATK = 0;
		if (stage["Name"] == "Sword")
		{
			std::shared_ptr<Sword> sword = std::make_shared<Sword>();
			m_player.lock()->SetSword(sword);
			weaponATK = stage["ATK"];
			sword->SetTrajectPointNUM(stage["Traject"]);
			weapon = sword;
		}
		else if (stage["Name"] == "Shield")
		{
			std::shared_ptr<Shield> shield = std::make_shared<Shield>();
			m_player.lock()->SetShield(shield);
			weapon = shield;
		}

		std::string _modelPath;
		_modelPath = stage["Path"];

		weapon->SetModelPath(_modelPath);
		weapon->Init();
		weapon->SetPos(_pos);
		weapon->SetScale(_size);
		weapon->SetAngleY(_angleY);
		if (m_player.expired() == false)
		{
			weapon->SetTarget(m_player.lock());
			if (weaponATK != 0)m_player.lock()->SetATK(stage["ATK"]);
		}
		weapon->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddObject(weapon);
		break;
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

		Math::Vector3 _dir = Math::Vector3::Zero;
		_dir.x = stage["DirX"];
		_dir.y = stage["DirY"];
		_dir.z = stage["DirZ"];

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

		std::shared_ptr<EnemyBase> enemy = nullptr;
		if (stage["Name"] == "Bone")
		{
			std::shared_ptr<Bone> bone = std::make_shared<Bone>();
			m_BoneList.push_back(bone);
			enemy =bone;
		}
		if (m_player.expired() == false)
		{
			enemy->SetPlayer(m_player.lock());
		}
		enemy->SetParam(_hp, _atk, _speed, _stamina, _pos, _dir, _angleY, _size, _atkRange, _forword);
		enemy->Init();
		enemy->SetPos(_pos);
		enemy->SetChaseRange(_chaseRange);
		enemy->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddObject(enemy);
		m_EnemyList.push_back(enemy);
	}

	ifs.close();
}

void ObjectManager::AddBone()
{
		Math::Vector3 _pos = Math::Vector3::Zero;
		Math::Vector3 _dir = Math::Vector3::Zero;
		float _size = 1.5f;
		float _angleY = 180.0f;
		int _hp = 10;
		int _atk = 2;
		float _speed = 1.0f;
		int _stamina = 50;
		float _atkRange = 3.0f;
		Math::Vector3 _forword = Math::Vector3::Zero;
		_forword.z = 1.0f;
		float _chaseRange = 1000.0f;

		std::shared_ptr<Bone> enemy = nullptr;
		enemy = std::make_shared<Bone>();
		m_BoneList.push_back(enemy);
		if (m_player.expired() == false)
		{
			enemy->SetPlayer(m_player.lock());
		}
		enemy->SetParam(_hp, _atk, _speed, _stamina, _pos, _dir, _angleY, _size, _atkRange, _forword);
		enemy->Init();
		enemy->SetChaseRange(_chaseRange);
		enemy->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddObject(enemy);
		m_EnemyList.push_back(enemy);
}
