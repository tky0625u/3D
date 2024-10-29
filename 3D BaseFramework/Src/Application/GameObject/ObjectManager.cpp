#include "ObjectManager.h"
#include"../Scene/SceneManager.h"
#include"../tinygltf/json.hpp"
#include"Character/CharacterBase.h"
#include"Character/Enemy/Bone/Bone.h"
#include<fstream>
#include<sstream>

//地面
#include"Stage/Ground/Ground.h"
//魔法陣の台
#include"Stage/Circle/Circle.h"
//魔法陣
#include"Stage/MagicPolygon/MagicPolygon.h"
//壁
#include"Stage/Wall/Wall.h"
//スカイボックス
#include"SkyBox/SkyBox.h"
//剣
#include"Weapon/Sword/Sword.h"
//盾
#include"Weapon/Shield/Shield.h"
//プレイヤー
#include"Character/Player/Player.h"
//カメラ
#include"Camera/TPSCamera/TPSCamera.h"
//骨
#include"Character/Enemy/Bone/Bone.h"
//ゴーレム
#include"Character/Enemy/Golem/Golem.h"
//タイトル
#include"UI/Title/Title/Title.h"
//ゲーム文字
#include"UI/Title/Game/Game.h"
//終了文字
#include"UI/Title/Exit/Exit.h"
//タイトルガイド
#include"UI/Title/Guide/Guide.h"
//カーソル
#include"UI/Title/Cursor/Cursor.h"
//タイトルカメラ
#include"Camera/TitleCamera/TitleCamera.h"


void ObjectManager::SceneCheck()
{
	switch (SceneManager::Instance().GetNowSceneType())
	{
	case SceneManager::SceneType::Title:
		m_nowScene = "Title";
		break;
	case SceneManager::SceneType::Game:
		m_nowScene = "Game";
		break;
	default:
		break;
	}
}

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
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
		);
	}
	else
	{
		m_nowStage++;
		m_player.lock()->SetPos(m_StartPosList[m_nowStage - 1]);
		m_nowWave = 0;
		std::string _filePath = ((std::string)"Json/Game/Enemy/Stage") + (std::to_string(m_nowStage).c_str()) + ((std::string)".json");
		SetEnemyParam(_filePath);
	}
}

bool ObjectManager::IsWaveMax()
{
	if (m_nowWave == m_MaxWave)return true;
	return false;
}

void ObjectManager::TitleCameraWrite()
{
	if (m_titleCamera.expired())return;

	nlohmann::json _json;

	_json["TitleCamera"]["Name"] = "TitleCamera";
	_json["TitleCamera"]["PosX"] = m_titleCamera.lock()->GetPos().x;
	_json["TitleCamera"]["PosY"] = m_titleCamera.lock()->GetPos().y;
	_json["TitleCamera"]["PosZ"] = m_titleCamera.lock()->GetPos().z;
	_json["TitleCamera"]["DegAngX"] = m_titleCamera.lock()->GetDegAng().x;
	_json["TitleCamera"]["DegAngY"] = m_titleCamera.lock()->GetDegAng().y;
	_json["TitleCamera"]["DegAngZ"] = m_titleCamera.lock()->GetDegAng().z;

	std::ofstream _file("Json/Title/Camera/TitleCamera.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::TitleWrite()
{
	if (m_title.expired())return;

	nlohmann::json _json;

	_json["Title"]["Name"] = "Title";
	_json["Title"]["PosX"] = m_title.lock()->GetVector2Pos().x;
	_json["Title"]["PosY"] = m_title.lock()->GetVector2Pos().y;
	_json["Title"]["Size"] = m_title.lock()->GetSize();

	std::ofstream _file("Json/Title/Title/Title.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::GameWrite()
{
	if (m_game.expired())return;

	nlohmann::json _json;

	_json["Game"]["Name"] = "Game";
	_json["Game"]["PosX"] = m_game.lock()->GetVector2Pos().x;
	_json["Game"]["PosY"] = m_game.lock()->GetVector2Pos().y;
	_json["Game"]["Size"] = m_game.lock()->GetSize();

	std::ofstream _file("Json/Title/Game/Game.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::ExitWrite()
{
	if (m_exit.expired())return;

	nlohmann::json _json;

	_json["Exit"]["Name"] = "Exit";
	_json["Exit"]["PosX"] = m_exit.lock()->GetVector2Pos().x;
	_json["Exit"]["PosY"] = m_exit.lock()->GetVector2Pos().y;
	_json["Exit"]["Size"] = m_exit.lock()->GetSize();

	std::ofstream _file("Json/Title/Exit/Exit.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::TitleGuideWrite()
{
	if (m_titleGuide.expired())return;

	nlohmann::json _json;

	_json["TitleGuide"]["Name"] = "TitleGuide";
	_json["TitleGuide"]["PosX"] = m_titleGuide.lock()->GetVector2Pos().x;
	_json["TitleGuide"]["PosY"] = m_titleGuide.lock()->GetVector2Pos().y;
	_json["TitleGuide"]["Size"] = m_titleGuide.lock()->GetSize();

	std::ofstream _file("Json/Title/Guide/Guide.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::CursorWrite()
{
	if (m_cursor.expired())return;

	nlohmann::json _json;

	_json["Cursor"]["Name"] = "Title";
	_json["Cursor"]["MaxSize"] = m_cursor.lock()->GetMaxSize();
	_json["Cursor"]["ChangeSize"] = m_cursor.lock()->GetChangeSizeNum();
	_json["Cursor"]["MaxAlpha"] = m_cursor.lock()->GetMaxAlpha();
	_json["Cursor"]["ChangeAlpha"] = m_cursor.lock()->GetChangeAlphaNum();

	std::ofstream _file("Json/Title/Cursor/Cursor.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::GameCameraWrite()
{
	nlohmann::json _json;

	_json["Camera"]["Name"] = "Camera";
	_json["Camera"]["PlayerPosX"] = m_camera.lock()->GetLocalPos().x;
	_json["Camera"]["PlayerPosY"] = m_camera.lock()->GetLocalPos().y;
	_json["Camera"]["PlayerPosZ"] = m_camera.lock()->GetLocalPos().z;
	_json["Camera"]["FixedPosX"] = m_camera.lock()->GetFixedPos().x;
	_json["Camera"]["FixedPosY"] = m_camera.lock()->GetFixedPos().y;
	_json["Camera"]["FixedPosZ"] = m_camera.lock()->GetFixedPos().z;
	_json["Camera"]["AngleX"] = m_camera.lock()->GetFixedAngle().x;
	_json["Camera"]["AngleY"] = m_camera.lock()->GetFixedAngle().y;

	std::ofstream _file("Json/Game/Camera/Camera.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::PlayerWrite(std::string _fileName)
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
	

	std::ofstream _file(_fileName);
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::EnemyWrite(int _stage, int _wave,std::string _fileName)
{
	std::string _stagePath = (_fileName) + std::to_string(_stage).c_str() + ((std::string)".json");
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
		else if (enemy.lock()->GetName() == "Golem")
		{
			static int g = 0;
			_category = "Golem";
			_name = (std::to_string(g).c_str()) + ((std::string)"Golem");
			g++;
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

void ObjectManager::SwordWrite(std::string _swordName, std::string _fileName)
{
	nlohmann::json _json;
	std::ifstream _oldFile(_fileName);
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

	std::ofstream _newFile(_fileName);
	if (_newFile.is_open())
	{
		_newFile << _json.dump();
		_newFile.close();
	}
}

void ObjectManager::ShieldWrite(std::string _shieldName, std::string _fileName)
{
	nlohmann::json _json;
	std::ifstream _oldFile(_fileName);
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

	std::ofstream _newFile(_fileName);
	if (_newFile.is_open())
	{
		_newFile << _json.dump();
		_newFile.close();
	}
}

void ObjectManager::ObjectWrite(std::string _fileName)
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

	std::ofstream _file(_fileName);
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::SetTitleCamera()
{
	//jsonファイル
	std::string fileName = "Json/Title/Camera/TitleCamera.json";

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

		Math::Vector3 _DegAng = Math::Vector3::Zero;
		_DegAng.x = stage["DegAngX"];
		_DegAng.y = stage["DegAngY"];
		_DegAng.z = stage["DegAngZ"];

		std::string _name;
		_name = stage["Name"];
		std::shared_ptr<TitleCamera> camera = std::make_shared<TitleCamera>();

		camera->SetPos(_pos);
		camera->SetDegAng(_DegAng);
		camera->SetName(_name);
		camera->SetID(m_id);
		camera->SetObjectManager(shared_from_this());
		camera->Init();
		m_id++;

		m_titleCamera = camera;
		SceneManager::Instance().AddObject(camera);
		KdEffekseerManager::GetInstance().SetCamera(m_titleCamera.lock()->GetCamera());
	}

	ifs.close();
}

void ObjectManager::SetTitleParam()
{
	//jsonファイル
	std::string fileName = "Json/Title/Title/Title.json";

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& stage : _json)
	{
		Math::Vector2 _pos = Math::Vector2::Zero;
		_pos.x = stage["PosX"];
		_pos.y = stage["PosY"];

		float _size = 1.0f;
		_size = stage["Size"];

		std::string _name;
		_name = stage["Name"];
		std::shared_ptr<Title> title = std::make_shared<Title>();

		title->SetPos(_pos);
		title->SetSize(_size);
		title->SetName(_name);
		title->SetID(m_id);
		title->Init();
		m_id++;

		m_title = title;
		SceneManager::Instance().AddObject(title);
	}

	ifs.close();
}

void ObjectManager::SetGameParam()
{
	//jsonファイル
	std::string fileName = "Json/Title/Game/Game.json";

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& stage : _json)
	{
		Math::Vector2 _pos = Math::Vector2::Zero;
		_pos.x = stage["PosX"];
		_pos.y = stage["PosY"];

		float _size = 1.0f;
		_size = stage["Size"];

		std::string _name;
		_name = stage["Name"];
		std::shared_ptr<Game> game = std::make_shared<Game>();

		game->SetPos(_pos);
		game->SetSize(_size);
		game->SetName(_name);
		game->SetID(m_id);
		game->Init();
		m_id++;

		m_game = game;
		SceneManager::Instance().AddObject(game);
	}

	ifs.close();
}

void ObjectManager::SetExitParam()
{
	//jsonファイル
	std::string fileName = "Json/Title/Exit/Exit.json";

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& stage : _json)
	{
		Math::Vector2 _pos = Math::Vector2::Zero;
		_pos.x = stage["PosX"];
		_pos.y = stage["PosY"];

		float _size = 1.0f;
		_size = stage["Size"];

		std::string _name;
		_name = stage["Name"];
		std::shared_ptr<Exit> exit = std::make_shared<Exit>();

		exit->SetPos(_pos);
		exit->SetSize(_size);
		exit->SetName(_name);
		exit->SetID(m_id);
		exit->Init();
		m_id++;

		m_exit = exit;
		SceneManager::Instance().AddObject(exit);
	}

	ifs.close();
}

void ObjectManager::SetTitleGuideParam()
{
	//jsonファイル
	std::string fileName = "Json/Title/Guide/Guide.json";

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& stage : _json)
	{
		Math::Vector2 _pos = Math::Vector2::Zero;
		_pos.x = stage["PosX"];
		_pos.y = stage["PosY"];

		float _size = 1.0f;
		_size = stage["Size"];

		std::string _name;
		_name = stage["Name"];
		std::shared_ptr<TitleGuide> guide = std::make_shared<TitleGuide>();

		guide->SetPos(_pos);
		guide->SetSize(_size);
		guide->SetName(_name);
		guide->SetID(m_id);
		guide->Init();
		m_id++;

		m_titleGuide = guide;
		SceneManager::Instance().AddObject(guide);
	}

	ifs.close();
}

void ObjectManager::SetCursorParam()
{
	//jsonファイル
	std::string fileName = "Json/Title/Cursor/Cursor.json";

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& stage : _json)
	{
		float _MaxSize = 1.0f;
		_MaxSize = stage["MaxSize"];

		float _ChangeSize = 0.01f;
		_ChangeSize = stage["ChangeSize"];

		float _MaxAlpha = 1.0f;
		_MaxAlpha = stage["MaxAlpha"];

		float _ChangeAlpha = 0.01f;
		_ChangeAlpha = stage["ChangeAlpha"];

		std::string _name;
		_name = stage["Name"];
		std::shared_ptr<Cursor> cursor = std::make_shared<Cursor>();

		if (!m_game.expired())cursor->SetPosList(m_game.lock()->GetVector2Pos());
		if (!m_exit.expired())cursor->SetPosList(m_exit.lock()->GetVector2Pos());
		cursor->SetMaxSize(_MaxSize);
		cursor->SetChangeSizeNum(_ChangeSize);
		cursor->SetMaxAlpha(_MaxAlpha);
		cursor->SetChangeAlphaNum(_ChangeAlpha);
		cursor->SetName(_name);
		cursor->SetObjectManager(shared_from_this());
		cursor->SetID(m_id);
		cursor->Init();
		m_id++;

		m_cursor = cursor;
		SceneManager::Instance().AddObject(cursor);
	}

	ifs.close();
}

void ObjectManager::SetGameCameraParam()
{
	//jsonファイル
	std::string fileName = "Json/Game/Camera/Camera.json";

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
		camera->SetObjectManager(shared_from_this());
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
	std::string fileName = ("Json/") + m_nowScene + ("/Object/Object.json");

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
			obj->SetObjectManager(shared_from_this());
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
	std::string fileName = ("Json/") + m_nowScene + ("/Player/Player.json");

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

		SetWeaponParam((("Json/") + m_nowScene + ("/Weapon/Sword/Sword.json")), stage["SwordName"]);
		SetWeaponParam((("Json/") + m_nowScene + ("/Weapon/Shield/Shield.json")), stage["ShieldName"]);

		player->SetCamera(m_camera.lock());
		player->SetParam(_hp, player->GetSword().lock()->GetATK(), _speed, _stamina);
		player->SetPos(_pos);
		player->SetSize(_size);
		player->SetDir(_dir);
		player->SetAngle(_angleY);
		player->SetAtkRange(_atkRange);
		player->SetForward(_forward);
		player->SetObjectManager(shared_from_this());
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
		weapon->SetObjectManager(shared_from_this());
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
			else if (stage["Name"] == "Golem")
			{
				std::shared_ptr<Golem> golem = std::make_shared<Golem>();
				enemy = golem;
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
			enemy->SetObjectManager(shared_from_this());
			enemy->Init();
			m_id++;

			SceneManager::Instance().AddObject(enemy);
			m_EnemyList.push_back(enemy);
		}
	}
}

void ObjectManager::AddTitleCamera()
{
	if (!m_titleCamera.expired())return;

	std::string _name = "TitleCamera";
	Math::Vector3 _pos = Math::Vector3::Zero;
	Math::Vector3 _degAng = Math::Vector3::Zero;

	std::shared_ptr<TitleCamera> _camera = std::make_shared<TitleCamera>();
	_camera->SetPos(_pos);
	_camera->SetDegAng(_degAng);
	_camera->Init();

	m_titleCamera = _camera;
	SceneManager::Instance().AddObject(_camera);
}

void ObjectManager::AddTitle()
{
	if (!m_title.expired())return;

	std::string _name = "Title";
	Math::Vector2 _pos = Math::Vector2::Zero;
	float _size = 1.0f;

	std::shared_ptr<Title> _title = std::make_shared<Title>();
	_title->SetPos(_pos);
	_title->SetSize(_size);
	_title->Init();

	m_title = _title;
	SceneManager::Instance().AddObject(_title);
}

void ObjectManager::AddGame()
{
	if (!m_game.expired())return;

	std::string _name = "Game";
	Math::Vector2 _pos = Math::Vector2::Zero;
	float _size = 1.0f;

	std::shared_ptr<Game> _game = std::make_shared<Game>();
	_game->SetPos(_pos);
	_game->SetSize(_size);
	_game->Init();

	m_game = _game;
	SceneManager::Instance().AddObject(_game);
}

void ObjectManager::AddExit()
{
	if (!m_exit.expired())return;

	std::string _name = "Exit";
	Math::Vector2 _pos = Math::Vector2::Zero;
	float _size = 1.0f;

	std::shared_ptr<Exit> _exit = std::make_shared<Exit>();
	_exit->SetPos(_pos);
	_exit->SetSize(_size);
	_exit->Init();

	m_exit = _exit;
	SceneManager::Instance().AddObject(_exit);
}

void ObjectManager::AddTitleGuide()
{
	if (!m_titleGuide.expired())return;

	std::string _name = "TitleGuide";
	Math::Vector2 _pos = Math::Vector2::Zero;
	float _size = 1.0f;

	std::shared_ptr<TitleGuide> _guide = std::make_shared<TitleGuide>();
	_guide->SetPos(_pos);
	_guide->SetSize(_size);
	_guide->Init();

	m_titleGuide = _guide;
	SceneManager::Instance().AddObject(_guide);
}

void ObjectManager::AddCursor()
{
	if (!m_cursor.expired())return;

	std::string _name = "Cursor";
	float _MaxSize = 1.0f;
	float _ChangeSize = 0.01f;
	float _MaxAlpha = 1.0f;
	float _ChangeAlpha = 0.01f;

	std::shared_ptr<Cursor> _cursor = std::make_shared<Cursor>();
	if (!m_game.expired())_cursor->SetPosList(m_game.lock()->GetVector2Pos());
	if (!m_exit.expired())_cursor->SetPosList(m_exit.lock()->GetVector2Pos());
	_cursor->SetMaxSize(_MaxSize);
	_cursor->SetChangeSizeNum(_ChangeSize);
	_cursor->SetMaxAlpha(_MaxAlpha);
	_cursor->SetChangeAlphaNum(_ChangeAlpha);
	_cursor->Init();

	m_cursor = _cursor;
	SceneManager::Instance().AddObject(_cursor);
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

void ObjectManager::AddGolem()
{
	std::string _name = "Golem";
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

	std::shared_ptr<Golem> enemy = nullptr;
	enemy = std::make_shared<Golem>();
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

void ObjectManager::AddSkyBox()
{
	std::string _name = "SkyBox";
	Math::Vector3 _pos = Math::Vector3::Zero;
	float _size = 10.0f;
	float _angleY = 0.0f;
	std::shared_ptr<SkyBox> obj = std::make_shared<SkyBox>();

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
