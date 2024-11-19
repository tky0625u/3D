#include "ObjectManager.h"
#include"../Scene/SceneManager.h"
#include"../tinygltf/json.hpp"
#include"Character/CharacterBase.h"
#include"Character/Enemy/Bone/Bone.h"
#include"Character/Action/Enemy/Enemy_ConText.h"
#include"Camera/GameCamera/GameCamera_ConText.h"
#include"StageManager.h"
#include<fstream>
#include<sstream>

//地面
#include"Stage/Ground/Ground.h"
//魔法陣の台
#include"Stage/Circle/Circle.h"
//魔法陣
#include"Stage/MagicPolygon/MagicPolygon.h"
#include"Stage/MagicPolygon/MagicPolygon_ConText.h"
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
#include"Character/Action/Player/Player_ConText.h"
//ゲームカメラ
#include"Camera/GameCamera/GameCamera.h"
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
//プレイヤーHP
#include"UI/Player/HP/Player_HP.h"
//プレイヤースタミナ
#include"UI/Player/Stamina/Player_Stamina.h"
//ロックオン
#include"UI/Player/LockON/LockON.h"
//階数
#include"UI/Player/Floor/Floor.h"
//テレポート
#include"UI/Player/Teleport/Teleport.h"
//敵HP
#include"UI/Enemy/HP/Enemy_HP.h"
//ゲームオーバー
#include"UI/Player/GameOver/GameOver.h"
//弾
#include"Character/Enemy/Golem/Bullet/Bullet.h"

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
	auto hp = m_EnemyHPList.begin();

	while (hp != m_EnemyHPList.end())
	{
		if (hp->expired())
		{
			hp = m_EnemyHPList.erase(hp);
		}
		else
		{
			++hp;
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

void ObjectManager::NextStageLiberation()
{
	m_player.lock()->GetConText()->Idol();
	m_magic.lock()->GetConText()->Next();
	m_camera.lock()->GetConText()->FixedCamera();
}

void ObjectManager::GameClear()
{
	m_player.lock()->GetConText()->Idol();
	m_camera.lock()->GetConText()->ClearCamera();
}

void ObjectManager::NextTeleport()
{
	//m_magic.lock()->GetConText()->Teleport();
	m_player.lock()->GetConText()->Teleport();
}

void ObjectManager::CreateStage(std::shared_ptr<StageManager> _stage)
{
	//jsonファイル
	std::string fileName = ("Asset/Json/") + m_nowScene + ("/Object/Object.json");

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	int _stageNum = 1;
	for (auto& stage : _json["Ground"])
	{
		if (_stage->GetnowStage() != _stageNum)
		{
			_stageNum++;
			continue;
		}
		Math::Vector3 _pos = Math::Vector3::Zero;
		_pos.x = stage["PosX"];
		_pos.y = stage["PosY"];
		_pos.z = stage["PosZ"];

		float _size = 0.0f;
		_size = stage["Size"];

		Math::Vector3 _angle = Math::Vector3::Zero;
		_angle.y = stage["Angle"];

		std::string _name;
		_name = stage["Name"];

		if (!m_ground.expired())
		{
			m_ground.lock()->SetPos(_pos);
			m_ground.lock()->SetSize(_size);
			m_ground.lock()->SetAngle(_angle);
		}

		break;
	}

	Math::Matrix _Scale;
	Math::Matrix _Rot;
	Math::Matrix _Trans;

	_Scale = Math::Matrix::CreateScale(m_ground.lock()->GetSize());
	_Rot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_ground.lock()->GetAngle().y));
	_Trans = Math::Matrix::CreateTranslation(m_ground.lock()->GetPos());
	Math::Matrix _GroundMat = _Scale * _Rot * _Trans;
	m_ground.lock()->SetMatrix(_GroundMat);

	_Scale = Math::Matrix::CreateScale(m_circle.lock()->GetSize());
	_Rot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_circle.lock()->GetAngle().y));
	_Trans = Math::Matrix::CreateTranslation(m_circle.lock()->GetPos());
	Math::Matrix _CircleMat = _Scale * _Rot * _Trans * _GroundMat;
	m_circle.lock()->SetMatrix(_CircleMat);

	_Scale = Math::Matrix::CreateScale(m_magic.lock()->GetSize());
	_Rot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_magic.lock()->GetAngle().y));
	_Trans = Math::Matrix::CreateTranslation(m_magic.lock()->GetPos());
	Math::Matrix _MagicMat = _Scale * _Rot * _Trans * _CircleMat;
	m_magic.lock()->SetMatrix(_MagicMat);

	ifs.close();

	std::string _filePath = ("Asset/Json/Game/Enemy/Stage") + (std::to_string(_stage->GetnowStage())) + (".json");
	SetEnemyParam(_filePath, _stage);

	m_player.lock()->GetConText()->GetState()->SetFlow(ActionBase::Flow::EndType);
	m_player.lock()->SetPos(m_ground.lock()->GetPos());
	m_player.lock()->SetTeleportFlg(false);

	m_magic.lock()->GetConText()->Normal();
}

void ObjectManager::DebugObject(std::shared_ptr<StageManager> _stage)
{
	// FPS
//ImGui::Text("FPS : %d", m_fpsController.m_nowfps);

	static bool stop = false;
	ImGui::Checkbox((const char*)u8"ストップ", &stop);
	SceneManager::Instance().m_stop = stop;

	std::string _filePath;

	switch (SceneManager::Instance().GetNowSceneType())
	{
	case SceneManager::SceneType::Title:
		_filePath = "Title";
		break;
	case SceneManager::SceneType::Game:
		_filePath = "Game";
		break;
	default:
		break;
	}

	if (ImGui::TreeNode("Title"))
	{
		if (ImGui::TreeNode("Camera"))
		{
			if (ImGui::Button((const char*)u8"Camera保存"))
			{
				TitleCameraWrite();
			}

			if (ImGui::Button((const char*)u8"Camera追加"))
			{
				AddTitleCamera();
			}

			if (m_titleCamera.expired() == false)
			{
				std::shared_ptr<TitleCamera> _camera = m_titleCamera.lock();

				// 位置
				ImGui::Text((const char*)u8"　カメラ位置 　　x=%.2f,y=%.2f,z=%.2f", _camera->GetPos().x, _camera->GetPos().y, _camera->GetPos().z);
				Math::Vector3 _TitleCameraPos = _camera->GetPos();
				ImGui::SliderFloat("TitleCameraPosX", &_TitleCameraPos.x, -500, 500);
				ImGui::SliderFloat("TitleCameraPosY", &_TitleCameraPos.y, -500, 500);
				ImGui::SliderFloat("TitleCameraPosZ", &_TitleCameraPos.z, -500, 500);

				// 角度
				ImGui::Text((const char*)u8"　角度 　　DegAngX=%.2f, DegAngY=%.2f, DegAngZ=%.2f", _camera->GetDegAng().x, _camera->GetDegAng().y, _camera->GetDegAng().z);
				float _DegAngX = _camera->GetDegAng().x;
				float _DegAngY = _camera->GetDegAng().y;
				float _DegAngZ = _camera->GetDegAng().z;
				ImGui::SliderFloat("AngleX", &_DegAngX, -180, 180);
				ImGui::SliderFloat("AngleY", &_DegAngY, -180, 180);
				ImGui::SliderFloat("AngleZ", &_DegAngZ, -180, 180);

				_camera->SetPos(_TitleCameraPos);
				_camera->SetDegAng(Math::Vector3{ _DegAngX,_DegAngY,_DegAngZ });
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("TitleUI"))
		{
			if (ImGui::TreeNode("Title"))
			{
				if (ImGui::Button((const char*)u8"Title保存"))
				{
					TitleWrite();
				}

				if (ImGui::Button((const char*)u8"Title追加"))
				{
					AddTitle();
				}

				if (m_title.expired() == false)
				{
					std::shared_ptr<Title> _title = m_title.lock();

					// 位置
					ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f", _title->GetVector2Pos().x, _title->GetVector2Pos().y);
					Math::Vector2 _TitlePos = _title->GetVector2Pos();
					ImGui::SliderFloat("PosX", &_TitlePos.x, -640, 640);
					ImGui::SliderFloat("PosY", &_TitlePos.y, -360, 360);

					// 大きさ
					ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _title->GetSize());
					float _TitleSize = _title->GetSize();
					ImGui::SliderFloat("Size", &_TitleSize, 1, 100);

					_title->SetPos(_TitlePos);
					_title->SetSize(_TitleSize);
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Game"))
			{
				if (ImGui::Button((const char*)u8"Game保存"))
				{
					GameWrite();
				}

				if (ImGui::Button((const char*)u8"Game追加"))
				{
					AddGame();
				}

				if (m_game.expired() == false)
				{
					std::shared_ptr<Game> _game = m_game.lock();

					// 位置
					ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f", _game->GetVector2Pos().x, _game->GetVector2Pos().y);
					Math::Vector2 _GamePos = _game->GetVector2Pos();
					ImGui::SliderFloat("PosX", &_GamePos.x, -640, 640);
					ImGui::SliderFloat("PosY", &_GamePos.y, -360, 360);

					// 大きさ
					ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _game->GetSize());
					float _GameSize = _game->GetSize();
					ImGui::SliderFloat("Size", &_GameSize, 0.01, 1);

					_game->SetPos(_GamePos);
					_game->SetSize(_GameSize);
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Exit"))
			{
				if (ImGui::Button((const char*)u8"Exit保存"))
				{
					ExitWrite();
				}

				if (ImGui::Button((const char*)u8"Exit追加"))
				{
					AddExit();
				}

				if (m_exit.expired() == false)
				{
					std::shared_ptr<Exit> _exit = m_exit.lock();

					// 位置
					ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f", _exit->GetVector2Pos().x, _exit->GetVector2Pos().y);
					Math::Vector2 _ExitPos = _exit->GetVector2Pos();
					ImGui::SliderFloat("PosX", &_ExitPos.x, -640, 640);
					ImGui::SliderFloat("PosY", &_ExitPos.y, -360, 360);

					// 大きさ
					ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _exit->GetSize());
					float _ExitSize = _exit->GetSize();
					ImGui::SliderFloat("Size", &_ExitSize, 0.01, 1);

					_exit->SetPos(_ExitPos);
					_exit->SetSize(_ExitSize);
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Guide"))
			{
				if (ImGui::Button((const char*)u8"Guide保存"))
				{
					TitleGuideWrite();
				}

				if (ImGui::Button((const char*)u8"Guide追加"))
				{
					AddTitleGuide();
				}

				if (m_titleGuide.expired() == false)
				{
					std::shared_ptr<TitleGuide> _guide = m_titleGuide.lock();

					// 位置
					ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f", _guide->GetVector2Pos().x, _guide->GetVector2Pos().y);
					Math::Vector2 _TitleGuidePos = _guide->GetVector2Pos();
					ImGui::SliderFloat("PosX", &_TitleGuidePos.x, -640, 640);
					ImGui::SliderFloat("PosY", &_TitleGuidePos.y, -360, 360);

					// 大きさ
					ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _guide->GetSize());
					float _TitleGuideSize = _guide->GetSize();
					ImGui::SliderFloat("Size", &_TitleGuideSize, 0.01, 1);

					_guide->SetPos(_TitleGuidePos);
					_guide->SetSize(_TitleGuideSize);
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Cursor"))
			{
				if (ImGui::Button((const char*)u8"Cursor保存"))
				{
					CursorWrite();
				}

				if (ImGui::Button((const char*)u8"Cursor追加"))
				{
					AddCursor();
				}

				if (m_cursor.expired() == false)
				{
					std::shared_ptr<Cursor> _cursor = m_cursor.lock();

					// 最大サイズ
					ImGui::Text((const char*)u8"　最大サイズ 　MaxSize=%.2f", _cursor->GetMaxSize());
					float _CursorMaxSize = _cursor->GetMaxSize();
					ImGui::SliderFloat("MaxSize", &_CursorMaxSize, 0.01, 1);

					// 大きさ変化量
					ImGui::Text((const char*)u8"　大きさ変化量 　ChangeSizeNum=%.2f", _cursor->GetChangeSizeNum());
					float _CursorChangeSize = _cursor->GetChangeSizeNum();
					ImGui::SliderFloat("ChangeSize", &_CursorChangeSize, 0.01, 1);

					// 最大アルファ値
					ImGui::Text((const char*)u8"　最大アルファ値 　MaxAlpha=%.2f", _cursor->GetMaxAlpha());
					float _CursorMaxAlpha = _cursor->GetMaxAlpha();
					ImGui::SliderFloat("MaxAlpha", &_CursorMaxAlpha, 0.01, 1);

					// アルファ値変化量
					ImGui::Text((const char*)u8"　アルファ値変化量 　ChangeAlphaNum=%.2f", _cursor->GetChangeAlphaNum());
					float _CursorChangeAlpha = _cursor->GetChangeAlphaNum();
					ImGui::SliderFloat("ChangeAlpha", &_CursorChangeAlpha, 0.01, 1);

					_cursor->SetMaxSize(_CursorMaxSize);
					_cursor->SetChangeSizeNum(_CursorChangeSize);
					_cursor->SetMaxAlpha(_CursorMaxAlpha);
					_cursor->SetChangeAlphaNum(_CursorChangeAlpha);
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Game"))
	{
		if (ImGui::TreeNode("Camera"))
		{
			if (ImGui::Button((const char*)u8"Camera保存"))
			{
				GameCameraWrite();
			}


			if (m_camera.expired() == false)
			{
				std::shared_ptr<GameCamera> _camera = m_camera.lock();

				if (_camera->GetCameraType() != GameCamera::CameraType::PlayerType)
				{
					if (ImGui::Button((const char*)u8"Player"))
					{
						_camera->SetCameraType(GameCamera::CameraType::PlayerType);
						_camera->GetConText()->PlayerCamera();
					}
				}
				if (_camera->GetCameraType() != GameCamera::CameraType::FixedType)
				{
					if (ImGui::Button((const char*)u8"Fixed"))
					{
						_camera->SetCameraType(GameCamera::CameraType::FixedType);
						_camera->GetConText()->FixedCamera();
					}
				}
				if (_camera->GetCameraType() != GameCamera::CameraType::ClearType)
				{
					if (ImGui::Button((const char*)u8"Clear"))
					{
						_camera->SetCameraType(GameCamera::CameraType::ClearType);
						_camera->GetConText()->ClearCamera();
					}
				}

				// 位置
				ImGui::Text((const char*)u8"　カメラ位置 　　x=%.2f,y=%.2f,z=%.2f", _camera->GetNowPos().x, _camera->GetNowPos().y, _camera->GetNowPos().z);
				Math::Vector3 pos = _camera->GetNowPos();
				ImGui::SliderFloat("PosX", &pos.x, -10, 10);
				ImGui::SliderFloat("PosY", &pos.y, -10, 10);
				ImGui::SliderFloat("PosZ", &pos.z, -100, 0);

				// 角度
				ImGui::Text((const char*)u8"　角度 　　AngleX=%.2f, AngleY=%.2f", _camera->GetNowDegAng().x, _camera->GetNowDegAng().y);
				float angleX = _camera->GetNowDegAng().x;
				float angleY = _camera->GetNowDegAng().y;

				ImGui::SliderFloat("AngleX", &angleX, -180.0f, 180.0f);
				ImGui::SliderFloat("AngleY", &angleY, 0, 360);

				if (angleX > 180.0f)angleX -= 180.0f;
				else if (angleX < -180.0f)angleX += 180.0f;
				if (angleY > 360.0f)angleY -= 360.0f;
				if (angleY < 0.0f)angleY += 360.0f;

				// 視野角
				ImGui::Text((const char*)u8"　視野角 　　ViewAngleX=%.2f", _camera->GetNowViewAng());
				float ViewAngle = _camera->GetNowViewAng();

				ImGui::SliderFloat("ViewAngle", &ViewAngle, 0, 360);

				_camera->SetPos(pos);
				_camera->SetDegAng(Math::Vector3{ angleX,angleY,0.0f });
				_camera->SetViewAng(ViewAngle);
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Player"))
		{
			if (ImGui::Button((const char*)u8"Player保存"))
			{
				PlayerWrite(("Asset/Json/") + _filePath + ("/Player/Player.json"));
			}

			if (m_player.expired() == false)
			{
				std::shared_ptr<Player> _player = m_player.lock();

				// 体力
				ImGui::Text((const char*)u8"　体力 　　HP=%d", _player->GetParam().Hp);
				int hp = _player->GetParam().Hp;
				ImGui::SliderInt("HP", &hp, 1, 100);
				// 攻撃力
				ImGui::Text((const char*)u8"　攻撃力 　ATK=%d", _player->GetParam().Atk);
				// 素早さ
				ImGui::Text((const char*)u8"　素早さ 　SP=%.2f", _player->GetParam().Sp);
				float speed = _player->GetParam().Sp;
				ImGui::SliderFloat("Speed", &speed, 1, 100);
				// スタミナ
				ImGui::Text((const char*)u8"　スタミナ SM=%d", _player->GetParam().Sm);
				int stamina = _player->GetParam().Sm;
				ImGui::SliderInt("Stamina", &stamina, 1, 100);
				// 位置
				ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", _player->GetPos().x, _player->GetPos().y, _player->GetPos().z);
				Math::Vector3 pos = _player->GetPos();
				ImGui::SliderFloat("PosX", &pos.x, -100, 100);
				ImGui::SliderFloat("PosY", &pos.y, 0, 400);
				ImGui::SliderFloat("PosZ", &pos.z, -100, 100);
				// 方向
				ImGui::Text((const char*)u8"　方向 　　x=%.2f,y=%.2f,z=%.2f", _player->GetDir().x, _player->GetDir().y, _player->GetDir().z);
				// 角度
				ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", _player->GetAngle());
				Math::Vector3 angle = _player->GetAngle();
				ImGui::SliderFloat("Angle", &angle.y, 0, 360);
				// 大きさ
				ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _player->GetSize());
				float size = _player->GetSize();
				ImGui::SliderFloat("Size", &size, 1, 100);
				// 攻撃範囲
				ImGui::Text((const char*)u8"　攻撃範囲 ATKRange=%.2f", _player->GetAtkRange());
				float range = _player->GetAtkRange();
				ImGui::SliderFloat("ATKRange", &range, 1, 100);
				// 前方方向
				ImGui::Text((const char*)u8"　前方方向 x=%.2f,y=%.2f,z=%.2f", _player->GetForward().x, _player->GetForward().y, _player->GetForward().z);
				// 無敵時間
				ImGui::Text((const char*)u8"　無敵付与時間 InviTime=%d", _player->GetinviTime());
				int _inviTime = _player->GetinviTime();
				ImGui::SliderInt("InviTIme", &_inviTime, 0, 300);

				// 武器
				static std::string _swordName = _player->GetSword().lock()->GetName().c_str();
				static std::string _shieldName = _player->GetShield().lock()->GetName().c_str();
				if (ImGui::TreeNode("Weapon"))
				{
					// 剣
					if (_player->GetSword().expired() == false)
					{
						std::shared_ptr<Sword> _sword = _player->GetSword().lock();
						static Math::Vector3 swordPos = _sword->GetPos();
						static int swordAtk = _sword->GetATK();
						static float swordSize = _sword->GetSize();
						static int traje = _sword->GetTraject();
						if (ImGui::TreeNode("Sword"))
						{
							if (ImGui::Button((const char*)u8"Sword保存"))
							{
								SwordWrite(_swordName, (("Asset/Json/") + _filePath + ("/Sword/Sword.json")));
							}

							ImGui::Text((const char*)u8"　剣 Sword=%s", _player->GetSword().lock()->GetName().c_str());
							for (auto& sword : m_swordNameList)
							{
								if (ImGui::Button(sword.c_str()))
								{
									_swordName = sword;
								}
							}

							// 位置
							ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", _sword->GetPos().x, _sword->GetPos().y, _sword->GetPos().z);
							swordPos = _sword->GetPos();
							ImGui::SliderFloat("PosX", &swordPos.x, -0.5, 0.5);
							ImGui::SliderFloat("PosY", &swordPos.y, -0.5, 0.5);
							ImGui::SliderFloat("PosZ", &swordPos.z, -0.5, 0.5);
							// 攻撃力
							ImGui::Text((const char*)u8"　攻撃力 　ATK=%d", _sword->GetATK());
							swordAtk = _sword->GetATK();
							ImGui::SliderInt("ATK", &swordAtk, 1, 100);
							// 大きさ
							ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _sword->GetSize());
							swordSize = _sword->GetSize();
							ImGui::SliderFloat("Size", &swordSize, 1, 100);
							// トレイルポリゴン
							ImGui::Text((const char*)u8"　トレイルポリゴン　Traject=%d", _sword->GetTraject());
							traje = _sword->GetTraject();
							ImGui::SliderInt("Traject", &traje, 1, 100);

							_sword->SetPos(swordPos);
							_sword->SetATK(swordAtk);
							_sword->SetSize(swordSize);
							_sword->SetTrajectPointNUM(traje);

							ImGui::TreePop();
						}
					}

					// 盾
					if (_player->GetShield().expired() == false)
					{
						if (ImGui::TreeNode("Shield"))
						{
							std::shared_ptr<Shield> _shield = _player->GetShield().lock();
							static Math::Vector3 shieldPos = _shield->GetPos();
							static float shieldSize = _shield->GetSize();
							if (ImGui::Button((const char*)u8"Shield保存"))
							{
								ShieldWrite(_shieldName, (("Asset/Json/") + _filePath + ("/Shield/Shield.json")));
							}

							ImGui::Text((const char*)u8"　盾 Shield=%s", _player->GetShield().lock()->GetName().c_str());
							for (auto& shield : m_shieldNameList)
							{
								if (ImGui::Button(shield.c_str()))
								{
									_shieldName = shield;
								}
							}

							// 位置
							ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", _shield->GetPos().x, _shield->GetPos().y, _shield->GetPos().z);
							shieldPos = _shield->GetPos();
							ImGui::SliderFloat("PosX", &shieldPos.x, -0.5, 0.5);
							ImGui::SliderFloat("PosY", &shieldPos.y, -0.5, 0.5);
							ImGui::SliderFloat("PosZ", &shieldPos.z, -0.5, 0.5);

							// 大きさ
							ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _shield->GetSize());
							shieldSize = _shield->GetSize();
							ImGui::SliderFloat("Size", &shieldSize, 1, 100);

							_shield->SetPos(shieldPos);
							_shield->SetSize(shieldSize);

							ImGui::TreePop();
						}
					}

					ImGui::TreePop();

					ChangeWeapon(_swordName, _shieldName);
				}

				_player->SetParam(hp, _player->GetSword().lock()->GetATK(), speed, stamina);
				_player->SetPos(pos);
				_player->SetAngle(angle);
				_player->SetSize(size);
				_player->SetAtkRange(range);
				_player->SetInviTime(_inviTime);

				if (ImGui::TreeNode("UI"))
				{
					if (ImGui::TreeNode("HP"))
					{
						if (ImGui::Button((const char*)u8"HP保存"))
						{
							PlayerHPWrite();
						}

						if (m_PlayerHP.expired() == false)
						{
							std::shared_ptr<Player_HP> _hp = m_PlayerHP.lock();

							// 位置
							ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f", _hp->GetVector2Pos().x, _hp->GetVector2Pos().y);
							Math::Vector2 _HPPos = _hp->GetVector2Pos();
							ImGui::SliderFloat("PosX", &_HPPos.x, -640, 640);
							ImGui::SliderFloat("PosY", &_HPPos.y, -360, 360);

							// 大きさ
							ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _hp->GetSize());
							float _HPSize = _hp->GetSize();
							ImGui::SliderFloat("Size", &_HPSize, 1, 100);

							_hp->SetPos(_HPPos);
							_hp->SetSize(_HPSize);
						}

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("Stamina"))
					{
						if (ImGui::Button((const char*)u8"Stamina保存"))
						{
							PlayerStaminaWrite();
						}

						if (m_PlayerStamina.expired() == false)
						{
							std::shared_ptr<Player_Stamina> _stamina = m_PlayerStamina.lock();

							// 位置
							ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f", _stamina->GetVector2Pos().x, _stamina->GetVector2Pos().y);
							Math::Vector2 _StaminaPos = _stamina->GetVector2Pos();
							ImGui::SliderFloat("PosX", &_StaminaPos.x, -640, 640);
							ImGui::SliderFloat("PosY", &_StaminaPos.y, -360, 360);

							// 大きさ
							ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _stamina->GetSize());
							float _StaminaSize = _stamina->GetSize();
							ImGui::SliderFloat("Size", &_StaminaSize, 1, 100);

							_stamina->SetPos(_StaminaPos);
							_stamina->SetSize(_StaminaSize);
						}

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("LockON"))
					{
						if (ImGui::Button((const char*)u8"LockON保存"))
						{
							LockONWrite();
						}

						if (m_lockON.expired() == false)
						{
							std::shared_ptr<LockON> _lock = m_lockON.lock();

							// 位置
							ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f", _lock->GetVector2Pos().x, _lock->GetVector2Pos().y);
							Math::Vector2 _LockPos = _lock->GetVector2Pos();
							ImGui::SliderFloat("PosX", &_LockPos.x, -640, 640);
							ImGui::SliderFloat("PosY", &_LockPos.y, -360, 360);

							// 大きさ
							ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _lock->GetSize());
							float _LockSize = _lock->GetSize();
							ImGui::SliderFloat("Size", &_LockSize, 1, 100);

							// 大きさ変化量
							ImGui::Text((const char*)u8"　大きさ変化量 　ChangeSize=%.2f", _lock->GetChangeSize());
							float _LockSizeChange = _lock->GetChangeSize();
							ImGui::SliderFloat("SizeChange", &_LockSizeChange, 0.01, 1.00);

							// アルファ値変化量
							ImGui::Text((const char*)u8"　アルファ値変化量 　ChangeAlpha=%.2f", _lock->GetChangeAlpha());
							float _LockAlphaChange = _lock->GetChangeAlpha();
							ImGui::SliderFloat("AlphaChange", &_LockAlphaChange, 0.01, 1.00);

							_lock->SetPos(_LockPos);
							_lock->SetSize(_LockSize);
							_lock->SetChangeSize(_LockSizeChange);
							_lock->SetChangeAlpha(_LockAlphaChange);
						}

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("Floor"))
					{
						if (ImGui::Button((const char*)u8"Floor保存"))
						{
							FloorWrite();
						}

						if (m_floor.expired() == false)
						{
							std::shared_ptr<Floor> _floor = m_floor.lock();

							// 位置
							ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f", _floor->GetVector2Pos().x, _floor->GetVector2Pos().y);
							Math::Vector2 _FloorPos = _floor->GetVector2Pos();
							ImGui::SliderFloat("PosX", &_FloorPos.x, -640, 640);
							ImGui::SliderFloat("PosY", &_FloorPos.y, -360, 360);

							// 大きさ
							ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _floor->GetSize());
							float _FloorSize = _floor->GetSize();
							ImGui::SliderFloat("Size", &_FloorSize, 1, 100);

							_floor->SetPos(_FloorPos);
							_floor->SetSize(_FloorSize);
						}

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("Teleport"))
					{
						if (ImGui::Button((const char*)u8"Teleport保存"))
						{
							TeleportWrite();
						}

						if (m_teleport.expired() == false)
						{
							std::shared_ptr<Teleport> _teleport = m_teleport.lock();

							// 位置
							ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f", _teleport->GetVector2Pos().x, _teleport->GetVector2Pos().y);
							Math::Vector2 _TeleportPos = _teleport->GetVector2Pos();
							ImGui::SliderFloat("PosX", &_TeleportPos.x, -640, 640);
							ImGui::SliderFloat("PosY", &_TeleportPos.y, -360, 360);

							// 大きさ
							ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _teleport->GetSize());
							float _TeleportSize = _teleport->GetSize();
							ImGui::SliderFloat("Size", &_TeleportSize, 1, 100);

							// アルファ値変化量
							ImGui::Text((const char*)u8"　アルファ値変化量 　ChangeAlpha=%.2f", _teleport->GetChangeAlpha());
							float _TeleportAlphaChange = _teleport->GetChangeAlpha();
							ImGui::SliderFloat("AlphaChange", &_TeleportAlphaChange, 0.01, 1.00);

							_teleport->SetPos(_TeleportPos);
							_teleport->SetSize(_TeleportSize);
							_teleport->SetChangeAlpha(_TeleportAlphaChange);
						}

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("GameOver"))
					{
						if (ImGui::Button((const char*)u8"GameOver保存"))
						{
							GameOverWrite();
						}

						if (m_gameOver.expired() == false)
						{
							std::shared_ptr<GameOver> _over = m_gameOver.lock();

							// 位置
							ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f", _over->GetVector2Pos().x, _over->GetVector2Pos().y);
							Math::Vector2 _GameOverPos = _over->GetVector2Pos();
							ImGui::SliderFloat("PosX", &_GameOverPos.x, -640, 640);
							ImGui::SliderFloat("PosY", &_GameOverPos.y, -360, 360);

							// 大きさ
							ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _over->GetSize());
							float _GameOverSize = _over->GetSize();
							ImGui::SliderFloat("Size", &_GameOverSize, 1, 100);

							// アルファ値変化量
							ImGui::Text((const char*)u8"　アルファ値変化量 　ChangeAlpha=%.2f", _over->GetAlphaChange());
							float _GameOverAlphaChange = _over->GetAlphaChange();
							ImGui::SliderFloat("AlphaChange", &_GameOverAlphaChange, 0.01, 1.00);

							_over->SetPos(_GameOverPos);
							_over->SetSize(_GameOverSize);
							_over->SetChangeAlpha(_GameOverAlphaChange);
						}

						ImGui::TreePop();
					}

					ImGui::TreePop();
				}
				ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");

				ImGui::TreePop();
			}
		}

		if (ImGui::TreeNode("Enemy"))
		{
			std::vector<std::weak_ptr<EnemyBase>> _boneList;
			std::vector<std::weak_ptr<EnemyBase>> _golemList;
			for (auto& enemy : m_EnemyList)
			{
				if (enemy.expired() == false)
				{
					if (enemy.lock()->GetName() == "Bone")
					{
						_boneList.push_back(enemy);
					}
					if (enemy.lock()->GetName() == "Golem")
					{
						_golemList.push_back(enemy);
					}
				}
			}

			static int _stageNum = 1;
			ImGui::Text((const char*)u8"ステージ数 : %d", _stageNum);
			static int _nowStage = _stage->GetnowStage();
			ImGui::SliderInt((const char*)u8"ステージ", &_nowStage, 1, _stageNum);
			if (ImGui::Button((const char*)u8"ステージ追加"))
			{
				_stageNum++;
				if (_stageNum - 1 == _nowStage)_nowStage = _stageNum;
			}

			static int _wave = _stage->GetMaxWave();
			ImGui::Text((const char*)u8"ウェーブ数 : %d", _wave);
			static int _nowWave = _stage->GetnowWave();
			ImGui::SliderInt((const char*)u8"ウェーブ", &_nowWave, 1, _wave);
			if (ImGui::Button((const char*)u8"ウェーブ追加"))
			{
				_wave++;
				if (_wave - 1 == _nowWave)_nowWave = _wave;
			}
			if (ImGui::Button((const char*)u8"ウェーブ削除"))
			{
				if (_wave > 1)
				{
					_wave--;
					if (_wave + 1 == _nowWave)_nowWave = _wave;
				}
			}

			if (ImGui::Button((const char*)u8"Enemy保存"))
			{
				EnemyWrite(_nowStage, _nowWave, (("Asset/Json/") + _filePath + ("/Enemy/Stage")));
				if (_nowWave == _wave)
				{
					_wave++;
					_nowWave = _wave;
				}
			}

			if (ImGui::TreeNode("Bone"))
			{
				if (ImGui::Button((const char*)u8"Bone追加"))
				{
					AddBone();
				}

				static int operation = -1;
				if (!SceneManager::Instance().m_stop)operation = -1;

				if (ImGui::TreeNode("Bone"))
				{
					ImGui::Text((const char*)u8"ボーン:%d体", _boneList.size());
					for (int bone = 0; bone < _boneList.size(); ++bone)
					{
						if (ImGui::Button(std::to_string(bone + 1).c_str()))
						{
							operation = bone;
						}
					}

					if (operation != -1)
					{
						ImGui::Text((const char*)u8"%d体目", operation + 1);
						// 体力
						ImGui::Text((const char*)u8"　体力 　　HP=%d", _boneList[operation].lock()->GetParam().Hp);
						int hp = _boneList[operation].lock()->GetParam().Hp;
						ImGui::SliderInt("HP", &hp, 1, 100);
						// 攻撃力
						ImGui::Text((const char*)u8"　攻撃力 　ATK=%d", _boneList[operation].lock()->GetParam().Atk);
						int atk = _boneList[operation].lock()->GetParam().Atk;
						ImGui::SliderInt("ATK", &atk, 1, 100);
						// 素早さ
						ImGui::Text((const char*)u8"　素早さ 　SP=%.2f", _boneList[operation].lock()->GetParam().Sp);
						float speed = _boneList[operation].lock()->GetParam().Sp;
						ImGui::SliderFloat("Speed", &speed, 1, 100);
						// スタミナ
						ImGui::Text((const char*)u8"　スタミナ SM=%d", _boneList[operation].lock()->GetParam().Sm);
						int stamina = _boneList[operation].lock()->GetParam().Sm;
						ImGui::SliderInt("Stamina", &stamina, 1, 100);
						// 位置
						ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", _boneList[operation].lock()->GetPos().x, _boneList[operation].lock()->GetPos().y, _boneList[operation].lock()->GetPos().z);
						Math::Vector3 pos = _boneList[operation].lock()->GetPos();
						ImGui::SliderFloat("PosX", &pos.x, -100, 100);
						ImGui::SliderFloat("PosY", &pos.y, 0, 400);
						ImGui::SliderFloat("PosZ", &pos.z, -100, 100);
						// 方向
						ImGui::Text((const char*)u8"　方向 　　x=%.2f,y=%.2f,z=%.2f", _boneList[operation].lock()->GetDir().x, _boneList[operation].lock()->GetDir().y, _boneList[operation].lock()->GetDir().z);
						// 角度
						ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", _boneList[operation].lock()->GetAngle());
						Math::Vector3 angle = _boneList[operation].lock()->GetAngle();
						ImGui::SliderFloat("Angle", &angle.y, 0, 360);
						// 大きさ
						ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _boneList[operation].lock()->GetSize());
						float size = _boneList[operation].lock()->GetSize();
						ImGui::SliderFloat("Size", &size, 1, 100);
						// 攻撃範囲
						ImGui::Text((const char*)u8"　攻撃範囲 ATKRange=%.2f", _boneList[operation].lock()->GetAtkRange());
						float range = _boneList[operation].lock()->GetAtkRange();
						ImGui::SliderFloat("ATKRange", &range, 1, 100);
						// 前方方向
						ImGui::Text((const char*)u8"　前方方向 x=%.2f,y=%.2f,z=%.2f", _boneList[operation].lock()->GetForward().x, _boneList[operation].lock()->GetForward().y, _boneList[operation].lock()->GetForward().z);

						_boneList[operation].lock()->SetParam(hp, atk, speed, stamina);
						_boneList[operation].lock()->SetPos(pos);
						_boneList[operation].lock()->SetAngle(angle);
						_boneList[operation].lock()->SetSize(size);
						_boneList[operation].lock()->SetAtkRange(range);

						if (ImGui::Button((const char*)u8"消滅"))
						{
							_boneList[operation].lock()->Expired();
							operation = -1;
						}

						ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");
					}
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Golem"))
			{
				if (ImGui::Button((const char*)u8"Golem追加"))
				{
					AddGolem();
				}

				static int operation = -1;
				if (!SceneManager::Instance().m_stop)operation = -1;

				if (ImGui::TreeNode("Golem"))
				{
					ImGui::Text((const char*)u8"Golem:%d体", _golemList.size());
					for (int golem = 0; golem < _golemList.size(); ++golem)
					{
						if (ImGui::Button(std::to_string(golem + 1).c_str()))
						{
							operation = golem;
						}
					}

					if (operation != -1)
					{
						ImGui::Text((const char*)u8"%d体目", operation + 1);
						// 体力
						ImGui::Text((const char*)u8"　体力 　　HP=%d", _golemList[operation].lock()->GetParam().Hp);
						int hp = _golemList[operation].lock()->GetParam().Hp;
						ImGui::SliderInt("HP", &hp, 1, 100);
						// 攻撃力
						ImGui::Text((const char*)u8"　攻撃力 　ATK=%d", _golemList[operation].lock()->GetParam().Atk);
						int atk = _golemList[operation].lock()->GetParam().Atk;
						ImGui::SliderInt("ATK", &atk, 1, 100);
						// 素早さ
						ImGui::Text((const char*)u8"　素早さ 　SP=%.2f", _golemList[operation].lock()->GetParam().Sp);
						float speed = _golemList[operation].lock()->GetParam().Sp;
						ImGui::SliderFloat("Speed", &speed, 1, 100);
						// スタミナ
						ImGui::Text((const char*)u8"　スタミナ SM=%d", _golemList[operation].lock()->GetParam().Sm);
						int stamina = _golemList[operation].lock()->GetParam().Sm;
						ImGui::SliderInt("Stamina", &stamina, 1, 100);
						// 位置
						ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", _golemList[operation].lock()->GetPos().x, _golemList[operation].lock()->GetPos().y, _golemList[operation].lock()->GetPos().z);
						Math::Vector3 pos = _golemList[operation].lock()->GetPos();
						ImGui::SliderFloat("PosX", &pos.x, -100, 100);
						ImGui::SliderFloat("PosY", &pos.y, 0, 400);
						ImGui::SliderFloat("PosZ", &pos.z, -100, 100);
						// 方向
						ImGui::Text((const char*)u8"　方向 　　x=%.2f,y=%.2f,z=%.2f", _golemList[operation].lock()->GetDir().x, _golemList[operation].lock()->GetDir().y, _golemList[operation].lock()->GetDir().z);
						// 角度
						ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", _golemList[operation].lock()->GetAngle());
						Math::Vector3 angle = _golemList[operation].lock()->GetAngle();
						ImGui::SliderFloat("Angle", &angle.y, 0, 360);
						// 大きさ
						ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _golemList[operation].lock()->GetSize());
						float size = _golemList[operation].lock()->GetSize();
						ImGui::SliderFloat("Size", &size, 0.01, 1.5);
						// 攻撃範囲
						ImGui::Text((const char*)u8"　攻撃範囲 ATKRange=%.2f", _golemList[operation].lock()->GetAtkRange());
						float range = _golemList[operation].lock()->GetAtkRange();
						ImGui::SliderFloat("ATKRange", &range, 1, 100);
						// 前方方向
						ImGui::Text((const char*)u8"　前方方向 x=%.2f,y=%.2f,z=%.2f", _golemList[operation].lock()->GetForward().x, _golemList[operation].lock()->GetForward().y, _golemList[operation].lock()->GetForward().z);

						_golemList[operation].lock()->SetParam(hp, atk, speed, stamina);
						_golemList[operation].lock()->SetPos(pos);
						_golemList[operation].lock()->SetAngle(angle);
						_golemList[operation].lock()->SetSize(size);
						_golemList[operation].lock()->SetAtkRange(range);

						if (ImGui::Button((const char*)u8"消滅"))
						{
							_golemList[operation].lock()->Expired();
							operation = -1;
						}

						ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");
					}
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("UI"))
			{
				if (ImGui::TreeNode("HP"))
				{
					if (ImGui::Button((const char*)u8"HP保存"))
					{
						EnemyHPWrite();
					}

					for (auto& enemy : m_EnemyHPList)
					{
						if (enemy.expired() == false)
						{
							std::shared_ptr<Enemy_HP> _hp = enemy.lock();

							// 位置
							ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f", _hp->GetVector2Pos().x, _hp->GetVector2Pos().y);
							Math::Vector2 _HPPos = _hp->GetVector2Pos();
							ImGui::SliderFloat("PosX", &_HPPos.x, -640, 640);
							ImGui::SliderFloat("PosY", &_HPPos.y, -360, 360);

							// 大きさ
							ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _hp->GetSize());
							float _HPSize = _hp->GetSize();
							ImGui::SliderFloat("Size", &_HPSize, 1, 100);

							_hp->SetPos(_HPPos);
							_hp->SetSize(_HPSize);
						}
					}

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Object"))
		{
			if (ImGui::Button((const char*)u8"Object保存")) ObjectWrite(("Asset/Json/") + _filePath + ("/Object/Object.json"));
			static int Goperation = -1;
			static int Coperation = -1;
			static int Woperation = -1;

			std::vector<std::weak_ptr<KdGameObject>> groundList;
			std::vector<std::weak_ptr<KdGameObject>> circleList;
			std::vector<std::weak_ptr<KdGameObject>> magicList;
			std::vector<std::weak_ptr<KdGameObject>> wallList;
			std::vector<std::weak_ptr<KdGameObject>> skyboxList;
			for (auto& obj : m_ObjectList)
			{
				if (obj.expired() == false)
				{
					if (obj.lock()->GetName() == "Ground")
					{
						groundList.push_back(obj);
					}
					else if (obj.lock()->GetName() == "Circle")
					{
						circleList.push_back(obj);
					}
					else if (obj.lock()->GetName() == "Magic")
					{
						magicList.push_back(obj);
					}
					else if (obj.lock()->GetName() == "Wall")
					{
						wallList.push_back(obj);
					}
					else if (obj.lock()->GetName() == "SkyBox")
					{
						skyboxList.push_back(obj);
					}
				}
			}

			if (ImGui::TreeNode("Ground"))
			{
				if (ImGui::Button((const char*)u8"Ground追加")) AddGround();
				for (int g = 0; g < groundList.size(); ++g)
				{
					if (ImGui::Button(std::to_string(g + 1).c_str()))Goperation = g;
				}

				if (Goperation != -1)
				{
					ImGui::Text((const char*)u8"%d個目", Goperation + 1);
					// 位置
					ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", groundList[Goperation].lock()->GetPos().x, groundList[Goperation].lock()->GetPos().y, groundList[Goperation].lock()->GetPos().z);
					Math::Vector3 pos = groundList[Goperation].lock()->GetPos();
					ImGui::SliderFloat("PosX", &pos.x, -100, 100);
					ImGui::SliderFloat("PosY", &pos.y, 0, 500);
					ImGui::SliderFloat("PosZ", &pos.z, -100, 100);
					// 角度
					ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", groundList[Goperation].lock()->GetAngle());
					Math::Vector3 angle = groundList[Goperation].lock()->GetAngle();
					ImGui::SliderFloat("Angle", &angle.y, 0, 360);
					// 大きさ
					ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", groundList[Goperation].lock()->GetSize());
					float size = groundList[Goperation].lock()->GetSize();
					ImGui::SliderFloat("Size", &size, 1, 100);

					groundList[Goperation].lock()->SetPos(pos);
					groundList[Goperation].lock()->SetSize(size);
					groundList[Goperation].lock()->SetAngle(angle);

					if (ImGui::Button((const char*)u8"消滅"))
					{
						groundList[Goperation].lock()->Expired();
						if (circleList.size() == groundList.size())
						{
							circleList[Goperation].lock()->Expired();
							magicList[Goperation].lock()->Expired();
						}
						DeleteObjectList();
						Goperation = -1;
						Coperation = -1;
					}

					ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Circle"))
			{
				if (ImGui::Button((const char*)u8"Circle追加"))
				{
					if (groundList.size() > circleList.size()) AddCircle();
				}

				if (circleList.size() != 0)
				{
					// 角度
					ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", circleList[0].lock()->GetAngle());
					Math::Vector3 angle = circleList[0].lock()->GetAngle();
					ImGui::SliderFloat("Angle", &angle.y, 0, 360);
					// 大きさ
					ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", circleList[0].lock()->GetSize());
					float size = circleList[0].lock()->GetSize();
					ImGui::SliderFloat("Size", &size, 1, 100);

					for (int c = 0; c < circleList.size(); ++c)
					{
						circleList[c].lock()->SetSize(size);
						circleList[c].lock()->SetAngle(angle);

						if (ImGui::Button(std::to_string(c + 1).c_str()))Coperation = c;
					}

					if (Coperation != -1)
					{
						if (circleList[Coperation].expired() == false)
						{
							ImGui::Text((const char*)u8"%d個目", Coperation + 1);
							// 位置
							Math::Vector3 pos;
							ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", circleList[Coperation].lock()->GetPos().x, circleList[Coperation].lock()->GetPos().y, circleList[Coperation].lock()->GetPos().z);
							pos = circleList[Coperation].lock()->GetPos();
							if (groundList[Coperation].expired() == false)pos.y = groundList[Coperation].lock()->GetPos().y;
							ImGui::SliderFloat("PosX", &pos.x, -100, 100);
							ImGui::SliderFloat("PosZ", &pos.z, -100, 100);

							circleList[Coperation].lock()->SetPos(pos);

							if (ImGui::Button((const char*)u8"消滅"))
							{
								circleList[Coperation].lock()->Expired();
								magicList[Coperation].lock()->Expired();
								DeleteObjectList();
								Coperation = -1;
							}
						}
					}


					ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");

					if (ImGui::TreeNode("Magic"))
					{
						float size = 0.0f;
						if (magicList[0].expired() == false)
						{
							// 大きさ
							ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", magicList[0].lock()->GetSize());
							size = magicList[0].lock()->GetSize();
							ImGui::SliderFloat("Size", &size, 1, 100);
						}

						for (int m = 0; m < magicList.size(); ++m)
						{
							if (circleList[m].expired() == false)
							{
								magicList[m].lock()->SetSize(size);
							}
						}
						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Wall"))
			{
				if (ImGui::Button((const char*)u8"Wall追加")) AddWall();
				for (int w = 0; w < wallList.size(); ++w)
				{
					if (ImGui::Button(std::to_string(w + 1).c_str()))Woperation = w;
				}

				if (Woperation != -1)
				{
					ImGui::Text((const char*)u8"%d個目", Woperation + 1);
					// 位置
					ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", wallList[Woperation].lock()->GetPos().x, wallList[Woperation].lock()->GetPos().y, wallList[Woperation].lock()->GetPos().z);
					Math::Vector3 pos = wallList[Woperation].lock()->GetPos();
					ImGui::SliderFloat("PosX", &pos.x, -100, 100);
					ImGui::SliderFloat("PosY", &pos.y, 0, 500);
					ImGui::SliderFloat("PosZ", &pos.z, -100, 100);
					// 角度
					ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", wallList[Woperation].lock()->GetAngle());
					Math::Vector3 angle = wallList[Woperation].lock()->GetAngle();
					ImGui::SliderFloat("Angle", &angle.y, 0, 360);
					// 大きさ
					ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", wallList[Woperation].lock()->GetSize());
					float size = wallList[Woperation].lock()->GetSize();
					ImGui::SliderFloat("Size", &size, 1, 100);

					wallList[Woperation].lock()->SetPos(pos);
					wallList[Woperation].lock()->SetSize(size);
					wallList[Woperation].lock()->SetAngle(angle);

					if (ImGui::Button((const char*)u8"消滅"))
					{
						wallList[Woperation].lock()->Expired();
						DeleteObjectList();
						Woperation = -1;
					}

					ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("SkyBox"))
			{
				if (ImGui::Button((const char*)u8"SkyBox追加")) AddSkyBox();

				if (skyboxList.size() > 0)
				{
					// 位置
					ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", skyboxList[0].lock()->GetPos().x, skyboxList[0].lock()->GetPos().y, skyboxList[0].lock()->GetPos().z);
					Math::Vector3 pos = skyboxList[0].lock()->GetPos();
					ImGui::SliderFloat("PosX", &pos.x, -500, 500);
					ImGui::SliderFloat("PosY", &pos.y, 0, 500);
					ImGui::SliderFloat("PosZ", &pos.z, -500, 500);
					// 角度
					ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", skyboxList[0].lock()->GetAngle());
					Math::Vector3 angle = skyboxList[0].lock()->GetAngle();
					ImGui::SliderFloat("Angle", &angle.y, 0, 360);
					// 大きさ
					ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", skyboxList[0].lock()->GetSize());
					float size = skyboxList[0].lock()->GetSize();
					ImGui::SliderFloat("Size", &size, 1, 100);

					skyboxList[0].lock()->SetPos(pos);
					skyboxList[0].lock()->SetSize(size);
					skyboxList[0].lock()->SetAngle(angle);

					ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");
				}
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode((const char*)u8"シーン切替"))
		{
			if (ImGui::Button("Title"))
			{
				SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
				KdEffekseerManager::GetInstance().StopAllEffect();
			}
			if (ImGui::Button("Game"))
			{
				SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
				KdEffekseerManager::GetInstance().StopAllEffect();
			}

			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
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

	std::ofstream _file("Asset/Json/Title/Camera/TitleCamera.json");
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

	std::ofstream _file("Asset/Json/Title/Title/Title.json");
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

	std::ofstream _file("Asset/Json/Title/Game/Game.json");
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

	std::ofstream _file("Asset/Json/Title/Exit/Exit.json");
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

	std::ofstream _file("Asset/Json/Title/Guide/Guide.json");
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

	std::ofstream _file("Asset/Json/Title/Cursor/Cursor.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::GameCameraWrite()
{
	nlohmann::json _json;
	std::ifstream _oldFile("Asset/Json/Game/Camera/Camera.json");
	if (_oldFile.is_open())
	{
		_oldFile >> _json;
		_oldFile.close();
	}

	std::string _cameraType;
	switch (m_camera.lock()->GetCameraType())
	{
	case GameCamera::CameraType::PlayerType:
		_cameraType = "Player";
		break;
	case GameCamera::CameraType::FixedType:
		_cameraType = "Fixed";
		break;
	case GameCamera::CameraType::ClearType:
		_cameraType = "Clear";
		break;
	default:
		break;
	}

	_json["Camera"]["Name"] = "Camera";
	_json["Camera"][_cameraType + "PosX"] = m_camera.lock()->GetNowPos().x;
	_json["Camera"][_cameraType + "PosY"] = m_camera.lock()->GetNowPos().y;
	_json["Camera"][_cameraType + "PosZ"] = m_camera.lock()->GetNowPos().z;
	_json["Camera"][_cameraType + "AngleX"] = m_camera.lock()->GetNowDegAng().x;
	_json["Camera"][_cameraType + "AngleY"] = m_camera.lock()->GetNowDegAng().y;
	_json["Camera"][_cameraType + "ViewAngle"] = m_camera.lock()->GetNowViewAng();

	std::ofstream _newfile("Asset/Json/Game/Camera/Camera.json");
	if (_newfile.is_open())
	{
		_newfile << _json.dump();
		_newfile.close();
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
	_json["Player"]["Angle"] = m_player.lock()->GetAngle().y;
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
	_json["Player"]["ShieldName"] = m_player.lock()->GetShield().lock()->GetName();


	std::ofstream _file(_fileName);
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::PlayerHPWrite()
{
	if (m_PlayerHP.expired())return;

	nlohmann::json _json;

	_json["HP"]["Name"] = "HP";
	_json["HP"]["PosX"] = m_PlayerHP.lock()->GetVector2Pos().x;
	_json["HP"]["PosY"] = m_PlayerHP.lock()->GetVector2Pos().y;
	_json["HP"]["Size"] = m_PlayerHP.lock()->GetSize();

	std::ofstream _file("Asset/Json/Game/UI/Player/HP/HP.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::PlayerStaminaWrite()
{
	if (m_PlayerStamina.expired())return;

	nlohmann::json _json;

	_json["Stamina"]["Name"] = "Stamina";
	_json["Stamina"]["PosX"] = m_PlayerStamina.lock()->GetVector2Pos().x;
	_json["Stamina"]["PosY"] = m_PlayerStamina.lock()->GetVector2Pos().y;
	_json["Stamina"]["Size"] = m_PlayerStamina.lock()->GetSize();

	std::ofstream _file("Asset/Json/Game/UI/Player/Stamina/Stamina.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::LockONWrite()
{
	if (m_lockON.expired())return;

	nlohmann::json _json;

	_json["LockON"]["Name"] = "LockON";
	_json["LockON"]["PosX"] = m_lockON.lock()->GetVector2Pos().x;
	_json["LockON"]["PosY"] = m_lockON.lock()->GetVector2Pos().y;
	_json["LockON"]["Size"] = m_lockON.lock()->GetSize();
	_json["LockON"]["SizeChange"] = m_lockON.lock()->GetChangeSize();
	_json["LockON"]["AlphaChange"] = m_lockON.lock()->GetChangeAlpha();

	std::ofstream _file("Asset/Json/Game/UI/Player/LockON/LockON.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::FloorWrite()
{
	if (m_floor.expired())return;

	nlohmann::json _json;

	_json["Floor"]["Name"] = "Floor";
	_json["Floor"]["PosX"] = m_floor.lock()->GetVector2Pos().x;
	_json["Floor"]["PosY"] = m_floor.lock()->GetVector2Pos().y;
	_json["Floor"]["Size"] = m_floor.lock()->GetSize();

	std::ofstream _file("Asset/Json/Game/UI/Player/Floor/Floor.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::TeleportWrite()
{
	if (m_teleport.expired())return;

	nlohmann::json _json;

	_json["Teleport"]["Name"] = "Teleport";
	_json["Teleport"]["PosX"] = m_teleport.lock()->GetVector2Pos().x;
	_json["Teleport"]["PosY"] = m_teleport.lock()->GetVector2Pos().y;
	_json["Teleport"]["Size"] = m_teleport.lock()->GetSize();
	_json["Teleport"]["AlphaChange"] = m_teleport.lock()->GetChangeAlpha();

	std::ofstream _file("Asset/Json/Game/UI/Player/Teleport/Teleport.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::GameOverWrite()
{
	if (m_gameOver.expired())return;

	nlohmann::json _json;

	_json["GameOver"]["Name"] = "gameOver";
	_json["GameOver"]["PosX"] = m_gameOver.lock()->GetVector2Pos().x;
	_json["GameOver"]["PosY"] = m_gameOver.lock()->GetVector2Pos().y;
	_json["GameOver"]["Size"] = m_gameOver.lock()->GetSize();
	_json["GameOver"]["AlphaChange"] = m_gameOver.lock()->GetAlphaChange();

	std::ofstream _file("Asset/Json/Game/UI/Player/GameOver/GameOver.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::EnemyWrite(int _stage, int _wave, std::string _fileName)
{
	std::string _stagePath = (_fileName)+std::to_string(_stage).c_str() + ((std::string)".json");
	nlohmann::json _json;
	std::ifstream _oldFile(_stagePath);
	if (_oldFile.is_open())
	{
		_oldFile >> _json;
		_oldFile.close();
	}

	std::string wave = (std::to_string(_wave).c_str()) + ((std::string)"Wave");
	_json[wave].clear();

	for (auto& enemy : m_EnemyList)
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
		_json[wave][_category][_name]["Angle"] = enemy.lock()->GetAngle().y;
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

void ObjectManager::EnemyHPWrite()
{
	if (m_EnemyHPList[0].expired())return;

	nlohmann::json _json;

	_json["HP"]["Name"] = "HP";
	_json["HP"]["PosX"] = m_EnemyHPList[0].lock()->GetVector2Pos().x;
	_json["HP"]["PosY"] = m_EnemyHPList[0].lock()->GetVector2Pos().y;
	_json["HP"]["Size"] = m_EnemyHPList[0].lock()->GetSize();

	std::ofstream _file("Asset/Json/Game/UI/Enemy/HP/HP.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
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

	for (auto& _sword : _json)
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

	for (auto& obj : m_ObjectList)
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
		_json[_category][_name]["Angle"] = obj.lock()->GetAngle().y;
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
	std::string fileName = "Asset/Json/Title/Camera/TitleCamera.json";

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
		camera->Init();
		m_id++;

		m_titleCamera = camera;
		SceneManager::Instance().SetCamera(camera);
		KdEffekseerManager::GetInstance().SetCamera(m_titleCamera.lock()->GetCamera());
	}

	ifs.close();
}

void ObjectManager::SetTitleParam()
{
	//jsonファイル
	std::string fileName = "Asset/Json/Title/Title/Title.json";

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
		SceneManager::Instance().AddUI(title);
	}

	ifs.close();
}

void ObjectManager::SetGameParam()
{
	//jsonファイル
	std::string fileName = "Asset/Json/Title/Game/Game.json";

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
		SceneManager::Instance().AddUI(game);
	}

	ifs.close();
}

void ObjectManager::SetExitParam()
{
	//jsonファイル
	std::string fileName = "Asset/Json/Title/Exit/Exit.json";

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
		SceneManager::Instance().AddUI(exit);
	}

	ifs.close();
}

void ObjectManager::SetTitleGuideParam()
{
	//jsonファイル
	std::string fileName = "Asset/Json/Title/Guide/Guide.json";

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
		SceneManager::Instance().AddUI(guide);
	}

	ifs.close();
}

void ObjectManager::SetCursorParam()
{
	//jsonファイル
	std::string fileName = "Asset/Json/Title/Cursor/Cursor.json";

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
		cursor->SetGame(m_game.lock());
		cursor->SetExit(m_exit.lock());
		cursor->SetID(m_id);
		cursor->Init();
		m_id++;

		m_cursor = cursor;
		SceneManager::Instance().AddUI(cursor);
	}

	ifs.close();
}

void ObjectManager::SetGameCameraParam()
{
	//jsonファイル
	std::string fileName = "Asset/Json/Game/Camera/Camera.json";

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

		Math::Vector3 _ClearPos = Math::Vector3::Zero;
		_ClearPos.x = stage["ClearPosX"];
		_ClearPos.y = stage["ClearPosY"];
		_ClearPos.z = stage["ClearPosZ"];

		Math::Vector3 _PlayerAngle = Math::Vector3::Zero;
		_PlayerAngle.x = stage["PlayerAngleX"];
		_PlayerAngle.y = stage["PlayerAngleY"];

		Math::Vector3 _FixedAngle = Math::Vector3::Zero;
		_FixedAngle.x = stage["FixedAngleX"];
		_FixedAngle.y = stage["FixedAngleY"];

		Math::Vector3 _ClearAngle = Math::Vector3::Zero;
		_ClearAngle.x = stage["ClearAngleX"];
		_ClearAngle.y = stage["ClearAngleY"];

		float _PlayerViewAngle = 0.0f;
		_PlayerViewAngle = stage["PlayerViewAngle"];

		float _FixedViewAngle = 0.0f;
		_FixedViewAngle = stage["FixedViewAngle"];

		float _ClearViewAngle = 0.0f;
		_ClearViewAngle = stage["ClearViewAngle"];

		std::string _name;
		_name = stage["Name"];
		std::shared_ptr<GameCamera> camera = std::make_shared<GameCamera>();

		camera->SetPosList(_PlayerPos, _FixedPos, _ClearPos);
		camera->SetDegAngList(_PlayerAngle, _FixedAngle, _ClearAngle);
		camera->SetViewAngList(_PlayerViewAngle, _FixedViewAngle, _ClearViewAngle);
		camera->SetName(_name);
		camera->SetObjManager(shared_from_this());
		camera->Init();
		camera->SetID(m_id);
		m_id++;

		m_camera = camera;
		SceneManager::Instance().SetCamera(camera);
	}

	ifs.close();
}

void ObjectManager::SetObjectParam(std::shared_ptr<StageManager> _stage)
{
	//jsonファイル
	std::string fileName = ("Asset/Json/") + m_nowScene + ("/Object/Object.json");

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	std::shared_ptr<Ground> ground;
	std::shared_ptr<Circle> circle;
	std::shared_ptr<MagicPolygon> magic;

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

			Math::Vector3 _angle = Math::Vector3::Zero;
			_angle.y = stage["Angle"];

			std::string _name;
			_name = stage["Name"];
			std::shared_ptr<KdGameObject> obj;
			if (_name == "Ground")
			{
				ground = std::make_shared<Ground>();
				m_ground = ground;
				obj = ground;
			}
			if (_name == "Circle")
			{
				circle = std::make_shared<Circle>();
				m_circle = circle;
				obj = circle;
			}
			if (_name == "Magic")
			{
				magic = std::make_shared<MagicPolygon>();
				m_magic = magic;
				if (m_camera.expired() == false)m_camera.lock()->SetFixedTarget(magic);
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
			obj->SetAngle(_angle);
			obj->SetName(_name);
			obj->SetID(m_id);
			obj->Init();
			m_id++;

			m_ObjectList.push_back(obj);
			SceneManager::Instance().AddObject(obj);
			break;
		}
	}

	int _maxStage = 0;
	for (auto& stage : _json["Ground"])
	{
		_maxStage++;
	}

	if (m_nowScene == "Game")_stage->SetMaxStage(_maxStage);

	ifs.close();

	if (circle && ground)circle->SetGround(ground);
	if (magic && circle)magic->SetCircle(circle);
}

void ObjectManager::SetPlayerParam(std::shared_ptr<StageManager> _stage)
{
	//jsonファイル
	std::string fileName = ("Asset/Json/") + m_nowScene + ("/Player/Player.json");

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
		_pos = m_circle.lock()->GetPos();

		Math::Vector3 _dir = Math::Vector3::Zero;
		_dir.x = stage["DirX"];
		_dir.y = stage["DirY"];
		_dir.z = stage["DirZ"];

		float _size = 0.0f;
		_size = stage["Size"];

		Math::Vector3 _angle = Math::Vector3::Zero;
		_angle.y = stage["Angle"];

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

		SetWeaponParam((("Asset/Json/") + m_nowScene + ("/Weapon/Sword/Sword.json")), stage["SwordName"]);
		SetWeaponParam((("Asset/Json/") + m_nowScene + ("/Weapon/Shield/Shield.json")), stage["ShieldName"]);

		player->SetCamera(m_camera.lock());
		player->SetParam(_hp, player->GetSword().lock()->GetATK(), _speed, _stamina);
		player->SetPos(_pos);
		player->SetSize(_size);
		player->SetDir(_dir);
		player->SetAngle(_angle);
		player->SetAtkRange(_atkRange);
		player->SetForward(_forward);
		player->SetObjManager(shared_from_this());
		player->SetStageManager(_stage);
		player->Init();
		player->SetInviTime(_inviTime);
		player->SetName(_name);
		player->SetID(m_id);
		m_id++;

		SceneManager::Instance().SetPlayer(player);

		m_camera.lock()->SetTarget(player);

		KdEffekseerManager::GetInstance().SetCamera(m_camera.lock()->GetCamera());
	}

	ifs.close();

	SetPlayerUI(_stage);
}

void ObjectManager::SetPlayerUI(std::shared_ptr<StageManager> _stage)
{
	SetPlayerHPParam();
	SetPlayerStaminaParam();
	SetLockONParam();
	SetFloorParam(_stage);
	SetTeleportParam();
}

void ObjectManager::SetPlayerHPParam()
{
	//jsonファイル
	std::string fileName = "Asset/Json/Game/UI/Player/HP/HP.json";

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
		std::shared_ptr<Player_HP> hp = std::make_shared<Player_HP>();

		hp->SetPos(_pos);
		hp->SetSize(_size);
		hp->SetName(_name);
		hp->SetTraget(m_player.lock());
		hp->SetID(m_id);
		hp->Init();
		m_id++;

		m_PlayerHP = hp;
		SceneManager::Instance().AddPlayerUI(hp);
	}

	ifs.close();
}

void ObjectManager::SetPlayerStaminaParam()
{
	//jsonファイル
	std::string fileName = "Asset/Json/Game/UI/Player/Stamina/Stamina.json";

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
		std::shared_ptr<Player_Stamina> stamina = std::make_shared<Player_Stamina>();

		stamina->SetPos(_pos);
		stamina->SetSize(_size);
		stamina->SetName(_name);
		stamina->SetTraget(m_player.lock());
		stamina->SetID(m_id);
		stamina->Init();
		m_id++;

		m_PlayerStamina = stamina;
		SceneManager::Instance().AddPlayerUI(stamina);
	}

	ifs.close();
}

void ObjectManager::SetLockONParam()
{
	//jsonファイル
	std::string fileName = "Asset/Json/Game/UI/Player/LockON/LockON.json";

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

		float _sizeChange = 0.0f;
		_sizeChange = stage["SizeChange"];

		float _alphaChange = 0.0f;
		_alphaChange = stage["AlphaChange"];

		std::string _name;
		_name = stage["Name"];
		std::shared_ptr<LockON> lockON = std::make_shared<LockON>();

		lockON->SetPos(_pos);
		lockON->SetSize(_size);
		lockON->SetChangeSize(_sizeChange);
		lockON->SetChangeAlpha(_alphaChange);
		lockON->SetName(_name);
		lockON->SetTraget(m_player.lock());
		lockON->SetID(m_id);
		lockON->Init();
		m_id++;

		m_lockON = lockON;
		SceneManager::Instance().AddPlayerUI(lockON);
	}

	ifs.close();
}

void ObjectManager::SetFloorParam(std::shared_ptr<StageManager> _stage)
{
	//jsonファイル
	std::string fileName = "Asset/Json/Game/UI/Player/Floor/Floor.json";

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
		std::shared_ptr<Floor> floor = std::make_shared<Floor>();

		floor->SetSize(_size);
		floor->SetPos(_pos);
		floor->SetName(_name);
		floor->SetStageManager(_stage);
		floor->SetID(m_id);
		floor->Init();
		m_id++;

		m_floor = floor;
		SceneManager::Instance().AddPlayerUI(floor);
	}

	ifs.close();
}

void ObjectManager::SetTeleportParam()
{
	//jsonファイル
	std::string fileName = "Asset/Json/Game/UI/Player/Teleport/Teleport.json";

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

		float _alphaChange = 0.0f;
		_alphaChange = stage["AlphaChange"];

		std::string _name;
		_name = stage["Name"];
		std::shared_ptr<Teleport> teleport = std::make_shared<Teleport>();

		teleport->SetPos(_pos);
		teleport->SetSize(_size);
		teleport->SetChangeAlpha(_alphaChange);
		teleport->SetName(_name);
		teleport->SetPlayer(m_player.lock());
		teleport->SetID(m_id);
		teleport->Init();
		m_id++;

		m_teleport = teleport;
		SceneManager::Instance().AddPlayerUI(teleport);
	}

	ifs.close();
}

void ObjectManager::SetGameOverParam()
{
	if (!m_gameOver.expired())return;

	//jsonファイル
	std::string fileName = "Asset/Json/Game/UI/Player/GameOver/GameOver.json";

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

		float _alphaChange = 0.0f;
		_alphaChange = stage["AlphaChange"];

		std::string _name;
		_name = stage["Name"];
		std::shared_ptr<GameOver> over = std::make_shared<GameOver>();

		over->SetPos(_pos);
		over->SetSize(_size);
		over->SetChangeAlpha(_alphaChange);
		over->SetName(_name);
		over->SetID(m_id);
		over->Init();
		m_id++;

		m_gameOver = over;
		SceneManager::Instance().AddPlayerUI(over);
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

		Math::Vector3 _angle = Math::Vector3::Zero;
		_angle.y = stage["Angle"];

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
		weapon->SetAngle(_angle);
		weapon->SetTarget(m_player.lock());
		weapon->SetName(_name);
		weapon->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddWeapon(weapon);

		//デバッグ
		//break;
	}

	ifs.close();
}

void ObjectManager::SetEnemyParam(std::string _filePath, std::shared_ptr<StageManager> _stage)
{
	//jsonファイル
	std::string fileName = _filePath;

	std::ifstream ifs(fileName.c_str());
	if (ifs.is_open())
	{
		ifs >> m_EnemyJson;
		ifs.close();
	}

	int _wave = 0;
	int _nowWave = 0;

	for (auto& wave : m_EnemyJson)
	{
		if (_stage->GetnowWave() != 0 && _nowWave != _stage->GetnowWave())
		{
			_nowWave++;
			continue;
		}

		if (SceneManager::Instance().GetEnemyList().size() == 0)
		{
			for (auto& category : wave)
			{
				for (auto& stage : category)
				{
					std::string _name;
					_name = stage["Name"];

					Math::Vector3 _pos = Math::Vector3::Zero;
					_pos.x = stage["PosX"];
					_pos.y = m_ground.lock()->GetPos().y;
					_pos.z = stage["PosZ"];

					Math::Vector3 _dir = Math::Vector3::Zero;
					_dir.x = stage["DirX"];
					_dir.y = stage["DirY"];
					_dir.z = stage["DirZ"];

					float _size = 0.0f;
					_size = stage["Size"];

					Math::Vector3 _angle = Math::Vector3::Zero;
					_angle.y = stage["Angle"];

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
					enemy->SetCamera(m_camera.lock());
					enemy->SetParam(_hp, _atk, _speed, _stamina);
					enemy->SetPos(_pos);
					enemy->SetSize(_size);
					enemy->SetDir(_dir);
					enemy->SetAngle(_angle);
					enemy->SetAtkRange(_atkRange);
					enemy->SetForward(_forward);
					enemy->SetTarget(m_player.lock());
					enemy->SetName(_name);
					enemy->SetID(m_id);
					enemy->SetObjManager(shared_from_this());
					enemy->Init();
					m_id++;

					SceneManager::Instance().AddEnemy(enemy);
					m_EnemyList.push_back(enemy);

					SetEnemyHPParam(enemy);
				}
			}
		}
		_wave++;
	}

	if (_stage->GetMaxWave() == 0)_stage->SetMaxWave(_wave);
}

void ObjectManager::SetEnemyHPParam(std::shared_ptr<EnemyBase> _enemy)
{
	//jsonファイル
	std::string fileName = "Asset/Json/Game/UI/Enemy/HP/HP.json";

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
		std::shared_ptr<Enemy_HP> hp = std::make_shared<Enemy_HP>();

		hp->SetPos(_pos);
		hp->SetSize(_size);
		hp->SetName(_name);
		hp->SetCamera(m_camera.lock());
		hp->SetTarget(_enemy);
		hp->SetID(m_id);
		hp->Init();
		m_id++;

		m_EnemyHPList.push_back(hp);
		SceneManager::Instance().AddEnemyUI(hp);
	}

	ifs.close();
}

std::shared_ptr<Bullet> ObjectManager::SetBulletParam(std::shared_ptr<Golem> _golem)
{
	std::shared_ptr<Bullet> _bullet = std::make_shared<Bullet>();
	_bullet->Init();
	_bullet->SetPos(_golem->GetBulletPoint().Translation());
	_bullet->SetGolem(_golem);
	_bullet->SetID(m_id);
	m_id++;
	SceneManager::Instance().AddObject(_bullet);

	return _bullet;
}

const bool& ObjectManager::GetTeleportFlg()
{
	return m_magic.lock()->GetTeleport();
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
	Math::Vector3 _angle = Math::Vector3::Zero;
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
	enemy->SetAngle(_angle);
	enemy->SetAtkRange(_atkRange);
	enemy->SetForward(_forward);
	enemy->SetName(_name);
	enemy->SetCamera(m_camera.lock());
	enemy->SetTarget(m_player.lock());
	enemy->SetID(m_id);
	enemy->SetObjManager(shared_from_this());
	enemy->Init();
	m_id++;

	SceneManager::Instance().AddEnemy(enemy);
	m_EnemyList.push_back(enemy);

	SetEnemyHPParam(enemy);
}

void ObjectManager::AddGolem()
{
	std::string _name = "Golem";
	Math::Vector3 _pos = Math::Vector3::Zero;
	Math::Vector3 _dir = Math::Vector3::Zero;
	float _size = 1.5f;
	Math::Vector3 _angle = Math::Vector3::Zero;
	_angle.y = 180.0f;
	int _hp = 10;
	int _atk = 2;
	float _speed = 1.0f;
	int _stamina = 50;
	float _atkRange = 3.0f;
	Math::Vector3 _forward = Math::Vector3::Zero;
	_forward.z = -1.0f;
	float _chaseRange = 1000.0f;

	std::shared_ptr<Golem> enemy = nullptr;
	enemy = std::make_shared<Golem>();
	enemy->SetParam(_hp, _atk, _speed, _stamina);
	enemy->SetPos(_pos);
	enemy->SetSize(_size);
	enemy->SetDir(_dir);
	enemy->SetAngle(_angle);
	enemy->SetAtkRange(_atkRange);
	enemy->SetForward(_forward);
	enemy->SetName(_name);
	enemy->SetCamera(m_camera.lock());
	enemy->SetTarget(m_player.lock());
	enemy->SetID(m_id);
	enemy->SetObjManager(shared_from_this());
	enemy->Init();
	m_id++;

	SceneManager::Instance().AddEnemy(enemy);
	m_EnemyList.push_back(enemy);

	SetEnemyHPParam(enemy);
}

void ObjectManager::AddWeapon(std::string _filePath, std::string _weaponName)
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

		Math::Vector3 _angle = Math::Vector3::Zero;
		_angle.y = stage["Angle"];

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
		weapon->SetAngle(_angle);
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
	Math::Vector3 _angle = Math::Vector3::Zero;
	std::shared_ptr<Ground> obj = std::make_shared<Ground>();

	obj->SetPos(_pos);
	obj->SetSize(_size);
	obj->SetAngle(_angle);
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
	Math::Vector3 _angle = Math::Vector3::Zero;
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
			_angle.y = 0.0f;
			obj = circle;
			break;
		case 1:
			magic = std::make_shared<MagicPolygon>();
			_name = "Magic";
			if (circle)_pos = circle->GetMagicPolygonPoint();
			else { _pos = Math::Vector3::Zero; }
			_size = 5.0f;
			_angle.y = 0.0f;
			obj = magic;
			break;
		default:
			break;
		}

		obj->SetPos(_pos);
		obj->SetSize(_size);
		obj->SetAngle(_angle);
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
	Math::Vector3 _angle = Math::Vector3::Zero;
	std::shared_ptr<Wall> obj = std::make_shared<Wall>();

	obj->SetPos(_pos);
	obj->SetSize(_size);
	obj->SetAngle(_angle);
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
	Math::Vector3 _angle = Math::Vector3::Zero;
	std::shared_ptr<SkyBox> obj = std::make_shared<SkyBox>();

	obj->SetSize(_size);
	obj->SetAngle(_angle);
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

		AddWeapon("Asset/Json/Weapon/Sword/Sword.json", _swordName);
	}

	if (m_player.lock()->GetShield().lock()->GetName() != _shieldName)
	{
		m_player.lock()->GetShield().lock()->Expired();

		AddWeapon("Asset/Json/Weapon/Shield/Shield", _shieldName);
	}
}
