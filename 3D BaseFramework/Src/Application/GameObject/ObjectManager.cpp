#include "ObjectManager.h"
#include"../Scene/SceneManager.h"
#include"../tinygltf/json.hpp"
#include"Character/CharacterBase.h"
#include"Character/Enemy/Bone/Bone.h"
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

bool ObjectManager::IsWaveMax()
{
	if (m_nowWave == m_MaxWave)return true;
	return false;
}

void ObjectManager::PlayerWrite()
{
	nlohmann::json _json;

	_json["Player"]["Name"] = "Player";
	_json["Player"]["PosX"] = m_player.lock()->GetParam().Pos.x;
	_json["Player"]["PosY"] = m_player.lock()->GetParam().Pos.y;
	_json["Player"]["PosZ"] = m_player.lock()->GetParam().Pos.z;
	_json["Player"]["DirX"] = m_player.lock()->GetParam().Dir.x;
	_json["Player"]["DirY"] = m_player.lock()->GetParam().Dir.y;
	_json["Player"]["DirZ"] = m_player.lock()->GetParam().Dir.z;
	_json["Player"]["Size"] = m_player.lock()->GetParam().Size;
	_json["Player"]["Angle"] = m_player.lock()->GetParam().Angle;
	_json["Player"]["HP"] = m_player.lock()->GetParam().Hp;
	_json["Player"]["ATK"] = m_player.lock()->GetParam().Atk;
	_json["Player"]["Speed"] = m_player.lock()->GetParam().Sp;
	_json["Player"]["Stamina"] = m_player.lock()->GetParam().Sm;
	_json["Player"]["ATKRange"] = m_player.lock()->GetParam().AtkRange;
	_json["Player"]["ForwardX"] = m_player.lock()->GetParam().ForwardX;
	_json["Player"]["ForwardY"] = m_player.lock()->GetParam().ForwardY;
	_json["Player"]["ForwardZ"] = m_player.lock()->GetParam().ForwardZ;
	_json["Player"]["InviTime"] = m_player.lock()->GetinviTime();
	_json["Player"]["SwordName"] = m_player.lock()->GetSword().lock()->GetWeaponName();
	_json["Player"]["ShieldName"]= m_player.lock()->GetShield().lock()->GetWeaponName();
	

	std::ofstream _file("Json/Player/Player.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::EnemyWrite(int _waveNum)
{
	nlohmann::json _json;
	std::ifstream _oldFile("Json/Enemy/Enemy.json");
	if (_oldFile.is_open())
	{
		_oldFile >> _json;
		_oldFile.close();
	}

	std::string wave = (std::to_string(_waveNum).c_str()) + ((std::string)"Wave");
	_json[wave].clear();

	for (int bone = 0; bone < m_BoneList.size(); ++bone)
	{
		std::string name = (std::to_string(bone).c_str());
		name += "Bone";
		_json[wave][name]["Name"] = "Bone";
		_json[wave][name]["PosX"] = m_BoneList[bone].lock()->GetParam().Pos.x;
		_json[wave][name]["PosY"] = m_BoneList[bone].lock()->GetParam().Pos.y;
		_json[wave][name]["PosZ"] = m_BoneList[bone].lock()->GetParam().Pos.z;
		_json[wave][name]["DirX"] = m_BoneList[bone].lock()->GetParam().Dir.x;
		_json[wave][name]["DirY"] = m_BoneList[bone].lock()->GetParam().Dir.y;
		_json[wave][name]["DirZ"] = m_BoneList[bone].lock()->GetParam().Dir.z;
		_json[wave][name]["Size"] = m_BoneList[bone].lock()->GetParam().Size;
		_json[wave][name]["Angle"] = m_BoneList[bone].lock()->GetParam().Angle;
		_json[wave][name]["HP"] = m_BoneList[bone].lock()->GetParam().Hp;
		_json[wave][name]["ATK"] = m_BoneList[bone].lock()->GetParam().Atk;
		_json[wave][name]["Speed"] = m_BoneList[bone].lock()->GetParam().Sp;
		_json[wave][name]["Stamina"] = m_BoneList[bone].lock()->GetParam().Sm;
		_json[wave][name]["ATKRange"] = m_BoneList[bone].lock()->GetParam().AtkRange;
		_json[wave][name]["ForwardX"] = m_BoneList[bone].lock()->GetParam().ForwardX;
		_json[wave][name]["ForwardY"] = m_BoneList[bone].lock()->GetParam().ForwardY;
		_json[wave][name]["ForwardZ"] = m_BoneList[bone].lock()->GetParam().ForwardZ;
	}

	std::ofstream _newFile("Json/Enemy/Enemy.json");
	if (_newFile.is_open())
	{
		_newFile << _json.dump();
		_newFile.close();
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
		_forword.x = stage["ForwardX"];
		_forword.y = stage["ForwardY"];
		_forword.z = stage["ForwardZ"];

		int _inviTime = 0;
		_inviTime = stage["InviTime"];

		std::shared_ptr<Player> player = std::make_shared<Player>();
		std::shared_ptr<TPSCamera> camera = std::make_shared<TPSCamera>();
		player->SetCamera(camera);
		player->SetParam(_hp, _atk, _speed, _stamina, _pos, _dir, _angleY, _size, _atkRange, _forword);
		player->Init();
		player->SetInviTime(_inviTime);
		player->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddObject(player);
		m_player = player;

		SetWeaponParam("Json/Weapon/Sword/Sword.json", stage["SwordName"]);
		SetWeaponParam("Json/Weapon/Shield/Shield.json", stage["ShieldName"]);

		camera->SetTarget(player);
		camera->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddObject(camera);
		KdEffekseerManager::GetInstance().SetCamera(camera->GetCamera());
	}

	ifs.close();
}

void ObjectManager::SetWeaponParam(std::string _filePath, std::string _weaponName)
{
	//jsonファイル
	std::string fileName = _filePath;

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& stage : _json)
	{
		if (stage["ObjectName"] == "Sword")
		{
			m_swordNameList.push_back(stage["Name"]);
		}
		else if (stage["ObjectName"] == "Shield")
		{
			m_shieldNameList.push_back(stage["Name"]);
		}

		if (stage["Name"] != _weaponName)continue;

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
		if (stage["ObjectName"] == "Sword")
		{
			std::shared_ptr<Sword> sword = std::make_shared<Sword>();
			m_player.lock()->SetSword(sword);
			weaponATK = stage["ATK"];
			sword->SetTrajectPointNUM(stage["Traject"]);
			weapon = sword;
		}
		else if (stage["ObjectName"] == "Shield")
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
		weapon->SetWeaponName(_weaponName);
		weapon->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddObject(weapon);
		
		//デバッグ
		//break;
	}

	ifs.close();
}

void ObjectManager::SetEnemyParam(std::string _filePath)
{
	if (m_nowWave == 0)
	{
		//jsonファイル
		std::string fileName = _filePath;

		std::ifstream ifs(fileName.c_str());
		if (ifs.is_open())
		{
			ifs >> m_EnemyJson;
			ifs.close();
		}

		m_MaxWave = m_EnemyJson.size();
	}
	m_nowWave++;

	std::string wave = std::to_string(m_nowWave).c_str() + ((std::string)"Wave");
	for (auto& stage : m_EnemyJson[wave])
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
		_forword.x = stage["ForwardX"];
		_forword.y = stage["ForwardY"];
		_forword.z = stage["ForwardZ"];

		std::shared_ptr<EnemyBase> enemy = nullptr;
		if (stage["Name"] == "Bone")
		{
			std::shared_ptr<Bone> bone = std::make_shared<Bone>();
			m_BoneList.push_back(bone);
			enemy = bone;
		}
		if (m_player.expired() == false)
		{
			enemy->SetPlayer(m_player.lock());
		}
		enemy->SetParam(_hp, _atk, _speed, _stamina, _pos, _dir, _angleY, _size, _atkRange, _forword);
		enemy->Init();
		enemy->SetPos(_pos);
		enemy->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddObject(enemy);
		m_EnemyList.push_back(enemy);
	}
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
		enemy->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddObject(enemy);
		m_EnemyList.push_back(enemy);
}

void ObjectManager::AddWeapon(std::string _filePath,std::string _weaponName)
{
	//jsonファイル
	std::string fileName = _filePath;

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& stage : _json)
	{

		if (stage["Name"] != _weaponName)continue;

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
		if (stage["ObjectName"] == "Sword")
		{
			std::shared_ptr<Sword> sword = std::make_shared<Sword>();
			m_player.lock()->SetSword(sword);
			weaponATK = stage["ATK"];
			sword->SetTrajectPointNUM(stage["Traject"]);
			weapon = sword;
		}
		else if (stage["ObjectName"] == "Shield")
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
		weapon->SetWeaponName(_weaponName);
		weapon->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddObject(weapon);

		break;
	}

	ifs.close();
}

void ObjectManager::ChangeWeapon(std::string _swordName, std::string _shieldName)
{
	if (m_player.lock()->GetSword().lock()->GetWeaponName() != _swordName)
	{
		m_player.lock()->GetSword().lock()->Expired();

		AddWeapon("Json/Weapon/Sword/Sword.json", _swordName);
	}

	if (m_player.lock()->GetShield().lock()->GetWeaponName() != _shieldName)
	{
		m_player.lock()->GetShield().lock()->Expired();

		AddWeapon("Json/Weapon/Shield/Shield", _shieldName);
	}
}
