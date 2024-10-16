#include "ObjectManager.h"
#include"../Scene/SceneManager.h"
#include"../tinygltf/json.hpp"
#include"Character/CharacterBase.h"
#include"Character/Enemy/Bone/Bone.h"
#include<fstream>
#include<sstream>

//地面
#include"../GameObject/Stage/Ground/Ground.h"
//魔法陣の台
#include"../GameObject/Stage/Circle/Circle.h"
//魔法陣
#include"../GameObject/Stage/MagicPolygon/MagicPolygon.h"
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

void ObjectManager::DeleteObjectList()
{
	auto Obj = m_ObjectList.begin();

	while (Obj != m_ObjectList.end())
	{
		if (Obj->lock()->IsExpired())
		{
			Obj = m_ObjectList.erase(Obj);
		}
		else
		{
			++Obj;
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

void ObjectManager::Clear()
{
	if (m_MaxStage == m_nowStage)
	{

	}
	else
	{
		m_nowStage++;
		m_player.lock()->SetPos(m_StartPosList[m_nowStage - 1]);
		m_nowWave = 0;
		std::string _filePath = ((std::string)"Json/Enemy/Stage") + (std::to_string(m_nowStage).c_str()) + ((std::string)".json");
		SetEnemyParam(_filePath);
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
	_json["Player"]["PosX"] = m_player.lock()->GetPos().x;
	_json["Player"]["PosY"] = m_player.lock()->GetPos().y;
	_json["Player"]["PosZ"] = m_player.lock()->GetPos().z;
	_json["Player"]["DirX"] = m_player.lock()->GetDir().x;
	_json["Player"]["DirY"] = m_player.lock()->GetDir().y;
	_json["Player"]["DirZ"] = m_player.lock()->GetDir().z;
	_json["Player"]["Size"] = m_player.lock()->GetSize();
	_json["Player"]["Angle"] = m_player.lock()->GetAngle();
	_json["Player"]["HP"] = m_player.lock()->GetParam().Hp;
	_json["Player"]["ATK"] = m_player.lock()->GetParam().Atk;
	_json["Player"]["Speed"] = m_player.lock()->GetParam().Sp;
	_json["Player"]["Stamina"] = m_player.lock()->GetParam().Sm;
	_json["Player"]["ATKRange"] = m_player.lock()->GetAtkRange();
	_json["Player"]["ForwardX"] = m_player.lock()->GetForward().x;
	_json["Player"]["ForwardY"] = m_player.lock()->GetForward().y;
	_json["Player"]["ForwardZ"] = m_player.lock()->GetForward().z;
	_json["Player"]["InviTime"] = m_player.lock()->GetinviTime();
	_json["Player"]["SwordName"] = m_player.lock()->GetSword().lock()->GetName();
	_json["Player"]["ShieldName"]= m_player.lock()->GetShield().lock()->GetName();
	

	std::ofstream _file("Json/Player/Player.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::EnemyWrite(int _stage, int _wave)
{
	std::string _stagePath = ((std::string)"Json/Enemy/Stage") + std::to_string(_stage).c_str() + ((std::string)".json");
	nlohmann::json _json;
	std::ifstream _oldFile(_stagePath);
	if (_oldFile.is_open())
	{
		_oldFile >> _json;
		_oldFile.close();
	}

	std::string wave = (std::to_string(_wave).c_str()) + ((std::string)"Wave");
	_json[wave].clear();

	for (auto& enemy:m_EnemyList)
	{
		std::string _category;
		std::string _name;
		if (enemy.lock()->GetName() == "Bone")
		{
			static int b = 0;
			_category = "Bone";
			_name = (std::to_string(b).c_str()) + ((std::string)"Bone");
			b++;
		}

		_json[wave][_category][_name]["Name"] = enemy.lock()->GetName();
		_json[wave][_category][_name]["PosX"] = enemy.lock()->GetPos().x;
		_json[wave][_category][_name]["PosY"] = enemy.lock()->GetPos().y;
		_json[wave][_category][_name]["PosZ"] = enemy.lock()->GetPos().z;
		_json[wave][_category][_name]["DirX"] = enemy.lock()->GetDir().x;
		_json[wave][_category][_name]["DirY"] = enemy.lock()->GetDir().y;
		_json[wave][_category][_name]["DirZ"] = enemy.lock()->GetDir().z;
		_json[wave][_category][_name]["Size"] = enemy.lock()->GetSize();
		_json[wave][_category][_name]["Angle"] = enemy.lock()->GetAngle();
		_json[wave][_category][_name]["HP"] = enemy.lock()->GetParam().Hp;
		_json[wave][_category][_name]["ATK"] = enemy.lock()->GetParam().Atk;
		_json[wave][_category][_name]["Speed"] = enemy.lock()->GetParam().Sp;
		_json[wave][_category][_name]["Stamina"] = enemy.lock()->GetParam().Sm;
		_json[wave][_category][_name]["ATKRange"] = enemy.lock()->GetAtkRange();
		_json[wave][_category][_name]["ForwardX"] = enemy.lock()->GetForward().x;
		_json[wave][_category][_name]["ForwardY"] = enemy.lock()->GetForward().y;
		_json[wave][_category][_name]["ForwardZ"] = enemy.lock()->GetForward().z;
	}

	std::ofstream _newFile(_stagePath);
	if (_newFile.is_open())
	{
		_newFile << _json.dump();
		_newFile.close();
	}
}

void ObjectManager::SwordWrite(std::string _swordName)
{
	nlohmann::json _json;
	std::ifstream _oldFile("Json/Weapon/Sword/Sword.json");
	if (_oldFile.is_open())
	{
		_oldFile >> _json;
		_oldFile.close();
	}

	for(auto& _sword: _json)
	{
		if (_sword["Name"] != _swordName)continue;

		_sword["PosX"] = m_player.lock()->GetSword().lock()->GetPos().x;
		_sword["PosY"] = m_player.lock()->GetSword().lock()->GetPos().y;
		_sword["PosZ"] = m_player.lock()->GetSword().lock()->GetPos().z;
		_sword["ATK"] = m_player.lock()->GetSword().lock()->GetATK();
		_sword["Size"] = m_player.lock()->GetSword().lock()->GetSize();
		_sword["Traject"] = m_player.lock()->GetSword().lock()->GetTraject();
	}

	std::ofstream _newFile("Json/Weapon/Sword/Sword.json");
	if (_newFile.is_open())
	{
		_newFile << _json.dump();
		_newFile.close();
	}
}

void ObjectManager::ShieldWrite(std::string _shieldName)
{
	nlohmann::json _json;
	std::ifstream _oldFile("Json/Weapon/Shield/Shield.json");
	if (_oldFile.is_open())
	{
		_oldFile >> _json;
		_oldFile.close();
	}

	for (auto& _shield : _json)
	{
		if (_shield["Name"] != _shieldName)continue;

		_shield["PosX"] = m_player.lock()->GetShield().lock()->GetPos().x;
		_shield["PosY"] = m_player.lock()->GetShield().lock()->GetPos().y;
		_shield["PosZ"] = m_player.lock()->GetShield().lock()->GetPos().z;
		_shield["Size"] = m_player.lock()->GetShield().lock()->GetSize();
	}

	std::ofstream _newFile("Json/Weapon/Shield/Shield.json");
	if (_newFile.is_open())
	{
		_newFile << _json.dump();
		_newFile.close();
	}
}

void ObjectManager::ObjectWrite()
{
	nlohmann::json _json;

	for (auto& obj:m_ObjectList)
	{
		std::string _category;
		std::string _name;
		if (obj.lock()->GetName() == "Ground")
		{
			static int g = 0;
			_category = "Ground";
			_name = (std::to_string(g).c_str()) + ((std::string)"Ground");
			g++;
		}
		else if (obj.lock()->GetName() == "Circle")
		{
			static int c = 0;
			_category = "Circle";
			_name = (std::to_string(c).c_str()) + ((std::string)"Circle");
			c++;
		}
		else if (obj.lock()->GetName() == "Magic")
		{
			static int m = 0;
			_category = "Magic";
			_name = (std::to_string(m).c_str()) + ((std::string)"Magic");
			m++;
		}
		else if (obj.lock()->GetName() == "Wall")
		{
			static int w = 0;
			_category = "Wall";
			_name = (std::to_string(w).c_str()) + ((std::string)"Wall");
			w++;
		}
		else if (obj.lock()->GetName() == "SkyBox")
		{
			static int s = 0;
			_category = "SkyBox";
			_name = (std::to_string(s).c_str()) + ((std::string)"SkyBox");
			s++;
		}

		_json[_category][_name]["Name"] = obj.lock()->GetName();
		_json[_category][_name]["PosX"] = obj.lock()->GetPos().x;
		_json[_category][_name]["PosY"] = obj.lock()->GetPos().y;
		_json[_category][_name]["PosZ"] = obj.lock()->GetPos().z;
		_json[_category][_name]["Size"] = obj.lock()->GetSize();
		_json[_category][_name]["Angle"] = obj.lock()->GetAngle();
	}

	std::ofstream _file("Json/Object/Object.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::SetCameraParam()
{
	//jsonファイル
	std::string fileName = "Json/Camera/Camera.json";

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& stage : _json)
	{
		Math::Vector3 _PlayerPos = Math::Vector3::Zero;
		_PlayerPos.x = stage["PlayerPosX"];
		_PlayerPos.y = stage["PlayerPosY"];
		_PlayerPos.z = stage["PlayerPosZ"];

		Math::Vector3 _FixedPos = Math::Vector3::Zero;
		_FixedPos.x = stage["FixedPosX"];
		_FixedPos.y = stage["FixedPosY"];
		_FixedPos.z = stage["FixedPosZ"];

		Math::Vector2 _angle = Math::Vector2::Zero;
		_angle.x = stage["AngleX"];
		_angle.y = stage["AngleY"];

		std::string _name;
		_name = stage["Name"];
		std::shared_ptr<TPSCamera> camera = std::make_shared<TPSCamera>();

		camera->SetPlayerTargetPos(_PlayerPos);
		camera->SetFixedTargetPos(_FixedPos);
		camera->SetFixedTargetAngle(_angle);
		camera->SetName(_name);
		camera->SetID(m_id);
		camera->Init();
		m_id++;

		m_camera = camera;
		SceneManager::Instance().AddObject(camera);
	}

	ifs.close();
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

	std::vector<std::shared_ptr<Circle>> circleList;
	std::vector<std::shared_ptr<MagicPolygon>> magicList;

	for (auto& category : _json)
	{
		for (auto& stage : category)
		{
			Math::Vector3 _pos = Math::Vector3::Zero;
			_pos.x = stage["PosX"];
			_pos.y = stage["PosY"];
			_pos.z = stage["PosZ"];

			float _size = 0.0f;
			_size = stage["Size"];

			float _angleY = 0.0f;
			_angleY = stage["Angle"];

			std::string _name;
			_name = stage["Name"];
			std::shared_ptr<KdGameObject> obj;
			if (_name == "Ground")
			{
				obj = std::make_shared<Ground>();
			}
			if (_name == "Circle")
			{
				std::shared_ptr<Circle> circle = std::make_shared<Circle>();
				circleList.push_back(circle);
				if (m_camera.expired() == false)m_camera.lock()->SetFixedTargetList(circle);
				obj = circle;
			}
			if (_name == "Magic")
			{
				std::shared_ptr<MagicPolygon> magic = std::make_shared<MagicPolygon>();
				magicList.push_back(magic);
				obj = magic;
			}
			if (_name == "Wall")
			{
				obj = std::make_shared<Wall>();
			}
			if (_name == "SkyBox")
			{
				obj = std::make_shared<SkyBox>();
			}

			obj->SetPos(_pos);
			obj->SetSize(_size);
			obj->SetAngle(_angleY);
			obj->SetName(_name);
			obj->SetID(m_id);
			obj->Init();
			m_id++;

			m_ObjectList.push_back(obj);
			if (_name == "Circle")m_StartPosList.push_back(_pos);
			SceneManager::Instance().AddObject(obj);
		}
	}

	ifs.close();

	for (int m = 0; m < magicList.size(); ++m)
	{
		magicList[m]->SetCircle(circleList[m]);
	}
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
		std::string _name;
		_name = stage["Name"];

		Math::Vector3 _pos = Math::Vector3::Zero;
		_pos.x = m_StartPosList[m_nowStage - 1].x;
		_pos.y = m_StartPosList[m_nowStage - 1].y;
		_pos.z = m_StartPosList[m_nowStage - 1].z;

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

		float _speed = 0.0f;
		_speed = stage["Speed"];

		int _stamina = 0;
		_stamina = stage["Stamina"];

		float _atkRange = 0.0f;
		_atkRange = stage["ATKRange"];

		Math::Vector3 _forward = Math::Vector3::Zero;
		_forward.x = stage["ForwardX"];
		_forward.y = stage["ForwardY"];
		_forward.z = stage["ForwardZ"];

		int _inviTime = 0;
		_inviTime = stage["InviTime"];

		std::shared_ptr<Player> player = std::make_shared<Player>();
		m_player = player;

		SetWeaponParam("Json/Weapon/Sword/Sword.json", stage["SwordName"]);
		SetWeaponParam("Json/Weapon/Shield/Shield.json", stage["ShieldName"]);

		player->SetCamera(m_camera.lock());
		player->SetParam(_hp, player->GetSword().lock()->GetATK(), _speed, _stamina);
		player->SetPos(_pos);
		player->SetSize(_size);
		player->SetDir(_dir);
		player->SetAngle(_angleY);
		player->SetAtkRange(_atkRange);
		player->SetForward(_forward);
		player->Init();
		player->SetInviTime(_inviTime);
		player->SetName(_name);
		player->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddObject(player);

		m_camera.lock()->SetTarget(player);
		m_camera.lock()->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddObject(m_camera.lock());
		KdEffekseerManager::GetInstance().SetCamera(m_camera.lock()->GetCamera());
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

		std::string _name;
		_name = stage["Name"];
		std::shared_ptr<WeaponBase> weapon = nullptr;
		if (stage["ObjectName"] == "Sword")
		{
			std::shared_ptr<Sword> sword = std::make_shared<Sword>();
			int weaponATK = 0;
			weaponATK = stage["ATK"];
			sword->SetATK(weaponATK);
			sword->SetTrajectPointNUM(stage["Traject"]);
			m_player.lock()->SetSword(sword);
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
		weapon->SetSize(_size);
		weapon->SetAngle(_angleY);
		weapon->SetTarget(m_player.lock());
		weapon->SetName(_name);
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

	std::string _wave = std::to_string(m_nowWave).c_str() + ((std::string)"Wave");
	for (auto& category : m_EnemyJson[_wave])
	{
		for (auto& stage : category)
		{
			std::string _name;
			_name = stage["Name"];

			Math::Vector3 _pos = Math::Vector3::Zero;
			_pos.x = stage["PosX"];
			_pos.y = m_StartPosList[m_nowStage - 1].y;
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

			Math::Vector3 _forward = Math::Vector3::Zero;
			_forward.x = stage["ForwardX"];
			_forward.y = stage["ForwardY"];
			_forward.z = stage["ForwardZ"];

			std::shared_ptr<EnemyBase> enemy = nullptr;
			if (stage["Name"] == "Bone")
			{
				std::shared_ptr<Bone> bone = std::make_shared<Bone>();
				enemy = bone;
			}
			if (m_player.expired() == false)
			{
				enemy->SetPlayer(m_player.lock());
			}
			enemy->SetParam(_hp, _atk, _speed, _stamina);
			enemy->SetPos(_pos);
			enemy->SetSize(_size);
			enemy->SetDir(_dir);
			enemy->SetAngle(_angleY);
			enemy->SetAtkRange(_atkRange);
			enemy->SetForward(_forward);
			enemy->SetName(_name);
			enemy->SetID(m_id);
			enemy->Init();
			m_id++;

			SceneManager::Instance().AddObject(enemy);
			m_EnemyList.push_back(enemy);
		}
	}
}

void ObjectManager::AddBone()
{
	std::string _name = "Bone";
	Math::Vector3 _pos = Math::Vector3::Zero;
	Math::Vector3 _dir = Math::Vector3::Zero;
	float _size = 1.5f;
	float _angleY = 180.0f;
	int _hp = 10;
	int _atk = 2;
	float _speed = 1.0f;
	int _stamina = 50;
	float _atkRange = 3.0f;
	Math::Vector3 _forward = Math::Vector3::Zero;
	_forward.z = 1.0f;
	float _chaseRange = 1000.0f;

	std::shared_ptr<Bone> enemy = nullptr;
	enemy = std::make_shared<Bone>();
	if (m_player.expired() == false)
	{
		enemy->SetPlayer(m_player.lock());
	}
	enemy->SetParam(_hp, _atk, _speed, _stamina);
	enemy->SetPos(_pos);
	enemy->SetSize(_size);
	enemy->SetDir(_dir);
	enemy->SetAngle(_angleY);
	enemy->SetAtkRange(_atkRange);
	enemy->SetForward(_forward);
	enemy->SetName(_name);
	enemy->SetID(m_id);
	enemy->Init();
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
		std::string _name;
		_name = stage["Name"];
		if (_name != _weaponName)continue;

		Math::Vector3 _pos = Math::Vector3::Zero;
		_pos.x = stage["PosX"];
		_pos.y = stage["PosY"];
		_pos.z = stage["PosZ"];

		float _size = 0.0f;
		_size = stage["Size"];

		float _angleY = 0.0f;
		_angleY = stage["Angle"];

		std::shared_ptr<WeaponBase> weapon = nullptr;
		if (stage["ObjectName"] == "Sword")
		{
			std::shared_ptr<Sword> sword = std::make_shared<Sword>();
			int weaponATK = 0;
			weaponATK = stage["ATK"];
			sword->SetTrajectPointNUM(stage["Traject"]);
			sword->SetATK(weaponATK);
			m_player.lock()->SetSword(sword);
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
		weapon->SetSize(_size);
		weapon->SetAngle(_angleY);
		weapon->SetName(_name);
		weapon->SetTarget(m_player.lock());
		weapon->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddObject(weapon);

		break;
	}

	ifs.close();
}

void ObjectManager::AddGround()
{
	std::string _name = "Ground";
	Math::Vector3 _pos = Math::Vector3::Zero;
	float _size = 10.0f;
	float _angleY = 0.0f;
	std::shared_ptr<Ground> obj = std::make_shared<Ground>();

	obj->SetPos(_pos);
	obj->SetSize(_size);
	obj->SetAngle(_angleY);
	obj->SetName(_name);
	obj->SetID(m_id);
	obj->Init();

	m_id++;

	m_ObjectList.push_back(obj);
	SceneManager::Instance().AddObject(obj);
}

void ObjectManager::AddCircle()
{
	std::string _name;
	Math::Vector3 _pos;
	float _size;
	float _angleY;
	std::shared_ptr<KdGameObject> obj;
	std::shared_ptr<Circle> circle;
	std::shared_ptr<MagicPolygon> magic;

	for (int i = 0; i < 2; ++i)
	{
		switch (i)
		{
		case 0:
			circle = std::make_shared<Circle>();
			_name = "Circle";
			_pos = Math::Vector3::Zero;
			_size = 10.0f;
			_angleY = 0.0f;
			obj = circle;
			break;
		case 1:
			magic = std::make_shared<MagicPolygon>();
			_name = "Magic";
			if (circle)_pos = circle->GetMagicPolygonPoint();
			else { _pos = Math::Vector3::Zero; }
			_size = 5.0f;
			_angleY = 0.0f;
			obj = magic;
			break;
		default:
			break;
		}

		obj->SetPos(_pos);
		obj->SetSize(_size);
		obj->SetAngle(_angleY);
		obj->SetName(_name);
		obj->SetID(m_id);
		obj->Init();

		m_id++;

		m_ObjectList.push_back(obj);
		SceneManager::Instance().AddObject(obj);
	}
}

void ObjectManager::AddWall()
{
	std::string _name = "Wall";
	Math::Vector3 _pos = Math::Vector3::Zero;
	float _size = 10.0f;
	float _angleY = 0.0f;
	std::shared_ptr<Wall> obj = std::make_shared<Wall>();

	obj->SetPos(_pos);
	obj->SetSize(_size);
	obj->SetAngle(_angleY);
	obj->SetName(_name);
	obj->SetID(m_id);
	obj->Init();

	m_id++;

	m_ObjectList.push_back(obj);
	SceneManager::Instance().AddObject(obj);
}

void ObjectManager::ChangeWeapon(std::string _swordName, std::string _shieldName)
{
	if (m_player.lock()->GetSword().lock()->GetName() != _swordName)
	{
		m_player.lock()->GetSword().lock()->Expired();

		AddWeapon("Json/Weapon/Sword/Sword.json", _swordName);
	}

	if (m_player.lock()->GetShield().lock()->GetName() != _shieldName)
	{
		m_player.lock()->GetShield().lock()->Expired();

		AddWeapon("Json/Weapon/Shield/Shield", _shieldName);
	}
}
