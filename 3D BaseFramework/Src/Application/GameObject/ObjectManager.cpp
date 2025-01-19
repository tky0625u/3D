#include "ObjectManager.h"
#include"../Scene/SceneManager.h"
#include"StageManager.h"
#include"Character/CharacterBase.h"
#include"Character/Enemy/Bone/Bone.h"
#include"../tinygltf/json.hpp"
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
#include"Stage/SkyBox/SkyBox.h"
//剣
#include"Weapon/Sword/Sword.h"
//盾
#include"Weapon/Shield/Shield.h"
//プレイヤー
#include"Character/Player/Player.h"
//ゲームカメラ
#include"Camera/GameCamera/GameCamera.h"
//骨
#include"Character/Enemy/Bone/Bone.h"
//骨色違い
#include"Character/Enemy/BoneAlpha/BoneAlpha.h"
//ゴーレム
#include"Character/Enemy/Golem/Golem.h"
//タイトル
#include"UI/Title/Title/Title.h"
//ゲーム文字
#include"UI/Title/Game/Game.h"
//終了文字
#include"UI/Title/Exit/Exit.h"
//カーソル
#include"UI/Title/Cursor/Cursor.h"
//セレクト
#include"UI/Title/Select/Select.h"
//タイトルカメラ
#include"Camera/TitleCamera/TitleCamera.h"
//プレイヤーHP
#include"UI/Player/HP/Player_HP.h"
//プレイヤースタミナ
#include"UI/Player/Stamina/Player_Stamina.h"
//階数
#include"UI/Player/Floor/Floor.h"
//テレポート
#include"UI/Player/Teleport/Teleport.h"
//敵HP
#include"UI/Enemy/HP/Enemy_HP.h"
//ゲーム状態UI
#include"UI/Player/GameStateUI/GameStateUI.h"
//弾
#include"Character/Enemy/Golem/Bullet/Bullet.h"
//骨色違い弾
#include"Character/Enemy/BoneAlpha/Bullet/BoneAlpha_Bullet.h"

void ObjectManager::SceneCheck()
{
	switch (SceneManager::Instance().GetNowSceneType())
	{
	case SceneManager::SceneType::Title: // タイトルシーン
		m_nowScene = "Title";
		break;
	case SceneManager::SceneType::Game: // ゲームシーン
		m_nowScene = "Game";
		break;
	default:
		break;
	}
}

void ObjectManager::DeleteEnemyList()
{
	// HPバー
	// 通常
	auto nHp = m_EnemyHPList[EnemyHPType::Normal].begin();

	while (nHp != m_EnemyHPList[EnemyHPType::Normal].end())
	{
		if (nHp->expired())
		{
			nHp = m_EnemyHPList[EnemyHPType::Normal].erase(nHp);
		}
		else
		{
			++nHp;
		}
	}

	// ボス
	auto bHp = m_EnemyHPList[EnemyHPType::Boss].begin();

	while (bHp != m_EnemyHPList[EnemyHPType::Boss].end())
	{
		if (bHp->expired())
		{
			bHp = m_EnemyHPList[EnemyHPType::Boss].erase(bHp);
		}
		else
		{
			++bHp;
		}
	}

	// 骨色違い
	auto alpha = m_BoneAlphaList.begin();

	while (alpha != m_BoneAlphaList.end())
	{
		if (alpha->expired())
		{
			alpha = m_BoneAlphaList.erase(alpha);
		}
		else
		{
			++alpha;
		}
	}
}

void ObjectManager::SlowChange()
{
	// 呼び出されたらそのときのフラグの反対になる
	if (m_slowFlg)
	{
		m_slowFlg = false;
		m_slow = 1.0f;
	}
	else
	{
		m_slowFlg = true;
		m_slow = 0.25f;
	}
}

void ObjectManager::NextStageLiberation()
{
	// テレポート開放演出
	m_magic.lock()->NextChange();

	// テレポート位置
	m_camera.lock()->FixedChange();
	
	// 待機
	m_player.lock()->IdolChange();
	
	// スロー切り替え
	if (m_slowFlg)SlowChange();
}

void ObjectManager::GameClear()
{
	// クリア位置
	m_camera.lock()->ClearChange();
	
	// 待機
	m_player.lock()->IdolChange();
	
	// スロー切り替え
	if (m_slowFlg)SlowChange();
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

	// ステージ数
	int _stageNum = 1;
	
	for (auto& stage : _json["Ground"])
	{
		// 現在のステージになるまで
		if (_stage->GetnowStage() != _stageNum)
		{
			_stageNum++;
			continue;
		}

		// 座標
		Math::Vector3 _pos = Math::Vector3::Zero;
		_pos.x = stage["PosX"];
		_pos.y = stage["PosY"];
		_pos.z = stage["PosZ"];

		// 大きさ
		float _size = 0.0f;
		_size = stage["Size"];

		// 角度
		Math::Vector3 _angle = Math::Vector3::Zero;
		_angle.y = stage["Angle"];

		// 名前
		std::string _name;
		_name = stage["Name"];

		// 読み込んだ値をセット
		if (!m_ground.expired())
		{
			m_ground.lock()->SetPos(_pos);
			m_ground.lock()->SetSize(_size);
			m_ground.lock()->SetAngle(_angle);
		}

		break;
	}

	Math::Matrix _Scale; // 拡縮
	Math::Matrix _Rot;   // 回転
	Math::Matrix _Trans; // 座標

	// 地面
	_Scale = Math::Matrix::CreateScale(m_ground.lock()->GetSize());
	_Rot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_ground.lock()->GetAngle().y));
	_Trans = Math::Matrix::CreateTranslation(m_ground.lock()->GetPos());
	Math::Matrix _GroundMat = _Scale * _Rot * _Trans;
	m_ground.lock()->SetMatrix(_GroundMat);

	// 魔法陣の台
	_Scale = Math::Matrix::CreateScale(m_circle.lock()->GetSize());
	_Rot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_circle.lock()->GetAngle().y));
	_Trans = Math::Matrix::CreateTranslation(m_circle.lock()->GetPos());
	Math::Matrix _CircleMat = _Scale * _Rot * _Trans * _GroundMat;
	m_circle.lock()->SetMatrix(_CircleMat);

	// 魔法陣
	_Scale = Math::Matrix::CreateScale(m_magic.lock()->GetSize());
	_Rot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_magic.lock()->GetAngle().y));
	_Trans = Math::Matrix::CreateTranslation(m_magic.lock()->GetPos());
	Math::Matrix _MagicMat = _Scale * _Rot * _Trans * _CircleMat;
	m_magic.lock()->SetMatrix(_MagicMat);
	m_magic.lock()->NormalChange();

	ifs.close();

	// 敵生成
	std::string _filePath = ("Asset/Json/Game/Enemy/Stage") + (std::to_string(_stage->GetnowStage())) + (".json");
	SetEnemyParam(_filePath, _stage);

	// プレイヤーを次のステージに移動
	m_player.lock()->SetPos(m_ground.lock()->GetPos());

	// カメラを正面を向くようにする
	Math::Vector3 _angle = Math::Vector3::Zero;
	_angle.x = 10.0f;
	m_camera.lock()->SetDegAng(_angle);
}

void ObjectManager::DebugObject(std::shared_ptr<StageManager> _stage)
{
	// オブジェクトの動きを止める
	static bool stop = false;
	ImGui::Checkbox((const char*)u8"ストップ", &stop);
	SceneManager::Instance().m_stop = stop;

	std::string _filePath;

	// 現在のシーン シーンごとのファイルを分けているため
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

	// タイトルシーン
	if (ImGui::TreeNode("Title"))
	{
		// カメラ
		if (ImGui::TreeNode("Camera"))
		{
			// 保存
			if (ImGui::Button((const char*)u8"Camera保存"))
			{
				TitleCameraWrite();
			}

			// 生成
			if (ImGui::Button((const char*)u8"Camera追加"))
			{
				AddTitleCamera();
			}

			if (m_titleCamera.expired() == false)
			{
				std::shared_ptr<TitleCamera> _camera = m_titleCamera.lock();

				// 位置
				ImGui::Text((const char*)u8"　カメラ位置 　　x=%.2f,y=%.2f,z=%.2f", _camera->GetPos().x, _camera->GetPos().y, _camera->GetPos().z);
				Math::Vector3 _tcPos = _camera->GetPos();
				ImGui::SliderFloat("TitleCameraPosX", &_tcPos.x, -500, 500);
				ImGui::SliderFloat("TitleCameraPosY", &_tcPos.y, -500, 500);
				ImGui::SliderFloat("TitleCameraPosZ", &_tcPos.z, -500, 500);

				// 角度
				ImGui::Text((const char*)u8"　角度 　　DegAngX=%.2f, DegAngY=%.2f, DegAngZ=%.2f", _camera->GetDegAng().x, _camera->GetDegAng().y, _camera->GetDegAng().z);
				float _tcDegAngX = _camera->GetDegAng().x;
				float _tcDegAngY = _camera->GetDegAng().y;
				float _tcDegAngZ = _camera->GetDegAng().z;
				ImGui::SliderFloat("AngleX", &_tcDegAngX, -180, 180);
				ImGui::SliderFloat("AngleY", &_tcDegAngY, -180, 180);
				ImGui::SliderFloat("AngleZ", &_tcDegAngZ, -180, 180);

				// 視野角
				ImGui::Text((const char*)u8"　視野角 　　ViewingAngle=%.2f", _camera->GetViewingAngle());
				float _tcViewingAngle = _camera->GetViewingAngle();
				ImGui::SliderFloat("ViewingAngle", &_tcViewingAngle, 1.0f, 100.0f);

				// 回転角度変化量
				ImGui::Text((const char*)u8"　回転角度変化量 　　ChangeAngle=%.2f", _camera->GetChangeAngle());
				float _tcChangeAngle=_camera->GetChangeAngle();
				ImGui::SliderFloat("ChangeAngle", &_tcChangeAngle, -10.00f, 10.00f);

				// セット
				_camera->SetPos(_tcPos);
				_camera->SetDegAng(Math::Vector3{ _tcDegAngX,_tcDegAngY,_tcDegAngZ });
				_camera->SetViewingAngle(_tcViewingAngle);
				_camera->SetChangeAngle(_tcChangeAngle);
			}

			ImGui::TreePop();
		}

		// タイトルUI
		if (ImGui::TreeNode("TitleUI"))
		{
			// タイトル名
			if (ImGui::TreeNode("Title"))
			{
				// 保存
				if (ImGui::Button((const char*)u8"Title保存"))
				{
					TitleWrite();
				}
				
				// 生成
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

					// セット
					_title->SetPos(_TitlePos);
					_title->SetSize(_TitleSize);
				}

				ImGui::TreePop();
			}

			// ゲームに入るUI
			if (ImGui::TreeNode("Game"))
			{
				// 保存
				if (ImGui::Button((const char*)u8"Game保存"))
				{
					GameWrite();
				}

				// 生成
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
					ImGui::SliderFloat("Size", &_GameSize, 0.01f, 1.0f);

					// セット
					_game->SetPos(_GamePos);
					_game->SetSize(_GameSize);
				}

				ImGui::TreePop();
			}

			// ゲームを閉じるUI
			if (ImGui::TreeNode("Exit"))
			{
				// 保存
				if (ImGui::Button((const char*)u8"Exit保存"))
				{
					ExitWrite();
				}

				// 生成
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
					ImGui::SliderFloat("Size", &_ExitSize, 0.01f, 1.0f);

					// セット
					_exit->SetPos(_ExitPos);
					_exit->SetSize(_ExitSize);
				}

				ImGui::TreePop();
			}

			// セレクト
			if (ImGui::TreeNode("Select"))
			{
				// 保存
				if (ImGui::Button((const char*)u8"Select保存"))
				{
					SelectWrite();
				}

				// 生成
				if (ImGui::Button((const char*)u8"Select追加"))
				{
					AddSelect();
				}

				if (m_cursor.expired() == false)
				{
					std::shared_ptr<Select> _select = m_select.lock();

					// サイズ
					ImGui::Text((const char*)u8"　サイズ 　Size=%.2f", _select->GetSize());
					float _SelectSize = _select->GetSize();
					ImGui::SliderFloat("Size", &_SelectSize, 0.01f, 1.0f);

					// アルファ値補正値
					ImGui::Text((const char*)u8"　アルファ値補正値 　AlphaCorrection=%.2f", _select->GetAlphaCorrection());
					float _SelectAlphaCorrection = _select->GetAlphaCorrection();
					ImGui::SliderFloat("AlphaCorrection", &_SelectAlphaCorrection, 0.01f, 2.0f);

					// 角度変化量
					ImGui::Text((const char*)u8"　角度変化量 　ChangeAngle=%.2f", _select->GetChangeAngle());
					float _SelectChangeAngle = _select->GetChangeAngle();
					ImGui::SliderFloat("ChangeAngle", &_SelectChangeAngle, 0.01f, 360.0f);

					// セット
					_select->SetSize(_SelectSize);
					_select->SetAlphaCorrection(_SelectAlphaCorrection);
					_select->SetChangeAngle(_SelectChangeAngle);
				}

				ImGui::TreePop();
			}

			// カーソル
			if (ImGui::TreeNode("Cursor"))
			{
				// 保存
				if (ImGui::Button((const char*)u8"Cursor保存"))
				{
					CursorWrite();
				}

				if (m_cursor.expired() == false)
				{
					std::shared_ptr<Cursor> _cursor = m_cursor.lock();

					// サイズ
					ImGui::Text((const char*)u8"　サイズ 　Size=%.2f", _cursor->GetSize());
					float _size = _cursor->GetSize();
					ImGui::SliderFloat("Size", &_size, 0.01f, 1.0f);

					// セット
					_cursor->SetSize(_size);

					// パーティクル
					if (ImGui::TreeNode("Particle"))
					{
						// 保存
						if (ImGui::Button((const char*)u8"Particle保存"))
						{
							CursorParticleWrite();
						}

						// 最大数
						ImGui::Text((const char*)u8"　最大数 　Num=%d", _cursor->GetParticleParam().ParticleNum);
						int _num=_cursor->GetParticleParam().ParticleNum;
						ImGui::SliderInt("Num", &_num, 1, 100);

						// X軸のランダムの数
						ImGui::Text((const char*)u8"　 X軸のランダムの数　XNum=%d", _cursor->GetParticleParam().MoveXNum);
						int _xNum = _cursor->GetParticleParam().MoveXNum;
						ImGui::SliderInt("XNum", &_xNum, 1, 1000);

						// Y軸のランダムの数
						ImGui::Text((const char*)u8"　Y軸のランダムの数 　YNum=%d", _cursor->GetParticleParam().MoveYNum);
						int _yNum = _cursor->GetParticleParam().MoveYNum;
						ImGui::SliderInt("YNum", &_yNum, 1, 1000);

						// 移動量X軸の最低値
						ImGui::Text((const char*)u8"　移動量X軸の最低値 　MoveMinX=%.2f", _cursor->GetParticleParam().MinMove.x);
						float _minMoveX = _cursor->GetParticleParam().MinMove.x;
						ImGui::SliderFloat("MoveMinX", &_minMoveX, 1, 1000);

						// 移動量Y軸の最低値
						ImGui::Text((const char*)u8"　移動量Y軸の最低値 　MoveMinY=%.2f", _cursor->GetParticleParam().MinMove.y);
						float _minMoveY = _cursor->GetParticleParam().MinMove.y;
						ImGui::SliderFloat("MoveMinY", &_minMoveY, 1, 1000);

						// 移動量X軸の補正値
						ImGui::Text((const char*)u8"　移動量X軸の補正値 　MoveCorrectionX=%.2f", _cursor->GetParticleParam().MoveCorrection.x);
						float _moveCorrectionX = _cursor->GetParticleParam().MoveCorrection.x;
						ImGui::SliderFloat("MoveCorrectionX", &_moveCorrectionX, 0.001f, 1.0f);

						// 移動量Y軸の補正値
						ImGui::Text((const char*)u8"　移動量Y軸の補正値 　MoveCorrectionY=%.2f", _cursor->GetParticleParam().MoveCorrection.y);
						float _moveCorrectionY = _cursor->GetParticleParam().MoveCorrection.y;
						ImGui::SliderFloat("MoveCorrectionY", &_moveCorrectionY, 0.001f, 1.0f);

						// サイズのランダムの数
						ImGui::Text((const char*)u8"　サイズのランダムの数 　SizeNum=%d", _cursor->GetParticleParam().SizeNum);
						int _sizeNum = _cursor->GetParticleParam().SizeNum;
						ImGui::SliderInt("SizeNum", &_sizeNum, 1, 100);

						// サイズ最低値
						ImGui::Text((const char*)u8"　サイズ最低値 　MinSize=%.2f", _cursor->GetParticleParam().MinSize);
						float _sizeMin = _cursor->GetParticleParam().MinSize;
						ImGui::SliderFloat("MinSize", &_sizeMin, 0.001f, 10.0f);

						// サイズ補正値
						ImGui::Text((const char*)u8"　サイズ補正値　SizeCorrection=%.2f", _cursor->GetParticleParam().SizeCorrection);
						float _sizeCorrection = _cursor->GetParticleParam().SizeCorrection;
						ImGui::SliderFloat("SizeCorrection", &_sizeCorrection, 0.001f, 10.0f);

						// サイズ変化量
						ImGui::Text((const char*)u8"　サイズ変化量 　SizeChange=%.2f", _cursor->GetParticleParam().SizeChange);
						float _sizeChange = _cursor->GetParticleParam().SizeChange;
						ImGui::SliderFloat("SizeChange", &_sizeChange, 0.001f, 10.0f);

						// セット
						_cursor->SetParticleParam(_num, _xNum, _yNum, Math::Vector2{ _minMoveX,_minMoveY }, Math::Vector2{ _moveCorrectionX,_moveCorrectionY }, _sizeNum, _sizeMin, _sizeCorrection, _sizeChange);

						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	// ゲームシーン
	if (ImGui::TreeNode("Game"))
	{
		// カメラ
		if (ImGui::TreeNode("Camera"))
		{
			// 保存
			if (ImGui::Button((const char*)u8"Camera保存"))
			{
				GameCameraWrite();
			}


			if (m_camera.expired() == false)
			{
				std::shared_ptr<GameCamera> _camera = m_camera.lock();

				// ステートの切り替え
				if (_camera->GetCameraType() != GameCamera::CameraType::PlayerType)
				{
					// プレイヤー
					if (ImGui::Button((const char*)u8"Player"))
					{
						_camera->SetCameraType(GameCamera::CameraType::PlayerType);
						_camera->PlayerChange();
					}
				}
				if (_camera->GetCameraType() != GameCamera::CameraType::FixedType)
				{
					// テレポー開放
					if (ImGui::Button((const char*)u8"Fixed"))
					{
						_camera->SetCameraType(GameCamera::CameraType::FixedType);
						_camera->FixedChange();
					}
				}
				if (_camera->GetCameraType() != GameCamera::CameraType::BossType)
				{
					// テレポー開放
					if (ImGui::Button((const char*)u8"Boss"))
					{
						_camera->SetCameraType(GameCamera::CameraType::BossType);
						_camera->BossChange();
					}
				}
				if (_camera->GetCameraType() != GameCamera::CameraType::ClearType)
				{
					// クリア
					if (ImGui::Button((const char*)u8"Clear"))
					{
						_camera->SetCameraType(GameCamera::CameraType::ClearType);
						_camera->ClearChange();
					}
				}

				// クリア時回転角度変化量
				ImGui::Text((const char*)u8"　クリア時回転角度変化量 　　ChangeClearAngle=%.2f", _camera->GetChangeShakeAngle());
				float ChangeClearAngle = _camera->GetChangeClearAngle();
				ImGui::SliderFloat("ChangeClearAngle", &ChangeClearAngle, 0.01f, 359.99f);

				// 振動時sinカーブ変化量
				ImGui::Text((const char*)u8"　振動sinカーブ変化量 　　ChangeShakeAngle=%.2f", _camera->GetChangeShakeAngle());
				float ChangeShakeAngle=_camera->GetChangeShakeAngle();
				ImGui::SliderFloat("ChangeShakeAngle", &ChangeShakeAngle, 0.1f, 359.9f);

				// 振動移動量
				ImGui::Text((const char*)u8"　振動移動量 　　Move=%.2f", _camera->GetDefaultMove());
				float move = _camera->GetDefaultMove();
				ImGui::SliderFloat("Move", &move, 0.1f, 100.0f);

				// 振動時間
				ImGui::Text((const char*)u8"　振動時間 　　ShakeTime=%d", _camera->GetDefaultShakeTime());
				int time = _camera->GetDefaultShakeTime();
				ImGui::SliderInt("ShakeTime", &time, 1, 300);

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
				else if (angleY < 0.0f)angleY += 360.0f;

				// 視野角
				ImGui::Text((const char*)u8"　視野角 　　ViewAngleX=%.2f", _camera->GetNowViewAng());
				float ViewAngle = _camera->GetNowViewAng();

				ImGui::SliderFloat("ViewAngle", &ViewAngle, 0, 360);

				// セット
				_camera->SetChangeClearAngle(ChangeClearAngle);
				_camera->SetChangeShakeAngle(ChangeShakeAngle);
				_camera->SetDefaultShakeMove(move);
				_camera->SetDefaultShakeTime(time);
				_camera->SetPos(pos);
				_camera->SetDegAng(Math::Vector3{ angleX,angleY,0.0f });
				_camera->SetViewAng(ViewAngle);
			}

			ImGui::TreePop();
		}

		// プレイヤー
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
				ImGui::Text((const char*)u8"　攻撃範囲 ATKRange=%.2f", _player->GetParam().AtkRange);
				float range = _player->GetParam().AtkRange;
				ImGui::SliderFloat("ATKRange", &range, 1, 100);
				// 前方方向
				ImGui::Text((const char*)u8"　前方方向 x=%.2f,y=%.2f,z=%.2f", _player->GetForward().x, _player->GetForward().y, _player->GetForward().z);
				// 無敵時間
				ImGui::Text((const char*)u8"　無敵付与時間 InviTime=%d", _player->GetinviAddTime());
				int _inviTime = _player->GetinviAddTime();
				ImGui::SliderInt("InviTime", &_inviTime, 0, 300);
				// カウンターの攻撃範囲
				ImGui::Text((const char*)u8"　カウンター攻撃範囲 CounterRadius=%.2f", _player->GetCounterRadius());
				float _counterRadius = _player->GetCounterRadius();
				ImGui::SliderFloat("CounterRadius", &_counterRadius, 0.1f, 10.0f);
				// スタミナ回復可能時間 
				ImGui::Text((const char*)u8"　スタミナ回復可能時間 StaminaRecoveryTime=%d", _player->GetStaminaRecoveryTime());
				int _StaminaRecoveryTime = _player->GetStaminaRecoveryTime();
				ImGui::SliderInt("StaminaRecoveryTime", &_StaminaRecoveryTime, 0, 600);
				// 回避した時のスタミナの減少量
				ImGui::Text((const char*)u8"　回避した時のスタミナの減少量 RollStamina=%d", _player->GetRollStamina());
				int _rollStamina = _player->GetRollStamina();
				ImGui::SliderInt("RollStamina", &_rollStamina, 0, _player->GetMaxStamina());
				// パリィ可能時間
				ImGui::Text((const char*)u8"　パリィ可能時間 ParryTime=%d", _player->GetParryTime());
				int _parryTime = _player->GetParryTime();
				ImGui::SliderInt("parryTime", &_parryTime, 0, 60);
				// 武器
				static std::string _swordName = _player->GetSword().lock()->GetName().c_str();
				static std::string _shieldName = _player->GetShield().lock()->GetName().c_str();
				if (ImGui::TreeNode("Weapon"))
				{
					// 剣
					if (_player->GetSword().expired() == false)
					{
						std::shared_ptr<Sword> _sword = _player->GetSword().lock();
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
							Math::Vector3 swordPos = _sword->GetPos();
							ImGui::SliderFloat("PosX", &swordPos.x, -0.5, 0.5);
							ImGui::SliderFloat("PosY", &swordPos.y, -0.5, 0.5);
							ImGui::SliderFloat("PosZ", &swordPos.z, -0.5, 0.5);
							
							// 攻撃力
							ImGui::Text((const char*)u8"　攻撃力 　ATK=%d", _sword->GetATK());
							int swordAtk = _sword->GetATK();
							ImGui::SliderInt("ATK", &swordAtk, 1, 100);
							
							// 大きさ
							ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _sword->GetSize());
							float swordSize = _sword->GetSize();
							ImGui::SliderFloat("Size", &swordSize, 1, 100);
							
							// 攻撃判定のスフィアサイズ
							ImGui::Text((const char*)u8"　攻撃判定のスフィアサイズ　AttackSphereSize=%.2f", _sword->GetAttackSphereSize());
							float swordAttackSphereSize = _sword->GetAttackSphereSize();
							ImGui::SliderFloat("AttackSphereSize", &swordAttackSphereSize, 0.1f, 1.0f);

							// トレイルポリゴン
							ImGui::Text((const char*)u8"　トレイルポリゴン　Traject=%d", _sword->GetTraject());
							int traje = _sword->GetTraject();
							ImGui::SliderInt("Traject", &traje, 1, 100);

							// セット
							_sword->SetPos(swordPos);
							_sword->SetATK(swordAtk);
							_sword->SetSize(swordSize);
							_sword->SetAttackSphereSize(swordAttackSphereSize);
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

							// セット
							_shield->SetPos(shieldPos);
							_shield->SetSize(shieldSize);

							ImGui::TreePop();
						}
					}

					ImGui::TreePop();

					// 武器の切替
					ChangeWeapon(_swordName, _shieldName);
				}

				// セット
				_player->SetParam(hp, _player->GetSword().lock()->GetATK(), speed, stamina);
				_player->SetPos(pos);
				_player->SetAngle(angle);
				_player->SetSize(size);
				_player->SetAtkRange(range);
				_player->SetInviAddTime(_inviTime);
				_player->SetCounterRadius(_counterRadius);
				_player->SetStaminaRecoveryTime(_StaminaRecoveryTime);
				_player->SetRollStamina(_rollStamina);
				_player->SetParryTime(_parryTime);

				// UI
				if (ImGui::TreeNode("UI"))
				{
					// HP
					if (ImGui::TreeNode("HP"))
					{
						// 保存
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
							ImGui::SliderFloat("Size", &_HPSize, 0.01f, 1.0f);

							// 減少ゲージ減少開始時間
							ImGui::Text((const char*)u8"　減少ゲージ減少開始時間　DownTime=%d", _hp->GetDownTime());
							int _DownTime = _hp->GetDownTime();
							ImGui::SliderInt("DownTime", &_DownTime, 1, 300);

							// 減少ゲージ変化量
							ImGui::Text((const char*)u8"　減少ゲージ変化量　DownChange=%.2f", _hp->GetDownChange());
							float _DownChange = _hp->GetDownChange();
							ImGui::SliderFloat("DownChange", &_DownChange, 0.01f, 100.0f);

							// セット
							_hp->SetPos(_HPPos);
							_hp->SetSize(_HPSize);
							_hp->SetDownTime(_DownTime);
							_hp->SetDownChange(_DownChange);
						}

						ImGui::TreePop();
					}

					// スタミナ
					if (ImGui::TreeNode("Stamina"))
					{
						// 保存
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
							ImGui::SliderFloat("Size", &_StaminaSize, 0.01f, 1.0f);

							// セット
							_stamina->SetPos(_StaminaPos);
							_stamina->SetSize(_StaminaSize);
						}

						ImGui::TreePop();
					}

					// ステージ数
					if (ImGui::TreeNode("Floor"))
					{
						// 保存
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

							// セット
							_floor->SetPos(_FloorPos);
							_floor->SetSize(_FloorSize);
						}

						ImGui::TreePop();
					}

					// ゲームオーバー・クリア
					if (ImGui::TreeNode("GameStateUI"))
					{
						// 保存
						if (ImGui::Button((const char*)u8"GameStateUI保存"))
						{
							GameStateWrite();
						}

						if (m_gameStateUI.expired() == false)
						{
							std::shared_ptr<GameStateUI>_gameState = m_gameStateUI.lock();

							// 位置
							ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f",_gameState->GetVector2Pos().x,_gameState->GetVector2Pos().y);
							Math::Vector2 _GameStatePos =_gameState->GetVector2Pos();
							ImGui::SliderFloat("PosX", &_GameStatePos.x, -640, 640);
							ImGui::SliderFloat("PosY", &_GameStatePos.y, -360, 360);

							// 大きさ
							ImGui::Text((const char*)u8"　大きさ 　Size=%.2f",_gameState->GetSize());
							float _GameStateSize =_gameState->GetSize();
							ImGui::SliderFloat("Size", &_GameStateSize, 1, 100);

							// アルファ値変化量
							ImGui::Text((const char*)u8"　アルファ値変化量 　ChangeAlpha=%.2f",_gameState->GetAlphaChange());
							float _GameStateAlphaChange =_gameState->GetAlphaChange();
							ImGui::SliderFloat("AlphaChange", &_GameStateAlphaChange, 0.01f, 1.00f);

							// セット
							_gameState->SetPos(_GameStatePos);
							_gameState->SetSize(_GameStateSize);
							_gameState->SetChangeAlpha(_GameStateAlphaChange);
						}

						ImGui::TreePop();
					}

					ImGui::TreePop();
				}
				ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");

				ImGui::TreePop();
			}
		}

		// 敵
		if (ImGui::TreeNode("Enemy"))
		{
			// 骨
			std::vector<std::weak_ptr<EnemyBase>> _boneList;
			// 骨色違い
			std::vector<std::weak_ptr<EnemyBase>> _alphaList;
			// リストに格納 オブジェクトの名前で判断
			for (auto& enemy : SceneManager::Instance().GetEnemyList())
			{
				if (enemy->GetName() == "Bone") // 骨
				{
					_boneList.push_back(enemy);
				}
				if (enemy->GetName() == "BoneAlpha") // 骨色違い
				{
					_alphaList.push_back(enemy);
				}
			}

			// ステージの数
			static int _stageNum = 1;
			ImGui::Text((const char*)u8"ステージ数 : %d", _stageNum);
			
			// 現在のステージ
			static int _nowStage = _stage->GetnowStage();
			
			// 設定したいステージに切り替え
			ImGui::SliderInt((const char*)u8"ステージ", &_nowStage, 1, _stageNum);
			// ステージ追加
			if (ImGui::Button((const char*)u8"ステージ追加"))
			{
				_stageNum++;
				if (_stageNum - 1 == _nowStage)_nowStage = _stageNum;
			}


			// ウェーブ数
			static int _wave = _stage->GetMaxWave();
			ImGui::Text((const char*)u8"ウェーブ数 : %d", _wave);
			
			// 現在のウェーブ
			static int _nowWave = _stage->GetnowWave();
			
			// 設定したいウェーブに切り替え
			ImGui::SliderInt((const char*)u8"ウェーブ", &_nowWave, 1, _wave);
			// ウェーブ追加
			if (ImGui::Button((const char*)u8"ウェーブ追加"))
			{
				_wave++;
				if (_wave - 1 == _nowWave)_nowWave = _wave;
			}
			// 削除
			if (ImGui::Button((const char*)u8"ウェーブ削除"))
			{
				if (_wave > 1)
				{
					_wave--;
					if (_wave + 1 == _nowWave)_nowWave = _wave;
				}
			}

			// 保存
			if (ImGui::Button((const char*)u8"Enemy保存"))
			{
				EnemyWrite(_nowStage, _nowWave, (("Asset/Json/") + _filePath + ("/Enemy/Stage")));
				if (_nowWave == _wave)
				{
					_wave++;
					_nowWave = _wave;
				}
			}

			// 骨
			if (ImGui::TreeNode("Bone"))
			{
				// 生成
				if (ImGui::Button((const char*)u8"Bone追加"))
				{
					AddBone();
				}

				// 設定したい敵
				static int operation = -1;
				if (!SceneManager::Instance().m_stop)operation = -1;

				// 骨の数
				ImGui::Text((const char*)u8"ボーン:%d体", _boneList.size());
				for (unsigned int bone = 0; bone < _boneList.size(); ++bone)
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
					// 出現演出時のエフェクトサイズ
					ImGui::Text((const char*)u8"　出現演出時のエフェクトサイズ 　AppealSize=%.2f", _boneList[operation].lock()->GetAppealEffectSize());
					float appealSize = _boneList[operation].lock()->GetAppealEffectSize();
					ImGui::SliderFloat("AppealSize", &appealSize, 0.1f, 10.0f);
					// 攻撃範囲
					ImGui::Text((const char*)u8"　攻撃範囲 ATKRange=%.2f", _boneList[operation].lock()->GetParam().AtkRange);
					float range = _boneList[operation].lock()->GetParam().AtkRange;
					ImGui::SliderFloat("ATKRange", &range, 1, 100);
					// 他の敵との距離判定時のスフィアの半径
					ImGui::Text((const char*)u8"　他の敵との距離判定時のスフィアの半径 EnemyCheckRadius=%.2f", _boneList[operation].lock()->GetEnemyCheckRadius());
					float radius = _boneList[operation].lock()->GetEnemyCheckRadius();
					ImGui::SliderFloat("EnemyCheckRadius", &radius, 0.1f, 10.0f);
					// 避ける距離
					ImGui::Text((const char*)u8"　避ける距離 LeaveDist=%.2f", _boneList[operation].lock()->GetLeaveDist());
					float leaveDist = _boneList[operation].lock()->GetLeaveDist();
					ImGui::SliderFloat("LeaveDist", &leaveDist, 0.1f, 20.0f);
					// 攻撃判定のスフィアサイズ
					ImGui::Text((const char*)u8"　攻撃判定のスフィアサイズ AttackSphereSize=%.2f", _boneList[operation].lock()->GetAttackSphereSize());
					float attackSphere = _boneList[operation].lock()->GetAttackSphereSize();
					ImGui::SliderFloat("AttackSphereSize", &attackSphere, 0.1f, 5.0f);
					// よろける時の移動スピード
					ImGui::Text((const char*)u8"　よろける時の移動スピード StumbleMove=%.2f", _boneList[operation].lock()->GetStumbleMove());
					float stumbleMove = _boneList[operation].lock()->GetStumbleMove();
					ImGui::SliderFloat("StumbleMove", &stumbleMove, 0.1f, 1.0f);
					// ボスフラグ
					std::string b;
					if (_boneList[operation].lock()->GetBossFlg())b = "true";
					else { b = "flase"; }
					ImGui::Text((const char*)u8"　ボスフラグ BossFlg=%s", b.c_str());
					int boss = _boneList[operation].lock()->GetBossFlg();
					ImGui::SliderInt("BossFlg", &boss, 0, 1);
					// 前方方向
					ImGui::Text((const char*)u8"　前方方向 x=%.2f,y=%.2f,z=%.2f", _boneList[operation].lock()->GetForward().x, _boneList[operation].lock()->GetForward().y, _boneList[operation].lock()->GetForward().z);

					// セット
					_boneList[operation].lock()->SetParam(hp, atk, speed, stamina);
					_boneList[operation].lock()->SetPos(pos);
					_boneList[operation].lock()->SetAngle(angle);
					_boneList[operation].lock()->SetSize(size);
					_boneList[operation].lock()->SetAppealEffectSize(appealSize);
					_boneList[operation].lock()->SetAtkRange(range);
					_boneList[operation].lock()->SetEnemyCheckRadius(radius);
					_boneList[operation].lock()->SetLeaveDist(leaveDist);
					_boneList[operation].lock()->SetAttackSphereSize(attackSphere);
					_boneList[operation].lock()->SetStumbleMove(stumbleMove);
					_boneList[operation].lock()->SetBossFlg(boss);
					if (boss && (m_camera.lock()->GetBossTarget().expired() || _boneList[operation].lock()->GetID() != m_camera.lock()->GetBossTarget().lock()->GetID()))
					{
						m_camera.lock()->SetBossTarget(_boneList[operation].lock());
					}

					//消滅
					if (ImGui::Button((const char*)u8"消滅"))
					{
						_boneList[operation].lock()->Expired();
						operation = -1;
					}

					ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");
				}

				ImGui::TreePop();
			}

			// 骨色違い
			if (ImGui::TreeNode("BoneAlpha"))
			{
				// 生成
				if (ImGui::Button((const char*)u8"BoneAlpha追加"))
				{
					AddBoneAlpha();
				}

				// 設定したい敵
				static int operation = -1;
				if (!SceneManager::Instance().m_stop)operation = -1;

				// 骨色違いの数
				ImGui::Text((const char*)u8"ボーンアルファ:%d体", _alphaList.size());
				for (unsigned int alpha = 0; alpha < _alphaList.size(); ++alpha)
				{
					if (ImGui::Button(std::to_string(alpha + 1).c_str()))
					{
						operation = alpha;
					}
				}

				if (operation != -1)
				{
					ImGui::Text((const char*)u8"%d体目", operation + 1);
					// 体力
					ImGui::Text((const char*)u8"　体力 　　HP=%d", _alphaList[operation].lock()->GetParam().Hp);
					int hp = _alphaList[operation].lock()->GetParam().Hp;
					ImGui::SliderInt("HP", &hp, 1, 100);
					// 攻撃力
					ImGui::Text((const char*)u8"　攻撃力 　ATK=%d", _alphaList[operation].lock()->GetParam().Atk);
					int atk = _alphaList[operation].lock()->GetParam().Atk;
					ImGui::SliderInt("ATK", &atk, 1, 100);
					// 素早さ
					ImGui::Text((const char*)u8"　素早さ 　SP=%.2f", _alphaList[operation].lock()->GetParam().Sp);
					float speed = _alphaList[operation].lock()->GetParam().Sp;
					ImGui::SliderFloat("Speed", &speed, 1, 100);
					// スタミナ
					ImGui::Text((const char*)u8"　スタミナ SM=%d", _alphaList[operation].lock()->GetParam().Sm);
					int stamina = _alphaList[operation].lock()->GetParam().Sm;
					ImGui::SliderInt("Stamina", &stamina, 1, 100);
					// 位置
					ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", _alphaList[operation].lock()->GetPos().x, _alphaList[operation].lock()->GetPos().y, _alphaList[operation].lock()->GetPos().z);
					Math::Vector3 pos = _alphaList[operation].lock()->GetPos();
					ImGui::SliderFloat("PosX", &pos.x, -100, 100);
					ImGui::SliderFloat("PosY", &pos.y, 0, 400);
					ImGui::SliderFloat("PosZ", &pos.z, -100, 100);
					// 方向
					ImGui::Text((const char*)u8"　方向 　　x=%.2f,y=%.2f,z=%.2f", _alphaList[operation].lock()->GetDir().x, _alphaList[operation].lock()->GetDir().y, _alphaList[operation].lock()->GetDir().z);
					// 角度
					ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", _alphaList[operation].lock()->GetAngle());
					Math::Vector3 angle = _alphaList[operation].lock()->GetAngle();
					ImGui::SliderFloat("Angle", &angle.y, 0, 360);
					// 大きさ
					ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _alphaList[operation].lock()->GetSize());
					float size = _alphaList[operation].lock()->GetSize();
					ImGui::SliderFloat("Size", &size, 1, 100);
					// 出現演出時のエフェクトサイズ
					ImGui::Text((const char*)u8"　出現演出時のエフェクトサイズ 　AppealSize=%.2f", _alphaList[operation].lock()->GetAppealEffectSize());
					float appealSize = _alphaList[operation].lock()->GetAppealEffectSize();
					ImGui::SliderFloat("AppealSize", &appealSize, 0.1f, 10.0f);
					// 攻撃範囲
					ImGui::Text((const char*)u8"　攻撃範囲 ATKRange=%.2f", _alphaList[operation].lock()->GetParam().AtkRange);
					float range = _alphaList[operation].lock()->GetParam().AtkRange;
					ImGui::SliderFloat("ATKRange", &range, 1, 100);
					// 他の敵との距離判定時のスフィアの半径
					ImGui::Text((const char*)u8"　他の敵との距離判定時のスフィアの半径 EnemyCheckRadius=%.2f", _alphaList[operation].lock()->GetEnemyCheckRadius());
					float radius = _alphaList[operation].lock()->GetEnemyCheckRadius();
					ImGui::SliderFloat("EnemyCheckRadius", &radius, 0.1f, 10.0f);
					// 避ける距離
					ImGui::Text((const char*)u8"　避ける距離 LeaveDist=%.2f", _alphaList[operation].lock()->GetLeaveDist());
					float leaveDist = _alphaList[operation].lock()->GetLeaveDist();
					ImGui::SliderFloat("LeaveDist", &leaveDist, 0.1f, 20.0f);
					// 攻撃判定のスフィアサイズ
					ImGui::Text((const char*)u8"　攻撃判定のスフィアサイズ AttackSphereSize=%.2f", _alphaList[operation].lock()->GetAttackSphereSize());
					float attackSphere = _alphaList[operation].lock()->GetAttackSphereSize();
					ImGui::SliderFloat("AttackSphereSize", &attackSphere, 0.1f, 5.0f);
					// よろける時の移動スピード
					ImGui::Text((const char*)u8"　よろける時の移動スピード StumbleMove=%.2f", _alphaList[operation].lock()->GetStumbleMove());
					float stumbleMove = _alphaList[operation].lock()->GetStumbleMove();
					ImGui::SliderFloat("StumbleMove", &stumbleMove, 0.1f, 1.0f);
					// ボスフラグ
					std::string b;
					if (_alphaList[operation].lock()->GetBossFlg())b = "true";
					else { b = "flase"; }
					ImGui::Text((const char*)u8"　ボスフラグ BossFlg=%s", b.c_str());
					int boss = _alphaList[operation].lock()->GetBossFlg();
					ImGui::SliderInt("BossFlg", &boss, 0, 1);
					// 前方方向
					ImGui::Text((const char*)u8"　前方方向 x=%.2f,y=%.2f,z=%.2f", _alphaList[operation].lock()->GetForward().x, _alphaList[operation].lock()->GetForward().y, _alphaList[operation].lock()->GetForward().z);

					// セット
					_alphaList[operation].lock()->SetParam(hp, atk, speed, stamina);
					_alphaList[operation].lock()->SetPos(pos);
					_alphaList[operation].lock()->SetAngle(angle);
					_alphaList[operation].lock()->SetSize(size);
					_alphaList[operation].lock()->SetAppealEffectSize(appealSize);
					_alphaList[operation].lock()->SetAtkRange(range);
					_alphaList[operation].lock()->SetEnemyCheckRadius(radius);
					_alphaList[operation].lock()->SetLeaveDist(leaveDist);
					_alphaList[operation].lock()->SetAttackSphereSize(attackSphere);
					_alphaList[operation].lock()->SetStumbleMove(stumbleMove);
					_alphaList[operation].lock()->SetBossFlg(boss);
					if (boss && (m_camera.lock()->GetBossTarget().expired() || _alphaList[operation].lock()->GetID() != m_camera.lock()->GetBossTarget().lock()->GetID()))
					{
						m_camera.lock()->SetBossTarget(_alphaList[operation].lock());
					}

					// 消滅
					if (ImGui::Button((const char*)u8"消滅"))
					{
						_alphaList[operation].lock()->Expired();
						operation = -1;
					}

					ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");
				}

				ImGui::TreePop();
			}

			// ゴーレム
			if (ImGui::TreeNode("Golem"))
			{
				// 生成
				if (ImGui::Button((const char*)u8"Golem追加"))
				{
					AddGolem();
				}

				if (!m_golem.expired())
				{
					// 体力
					ImGui::Text((const char*)u8"　体力 　　HP=%d", m_golem.lock()->GetParam().Hp);
					int hp = m_golem.lock()->GetParam().Hp;
					ImGui::SliderInt("HP", &hp, 1, 100);
					// 攻撃力
					ImGui::Text((const char*)u8"　攻撃力 　ATK=%d", m_golem.lock()->GetParam().Atk);
					int atk = m_golem.lock()->GetParam().Atk;
					ImGui::SliderInt("ATK", &atk, 1, 100);
					// 素早さ
					ImGui::Text((const char*)u8"　素早さ 　SP=%.2f", m_golem.lock()->GetParam().Sp);
					float speed = m_golem.lock()->GetParam().Sp;
					ImGui::SliderFloat("Speed", &speed, 1, 100);
					// スタミナ
					ImGui::Text((const char*)u8"　スタミナ SM=%d", m_golem.lock()->GetParam().Sm);
					int stamina = m_golem.lock()->GetParam().Sm;
					ImGui::SliderInt("Stamina", &stamina, 1, 100);
					// 位置
					ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", m_golem.lock()->GetPos().x, m_golem.lock()->GetPos().y, m_golem.lock()->GetPos().z);
					Math::Vector3 pos = m_golem.lock()->GetPos();
					ImGui::SliderFloat("PosX", &pos.x, -100, 100);
					ImGui::SliderFloat("PosY", &pos.y, 0, 400);
					ImGui::SliderFloat("PosZ", &pos.z, -100, 100);
					// 方向
					ImGui::Text((const char*)u8"　方向 　　x=%.2f,y=%.2f,z=%.2f", m_golem.lock()->GetDir().x, m_golem.lock()->GetDir().y, m_golem.lock()->GetDir().z);
					// 角度
					ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", m_golem.lock()->GetAngle());
					Math::Vector3 angle = m_golem.lock()->GetAngle();
					ImGui::SliderFloat("Angle", &angle.y, 0, 360);
					// 大きさ
					ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", m_golem.lock()->GetSize());
					float size = m_golem.lock()->GetSize();
					ImGui::SliderFloat("Size", &size, 0.01f, 1.5f);
					// 出現演出時のエフェクトサイズ
					ImGui::Text((const char*)u8"　出現演出時のエフェクトサイズ 　AppealSize=%.2f", m_golem.lock()->GetAppealEffectSize());
					float appealSize = m_golem.lock()->GetAppealEffectSize();
					ImGui::SliderFloat("AppealSize", &appealSize, 0.1f, 10.0f);
					// 攻撃範囲
					ImGui::Text((const char*)u8"　攻撃範囲 ATKRange=%.2f", m_golem.lock()->GetParam().AtkRange);
					float range = m_golem.lock()->GetParam().AtkRange;
					ImGui::SliderFloat("ATKRange", &range, 1, 100);
					// 弾攻撃の射程角度
					ImGui::Text((const char*)u8"　弾攻撃の射程角度 Attack1Angle=%.2f", m_golem.lock()->GetAttack1Angle());
					float attack1Ang = m_golem.lock()->GetAttack1Angle();
					ImGui::SliderFloat("Attack1Angle", &attack1Ang, 0.1f, 360);
					// ジャンプ攻撃の最短距離
					ImGui::Text((const char*)u8"　ジャンプ攻撃の最短距離 Attack2Dist=%.2f", m_golem.lock()->GetAttack2Dist());
					float attack2Dist = m_golem.lock()->GetAttack2Dist();
					ImGui::SliderFloat("Attack2Dist", &attack2Dist, 0.1f, 100);
					// 他の敵との距離判定時のスフィアの半径
					ImGui::Text((const char*)u8"　他の敵との距離判定時のスフィアの半径 EnemyCheckRadius=%.2f", m_golem.lock()->GetEnemyCheckRadius());
					float radius = m_golem.lock()->GetEnemyCheckRadius();
					ImGui::SliderFloat("EnemyCheckRadius", &radius, 0.1f, 10.0f);
					// 避ける距離
					ImGui::Text((const char*)u8"　避ける距離 LeaveDist=%.2f", m_golem.lock()->GetLeaveDist());
					float leaveDist = m_golem.lock()->GetLeaveDist();
					ImGui::SliderFloat("LeaveDist", &leaveDist, 0.1f, 20.0f);
					// 攻撃判定のスフィアサイズ
					ImGui::Text((const char*)u8"　攻撃判定のスフィアサイズ AttackSphereSize=%.2f", m_golem.lock()->GetAttackSphereSize());
					float attackSphere = m_golem.lock()->GetAttackSphereSize();
					ImGui::SliderFloat("AttackSphereSize", &attackSphere, 0.1f, 5.0f);
					// よろける時の移動スピード
					ImGui::Text((const char*)u8"　よろける時の移動スピード StumbleMove=%.2f", m_golem.lock()->GetStumbleMove());
					float stumbleMove = m_golem.lock()->GetStumbleMove();
					ImGui::SliderFloat("StumbleMove", &stumbleMove, 0.1f, 1.0f);
					// ボスフラグ
					std::string b;
					if (m_golem.lock()->GetBossFlg())b = "true";
					else { b = "flase"; }
					ImGui::Text((const char*)u8"　ボスフラグ BossFlg=%s", b.c_str());
					int boss = m_golem.lock()->GetBossFlg();
					ImGui::SliderInt("BossFlg", &boss, 0, 1);
					// 前方方向
					ImGui::Text((const char*)u8"　前方方向 x=%.2f,y=%.2f,z=%.2f", m_golem.lock()->GetForward().x, m_golem.lock()->GetForward().y, m_golem.lock()->GetForward().z);

					// セット
					m_golem.lock()->SetParam(hp, atk, speed, stamina);
					m_golem.lock()->SetPos(pos);
					m_golem.lock()->SetAngle(angle);
					m_golem.lock()->SetSize(size);
					m_golem.lock()->SetAppealEffectSize(appealSize);
					m_golem.lock()->SetAtkRange(range);
					m_golem.lock()->SetAttack1Angle(attack1Ang);
					m_golem.lock()->SetAttack2Dist(attack2Dist);
					m_golem.lock()->SetEnemyCheckRadius(radius);
					m_golem.lock()->SetLeaveDist(leaveDist);
					m_golem.lock()->SetAttackSphereSize(attackSphere);
					m_golem.lock()->SetStumbleMove(stumbleMove);
					m_golem.lock()->SetBossFlg(boss);
					if (boss && (m_camera.lock()->GetBossTarget().expired() || m_golem.lock()->GetID() != m_camera.lock()->GetBossTarget().lock()->GetID()))
					{
						m_camera.lock()->SetBossTarget(m_golem.lock());
					}

					// 消滅
					if (ImGui::Button((const char*)u8"消滅"))
					{
						m_golem.lock()->Expired();
					}

					ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");
				}

				ImGui::TreePop();
			}

			// UI
			if (ImGui::TreeNode("UI"))
			{
				// HP
				if (ImGui::TreeNode("HP"))
				{
					// 保存
					if (ImGui::Button((const char*)u8"HP保存"))
					{
						EnemyHPWrite();
					}

					// 通常
					if (ImGui::TreeNode((const char*)u8"通常"))
					{
						for (auto& _normal : m_EnemyHPList[EnemyHPType::Normal])
						{
							if (_normal.expired())continue;

							std::shared_ptr<Enemy_HP> _hp = _normal.lock();

							// 位置
							ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f", _hp->GetVector2Pos().x, _hp->GetVector2Pos().y);
							Math::Vector2 _HPPos = _hp->GetVector2Pos();
							ImGui::SliderFloat("PosX", &_HPPos.x, -640, 640);
							ImGui::SliderFloat("PosY", &_HPPos.y, -360, 360);

							// 位置補正
							ImGui::Text((const char*)u8"　位置補正 　　x=%.2f", _hp->GetPosXCorrection());
							float _HPPosCorrection = _hp->GetPosXCorrection();
							ImGui::SliderFloat("PosCorrection", &_HPPosCorrection, -640, 640);

							// 大きさ
							ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _hp->GetSize());
							float _HPSize = _hp->GetSize();
							ImGui::SliderFloat("Size", &_HPSize, 0.01f, 1.0f);

							// セット
							_hp->SetPos(_HPPos);
							_hp->SetPosXCorrection(_HPPosCorrection);
							_hp->SetSize(_HPSize);
						}
						ImGui::TreePop();
					}

					// ボス
					if (ImGui::TreeNode((const char*)u8"ボス"))
					{
						for (auto& _boss : m_EnemyHPList[EnemyHPType::Boss])
						{
							if (_boss.expired())continue;

							std::shared_ptr<Enemy_HP> _hp = _boss.lock();

							// 位置
							ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f", _hp->GetVector2Pos().x, _hp->GetVector2Pos().y);
							Math::Vector2 _HPPos = _hp->GetVector2Pos();
							ImGui::SliderFloat("PosX", &_HPPos.x, -640, 640);
							ImGui::SliderFloat("PosY", &_HPPos.y, -360, 360);

							// 大きさ
							ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", _hp->GetSize());
							float _HPSize = _hp->GetSize();
							ImGui::SliderFloat("Size", &_HPSize, 0.01f, 1.0f);

							// セット
							_hp->SetPos(_HPPos);
							_hp->SetSize(_HPSize);
						}
						ImGui::TreePop();
					}

					for (auto& category : m_EnemyHPList)
					{
						for (auto& hp : category)
						{
							if (hp.expired())continue;

							std::shared_ptr<Enemy_HP> _hp = hp.lock();

							// 減少ゲージ減少開始時間
							ImGui::Text((const char*)u8"　減少ゲージ減少開始時間　DownTime=%d", _hp->GetDownTime());
							int _DownTime = _hp->GetDownTime();
							ImGui::SliderInt("DownTime", &_DownTime, 1, 300);

							// 減少ゲージ変化量
							ImGui::Text((const char*)u8"　減少ゲージ変化量　DownChange=%.2f", _hp->GetDownChange());
							float _DownChange = _hp->GetDownChange();
							ImGui::SliderFloat("DownChange", &_DownChange, 0.01f, 100.0f);

							// セット
							_hp->SetDownTime(_DownTime);
							_hp->SetDownChange(_DownChange);
						}
					}

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		// キャラクター以外のオブジェクト
		if (ImGui::TreeNode("Object"))
		{
			// 保存
			if (ImGui::Button((const char*)u8"Object保存")) ObjectWrite(("Asset/Json/") + _filePath + ("/Object/Object.json"));
			
			// 地面
			if (ImGui::TreeNode("Ground"))
			{
				// 生成
				if (ImGui::Button((const char*)u8"Ground追加")) AddGround();

				// 位置
				ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", m_ground.lock()->GetPos().x, m_ground.lock()->GetPos().y, m_ground.lock()->GetPos().z);
				Math::Vector3 pos = m_ground.lock()->GetPos();
				ImGui::SliderFloat("PosX", &pos.x, -100, 100);
				ImGui::SliderFloat("PosY", &pos.y, 0, 500);
				ImGui::SliderFloat("PosZ", &pos.z, -100, 100);
				// 角度
				ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", m_ground.lock()->GetAngle());
				Math::Vector3 angle = m_ground.lock()->GetAngle();
				ImGui::SliderFloat("Angle", &angle.y, 0, 360);
				// 大きさ
				ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", m_ground.lock()->GetSize());
				float size = m_ground.lock()->GetSize();
				ImGui::SliderFloat("Size", &size, 1, 100);

				// セット
				m_ground.lock()->SetPos(pos);
				m_ground.lock()->SetSize(size);
				m_ground.lock()->SetAngle(angle);
				
				ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");

				ImGui::TreePop();
			}

			// 魔法陣の台
			if (ImGui::TreeNode("Circle"))
			{
				// 生成
				if (ImGui::Button((const char*)u8"Circle追加")) AddCircle();

				// 角度
				ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", m_circle.lock()->GetAngle());
				Math::Vector3 angle = m_circle.lock()->GetAngle();
				ImGui::SliderFloat("Angle", &angle.y, 0, 360);
				// 大きさ
				ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", m_circle.lock()->GetSize());
				float size = m_circle.lock()->GetSize();
				ImGui::SliderFloat("Size", &size, 1, 100);
				// 位置
				Math::Vector3 pos;
				ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", m_circle.lock()->GetPos().x, m_circle.lock()->GetPos().y, m_circle.lock()->GetPos().z);
				pos = m_circle.lock()->GetPos();
				if (m_ground.expired() == false)pos.y = m_ground.lock()->GetPos().y;
				ImGui::SliderFloat("PosX", &pos.x, -100, 100);
				ImGui::SliderFloat("PosZ", &pos.z, -100, 100);

				// セット
				m_circle.lock()->SetSize(size);
				m_circle.lock()->SetAngle(angle);
				m_circle.lock()->SetPos(pos);


				ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");

				// 魔法陣
				if (ImGui::TreeNode("Magic"))
				{
					if (m_magic.expired() == false)
					{
						// 大きさ
						ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", m_magic.lock()->GetSize());
						float size = m_magic.lock()->GetSize();
						ImGui::SliderFloat("Size", &size, 1, 100);

						// RGB変化量
						ImGui::Text((const char*)u8"　RGB変化量 　RGBChange=%.2f", m_magic.lock()->GetRGBChange());
						float rgb = m_magic.lock()->GetRGBChange();
						ImGui::SliderFloat("RGB", &rgb, 0.001f, 100.0f);

						// 角度変化量
						ImGui::Text((const char*)u8"　角度変化量 　ChangeAngle=%.2f", m_magic.lock()->GetChangeAngle());
						float angle = m_magic.lock()->GetChangeAngle();
						ImGui::SliderFloat("ChangeAngle", &angle, 0.001f, 360.0f);


						// セット
						m_magic.lock()->SetSize(size);
						m_magic.lock()->SetRGBChange(rgb);
						m_magic.lock()->SetChangeAngle(angle);

					}

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			// 壁
			if (ImGui::TreeNode("Wall"))
			{
				// 生成
				if (ImGui::Button((const char*)u8"Wall追加")) AddWall();

				// 位置
				ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", m_wall.lock()->GetPos().x, m_wall.lock()->GetPos().y, m_wall.lock()->GetPos().z);
				Math::Vector3 pos = m_wall.lock()->GetPos();
				ImGui::SliderFloat("PosX", &pos.x, -100, 100);
				ImGui::SliderFloat("PosY", &pos.y, 0, 500);
				ImGui::SliderFloat("PosZ", &pos.z, -100, 100);
				// 角度
				ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", m_wall.lock()->GetAngle());
				Math::Vector3 angle = m_wall.lock()->GetAngle();
				ImGui::SliderFloat("Angle", &angle.y, 0, 360);
				// 大きさ
				ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", m_wall.lock()->GetSize());
				float size = m_wall.lock()->GetSize();
				ImGui::SliderFloat("Size", &size, 1, 100);

				// セット
				m_wall.lock()->SetPos(pos);
				m_wall.lock()->SetSize(size);
				m_wall.lock()->SetAngle(angle);
				
				ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");
				
				ImGui::TreePop();
			}

			// 空
			if (ImGui::TreeNode("SkyBox"))
			{
				// 生成
				if (ImGui::Button((const char*)u8"SkyBox追加")) AddSkyBox();

				// 位置
				ImGui::Text((const char*)u8"　位置 　　x=%.2f,y=%.2f,z=%.2f", m_skybox.lock()->GetPos().x, m_skybox.lock()->GetPos().y, m_skybox.lock()->GetPos().z);
				Math::Vector3 pos = m_skybox.lock()->GetPos();
				ImGui::SliderFloat("PosX", &pos.x, -500, 500);
				ImGui::SliderFloat("PosY", &pos.y, 0, 500);
				ImGui::SliderFloat("PosZ", &pos.z, -500, 500);
				// 角度
				ImGui::Text((const char*)u8"　角度 　　Angle=%.2f", m_skybox.lock()->GetAngle());
				Math::Vector3 angle = m_skybox.lock()->GetAngle();
				ImGui::SliderFloat("Angle", &angle.y, 0, 360);
				// 大きさ
				ImGui::Text((const char*)u8"　大きさ 　Size=%.2f", m_skybox.lock()->GetSize());
				float size = m_skybox.lock()->GetSize();
				ImGui::SliderFloat("Size", &size, 1, 100);

				// セット
				m_skybox.lock()->SetPos(pos);
				m_skybox.lock()->SetSize(size);
				m_skybox.lock()->SetAngle(angle);

				ImGui::Text((const char*)u8"------------------------------------------------------------------------------------------------------------------------------------------------------------------");

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		// シーンの切り替え
		if (ImGui::TreeNode((const char*)u8"シーン切替"))
		{
			// タイトル
			if (ImGui::Button("Title"))
			{
				SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
				KdEffekseerManager::GetInstance().StopAllEffect();
			}
			// ゲーム
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

	// 名前
	_json["TitleCamera"]["Name"] = "TitleCamera";
	
	// 座標
	_json["TitleCamera"]["PosX"] = m_titleCamera.lock()->GetPos().x;
	_json["TitleCamera"]["PosY"] = m_titleCamera.lock()->GetPos().y;
	_json["TitleCamera"]["PosZ"] = m_titleCamera.lock()->GetPos().z;
	
	// 角度
	_json["TitleCamera"]["DegAngX"] = m_titleCamera.lock()->GetDegAng().x;
	_json["TitleCamera"]["DegAngY"] = m_titleCamera.lock()->GetDegAng().y;
	_json["TitleCamera"]["DegAngZ"] = m_titleCamera.lock()->GetDegAng().z;

	// 視野角
	_json["TitleCamera"]["ViewingAngle"] = m_titleCamera.lock()->GetViewingAngle();

	// 回転角度変化量
	_json["TitleCamera"]["ChangeAngle"] = m_titleCamera.lock()->GetChangeAngle();

	// ファイルに保存
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

	// 名前
	_json["Title"]["Name"] = "Title";
	
	// 座標
	_json["Title"]["PosX"] = m_title.lock()->GetVector2Pos().x;
	_json["Title"]["PosY"] = m_title.lock()->GetVector2Pos().y;
	
	// 大きさ
	_json["Title"]["Size"] = m_title.lock()->GetSize();

	// ファイルに保存
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

	// 名前
	_json["Game"]["Name"] = "Game";
	
	// 座標
	_json["Game"]["PosX"] = m_game.lock()->GetVector2Pos().x;
	_json["Game"]["PosY"] = m_game.lock()->GetVector2Pos().y;
	
	// 大きさ
	_json["Game"]["Size"] = m_game.lock()->GetSize();

	// ファイルに保存
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

	// 名前
	_json["Exit"]["Name"] = "Exit";
	
	// 座標
	_json["Exit"]["PosX"] = m_exit.lock()->GetVector2Pos().x;
	_json["Exit"]["PosY"] = m_exit.lock()->GetVector2Pos().y;
	
	// 大きさ
	_json["Exit"]["Size"] = m_exit.lock()->GetSize();

	// ファイルに保存
	std::ofstream _file("Asset/Json/Title/Exit/Exit.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::SelectWrite()
{
	if (m_select.expired())return;

	nlohmann::json _json;

	// 名前
	_json["Select"]["Name"] = "Select";

	// サイズ
	_json["Select"]["Size"] = m_select.lock()->GetSize();

	// アルファ値補正値
	_json["Select"]["AlphaCorrection"] = m_select.lock()->GetAlphaCorrection();

	// 角度変化量
	_json["Select"]["ChangeAngle"] = m_select.lock()->GetChangeAngle();

	// ファイルに保存
	std::ofstream _file("Asset/Json/Title/Select/Select.json");
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

	// 名前
	_json["Cursor"]["Name"] = "Cursor";

	// サイズ
	_json["Cursor"]["Size"] = m_cursor.lock()->GetSize();

	// ファイルに保存
	std::ofstream _file("Asset/Json/Title/Cursor/Cursor.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::CursorParticleWrite()
{
	nlohmann::json _json;

	// 最大数
	_json["Particle"]["Num"] = m_cursor.lock()->GetParticleParam().ParticleNum;

	// 移動量X軸のランダムの数
	_json["Particle"]["moveXNum"] = m_cursor.lock()->GetParticleParam().MoveXNum;

	// 移動量Y軸のランダムの数
	_json["Particle"]["moveYNum"] = m_cursor.lock()->GetParticleParam().MoveYNum;

	// 移動量の最低値
	_json["Particle"]["moveMinX"] = m_cursor.lock()->GetParticleParam().MinMove.x;
	_json["Particle"]["moveMinY"] = m_cursor.lock()->GetParticleParam().MinMove.y;

	// 移動量の補正値
	_json["Particle"]["moveCorrectionX"] = m_cursor.lock()->GetParticleParam().MoveCorrection.x;
	_json["Particle"]["moveCorrectionY"] = m_cursor.lock()->GetParticleParam().MoveCorrection.y;

	// サイズのランダムの数
	_json["Particle"]["SizeNum"] = m_cursor.lock()->GetParticleParam().SizeNum;

	// サイズ最低値
	_json["Particle"]["SizeMin"] = m_cursor.lock()->GetParticleParam().MinSize;

	// サイズ補正値
	_json["Particle"]["SizeCorrection"] = m_cursor.lock()->GetParticleParam().SizeCorrection;

	// サイズ変化量
	_json["Particle"]["SizeChange"] = m_cursor.lock()->GetParticleParam().SizeChange;

	// ファイルに保存
	std::ofstream _file("Asset/Json/Title/Cursor/Particle/Particle.json");
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

	// 保存するステート
	std::string _cameraType;
	switch (m_camera.lock()->GetCameraType())
	{
	case GameCamera::CameraType::PlayerType:
		_cameraType = "Player";
		break;
	case GameCamera::CameraType::FixedType:
		_cameraType = "Fixed";
		break;
	case GameCamera::CameraType::BossType:
		_cameraType = "Boss";
		break;
	case GameCamera::CameraType::ClearType:
		_cameraType = "Clear";
		break;
	default:
		break;
	}

	// 名前
	_json["Camera"]["Name"] = "Camera";
	
	// クリア時回転角度変化量
	_json["Camera"]["ChangeClearAngle"] = m_camera.lock()->GetChangeClearAngle();

	// 振動時sinカーブ変化量
	_json["Camera"]["ChangeShakeAngle"] = m_camera.lock()->GetChangeShakeAngle();

	// 振動移動量
	_json["Camera"]["DefaultMove"] = m_camera.lock()->GetDefaultMove();

	// 振動時間
	_json["Camera"]["DefaultShakeTime"] = m_camera.lock()->GetDefaultShakeTime();

	// 座標
	_json["Camera"][_cameraType + "PosX"] = m_camera.lock()->GetNowPos().x;
	_json["Camera"][_cameraType + "PosY"] = m_camera.lock()->GetNowPos().y;
	_json["Camera"][_cameraType + "PosZ"] = m_camera.lock()->GetNowPos().z;
	
	// 角度
	_json["Camera"][_cameraType + "AngleX"] = m_camera.lock()->GetNowDegAng().x;
	_json["Camera"][_cameraType + "AngleY"] = m_camera.lock()->GetNowDegAng().y;
	
	// 視野角
	_json["Camera"][_cameraType + "ViewAngle"] = m_camera.lock()->GetNowViewAng();

	// ファイルに保存
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

	// 名前
	_json["Player"]["Name"] = "Player";
	
	// 座標
	_json["Player"]["PosX"] = m_player.lock()->GetPos().x;
	_json["Player"]["PosY"] = m_player.lock()->GetPos().y;
	_json["Player"]["PosZ"] = m_player.lock()->GetPos().z;
	
	// 大きさ
	_json["Player"]["Size"] = m_player.lock()->GetSize();
	
	// 角度
	_json["Player"]["Angle"] = m_player.lock()->GetAngle().y;
	
	// HP
	_json["Player"]["HP"] = m_player.lock()->GetParam().Hp;
	
	// 攻撃力
	_json["Player"]["ATK"] = m_player.lock()->GetParam().Atk;
	
	// 素早さ
	_json["Player"]["Speed"] = m_player.lock()->GetParam().Sp;
	
	// スタミナ
	_json["Player"]["Stamina"] = m_player.lock()->GetParam().Sm;
	
	// 攻撃範囲
	_json["Player"]["ATKRange"] = m_player.lock()->GetParam().AtkRange;
	
	// 前方方向
	_json["Player"]["ForwardX"] = m_player.lock()->GetForward().x;
	_json["Player"]["ForwardY"] = m_player.lock()->GetForward().y;
	_json["Player"]["ForwardZ"] = m_player.lock()->GetForward().z;
	
	// 無敵付与時間
	_json["Player"]["InviAddTime"] = m_player.lock()->GetinviAddTime();
	
	// カウンターの攻撃範囲
	_json["Player"]["CounterRadius"] = m_player.lock()->GetCounterRadius();

	// スタミナ回復可能時間 
	_json["Player"]["StaminaRecoveryTime"] = m_player.lock()->GetStaminaRecoveryTime();

	// 回避した時のスタミナの減少量
	_json["Player"]["RollStamina"] = m_player.lock()->GetRollStamina();

	// パリィ可能時間
	_json["Player"]["ParryTime"] = m_player.lock()->GetParryTime();

	// 武器
	_json["Player"]["SwordName"] = m_player.lock()->GetSword().lock()->GetName();
	_json["Player"]["ShieldName"] = m_player.lock()->GetShield().lock()->GetName();

	// ファイルに保存
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

	// 名前
	_json["HP"]["Name"] = "HP";
	
	// 座標
	_json["HP"]["PosX"] = m_PlayerHP.lock()->GetVector2Pos().x;
	_json["HP"]["PosY"] = m_PlayerHP.lock()->GetVector2Pos().y;
	
	// 大きさ
	_json["HP"]["Size"] = m_PlayerHP.lock()->GetSize();

	// 減少ゲージ減少開始時間
	_json["HP"]["DownTime"] = m_PlayerHP.lock()->GetDownTime();

	// 減少ゲージ変化量
	_json["HP"]["DownChange"] = m_PlayerHP.lock()->GetDownChange();

	// ファイルに保存
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

	// 名前
	_json["Stamina"]["Name"] = "Stamina";
	
	// 座標
	_json["Stamina"]["PosX"] = m_PlayerStamina.lock()->GetVector2Pos().x;
	_json["Stamina"]["PosY"] = m_PlayerStamina.lock()->GetVector2Pos().y;
	
	// 大きさ
	_json["Stamina"]["Size"] = m_PlayerStamina.lock()->GetSize();

	// ファイルに保存
	std::ofstream _file("Asset/Json/Game/UI/Player/Stamina/Stamina.json");
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

	// 名前
	_json["Floor"]["Name"] = "Floor";
	
	// 座標
	_json["Floor"]["PosX"] = m_floor.lock()->GetVector2Pos().x;
	_json["Floor"]["PosY"] = m_floor.lock()->GetVector2Pos().y;
	
	// 大きさ
	_json["Floor"]["Size"] = m_floor.lock()->GetSize();

	// ファイルに保存
	std::ofstream _file("Asset/Json/Game/UI/Player/Floor/Floor.json");
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::GameStateWrite()
{
	if (m_gameStateUI.expired())return;

	nlohmann::json _json;

	// 名前
	_json["GameState"]["Name"] = "GameStateUI";
	
	// 座標
	_json["GameState"]["PosX"] = m_gameStateUI.lock()->GetVector2Pos().x;
	_json["GameState"]["PosY"] = m_gameStateUI.lock()->GetVector2Pos().y;
	
	// 大きさ
	_json["GameState"]["Size"] = m_gameStateUI.lock()->GetSize();
	
	// アルファ値変化量
	_json["GameState"]["AlphaChange"] = m_gameStateUI.lock()->GetAlphaChange();

	// ファイルに保存
	std::ofstream _file("Asset/Json/Game/UI/Player/GameState/GameState.json");
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
	// 前の状態
	std::ifstream _oldFile(_stagePath);
	if (_oldFile.is_open())
	{
		_oldFile >> _json;
		_oldFile.close();
	}

	// 変更するウェーブ
	std::string wave = (std::to_string(_wave).c_str()) + ((std::string)"Wave");
	_json[wave].clear();

	for (auto& enemy :SceneManager::Instance().GetEnemyList())
	{
		std::string _category; // 種類
		std::string _num;      // 番号

		// 骨
		if (enemy->GetName() == "Bone")
		{
			static int b = 0;
			_category = "Bone";
			_num = (std::to_string(b).c_str()) + ((std::string)"Bone");
			b++;
		}
		// 骨色違い
		else if (enemy->GetName() == "BoneAlpha")
		{
			static int b = 0;
			_category = "BoneAlpha";
			_num = (std::to_string(b).c_str()) + ((std::string)"BoneAlpha");
			b++;
		}
		// ゴーレム
		else if (enemy->GetName() == "Golem")
		{
			static int g = 0;
			_category = "Golem";
			_num = (std::to_string(g).c_str()) + ((std::string)"Golem");
			
			// 弾攻撃の射程角度
			_json[wave][_category][_num]["Attack1Angle"] = m_golem.lock()->GetAttack1Angle();
			
			// ジャンプ攻撃の最短距離
			_json[wave][_category][_num]["Attack2Dist"] = m_golem.lock()->GetAttack2Dist();
			
			g++;
		}

		// 名前
		_json[wave][_category][_num]["Name"] = enemy->GetName();

		// 座標　
		_json[wave][_category][_num]["PosX"] = enemy->GetPos().x;
		_json[wave][_category][_num]["PosY"] = enemy->GetPos().y;
		_json[wave][_category][_num]["PosZ"] = enemy->GetPos().z;
		
		// 大きさ
		_json[wave][_category][_num]["Size"] = enemy->GetSize();
		
		// 角度
		_json[wave][_category][_num]["Angle"] = enemy->GetAngle().y;
		
		// HP
		_json[wave][_category][_num]["HP"] = enemy->GetParam().Hp;
		
		// 攻撃力
		_json[wave][_category][_num]["ATK"] = enemy->GetParam().Atk;
		
		// 素早さ
		_json[wave][_category][_num]["Speed"] = enemy->GetParam().Sp;
		
		// スタミナ
		_json[wave][_category][_num]["Stamina"] = enemy->GetParam().Sm;
		
		// 出現演出時のエフェクトサイズ
		_json[wave][_category][_num]["AppealEffectSize"] = enemy->GetAppealEffectSize();

		// 攻撃範囲
		_json[wave][_category][_num]["ATKRange"] = enemy->GetParam().AtkRange;
		
		// 他の敵との距離判定時のスフィアの半径
		_json[wave][_category][_num]["EnemyCheckRadius"] = enemy->GetEnemyCheckRadius();

		// 避ける距離
		_json[wave][_category][_num]["LeaveDist"] = enemy->GetLeaveDist();

		// 攻撃判定のスフィアサイズ
		_json[wave][_category][_num]["AttackSphereSize"] = enemy->GetAttackSphereSize();
		
		// よろける時の移動スピード
		_json[wave][_category][_num]["StumbleMove"] = enemy->GetStumbleMove();

		// ボスフラグ
		_json[wave][_category][_num]["Boss"] = enemy->GetBossFlg();

		// 前方方向
		_json[wave][_category][_num]["ForwardX"] = enemy->GetForward().x;
		_json[wave][_category][_num]["ForwardY"] = enemy->GetForward().y;
		_json[wave][_category][_num]["ForwardZ"] = enemy->GetForward().z;
	}

	// ファイルに保存
	std::ofstream _newFile(_stagePath);
	if (_newFile.is_open())
	{
		_newFile << _json.dump();
		_newFile.close();
	}
}

void ObjectManager::EnemyHPWrite()
{
	if (m_EnemyHPList[EnemyHPType::Normal].size() == 0 && m_EnemyHPList[EnemyHPType::Boss].size() == 0)return;

	nlohmann::json _json;
	// 前の状態のファイル
	std::ifstream _oldFile("Asset/Json/Game/UI/Enemy/HP/HP.json");
	if (_oldFile.is_open())
	{
		_oldFile >> _json;
		_oldFile.close();
	}

	// 名前
	_json["HP"]["Name"] = "HP";

	// 通常==============================================================================
	if (m_EnemyHPList[EnemyHPType::Normal].size() > 0)
	{
		// 座標
		_json["HP"]["NormalPosX"] = m_EnemyHPList[EnemyHPType::Normal][0].lock()->GetVector2Pos().x;
		_json["HP"]["NormalPosY"] = m_EnemyHPList[EnemyHPType::Normal][0].lock()->GetVector2Pos().y;

		// 座標補正
		_json["HP"]["Correction"] = m_EnemyHPList[EnemyHPType::Normal][0].lock()->GetPosXCorrection();

		// 大きさ
		_json["HP"]["NormalSize"] = m_EnemyHPList[EnemyHPType::Normal][0].lock()->GetSize();

		// 減少ゲージ減少開始時間
		_json["HP"]["DownTime"] = m_EnemyHPList[EnemyHPType::Normal][0].lock()->GetDownTime();

		// 減少ゲージ変化量
		_json["HP"]["DownChange"] = m_EnemyHPList[EnemyHPType::Normal][0].lock()->GetDownChange();
	}
	//===================================================================================

	// ボス==============================================================================
	if (m_EnemyHPList[EnemyHPType::Boss].size() > 0)
	{
		// 座標
		_json["HP"]["BossPosX"] = m_EnemyHPList[EnemyHPType::Boss][0].lock()->GetVector2Pos().x;
		_json["HP"]["BossPosY"] = m_EnemyHPList[EnemyHPType::Boss][0].lock()->GetVector2Pos().y;

		// 大きさ
		_json["HP"]["BossSize"] = m_EnemyHPList[EnemyHPType::Boss][0].lock()->GetSize();

		// 減少ゲージ減少開始時間
		_json["HP"]["DownTime"] = m_EnemyHPList[EnemyHPType::Boss][0].lock()->GetDownTime();

		// 減少ゲージ変化量
		_json["HP"]["DownChange"] = m_EnemyHPList[EnemyHPType::Boss][0].lock()->GetDownChange();
	}
	//===================================================================================

	// ファイルに保存
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
	// 前の状態のファイル
	std::ifstream _oldFile(_fileName);
	if (_oldFile.is_open())
	{
		_oldFile >> _json;
		_oldFile.close();
	}

	for (auto& _sword : _json)
	{
		if (_sword["Name"] != _swordName)continue;

		// 座標
		_sword["PosX"] = m_player.lock()->GetSword().lock()->GetPos().x;
		_sword["PosY"] = m_player.lock()->GetSword().lock()->GetPos().y;
		_sword["PosZ"] = m_player.lock()->GetSword().lock()->GetPos().z;
		
		// 攻撃力
		_sword["ATK"] = m_player.lock()->GetSword().lock()->GetATK();
		
		// 大きさ
		_sword["Size"] = m_player.lock()->GetSword().lock()->GetSize();
		
		// 攻撃判定のスフィアサイズ
		_sword["AttackSphereSize"] = m_player.lock()->GetSword().lock()->GetAttackSphereSize();

		// 軌跡の長さ
		_sword["Traject"] = m_player.lock()->GetSword().lock()->GetTraject();
	}

	// ファイルに保存
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
	// 前の状態のファイル
	std::ifstream _oldFile(_fileName);
	if (_oldFile.is_open())
	{
		_oldFile >> _json;
		_oldFile.close();
	}

	for (auto& _shield : _json)
	{
		if (_shield["Name"] != _shieldName)continue;

		// 座標
		_shield["PosX"] = m_player.lock()->GetShield().lock()->GetPos().x;
		_shield["PosY"] = m_player.lock()->GetShield().lock()->GetPos().y;
		_shield["PosZ"] = m_player.lock()->GetShield().lock()->GetPos().z;
		
		// 大きさ
		_shield["Size"] = m_player.lock()->GetShield().lock()->GetSize();
	}

	// ファイルに保存
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

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		std::string _category; // 種類
		std::string _num;      // 番号
		if (obj->GetName() == "Ground") // 地面
		{
			static int g = 0;
			_category = "Ground";
			_num = (std::to_string(g).c_str()) + ((std::string)"Ground");
			g++;
		}
		else if (obj->GetName() == "Circle") // 魔法陣の台
		{
			static int c = 0;
			_category = "Circle";
			_num = (std::to_string(c).c_str()) + ((std::string)"Circle");
			c++;
		}
		else if (obj->GetName() == "Magic") // 魔法陣
		{
			static int m = 0;
			_category = "Magic";
			_num = (std::to_string(m).c_str()) + ((std::string)"Magic");

			// RGB変化量
			_json[_category][_num]["RGBChange"] = m_magic.lock()->GetRGBChange();

			// 角度変化量
			_json[_category][_num]["ChangeAngle"] = m_magic.lock()->GetChangeAngle();

			m++;
		}
		else if (obj->GetName() == "Wall") // 壁
		{
			static int w = 0;
			_category = "Wall";
			_num = (std::to_string(w).c_str()) + ((std::string)"Wall");
			w++;
		}
		else if (obj->GetName() == "SkyBox") // 空
		{
			static int s = 0;
			_category = "SkyBox";
			_num = (std::to_string(s).c_str()) + ((std::string)"SkyBox");
			s++;
		}

		// 名前
		_json[_category][_num]["Name"] = obj->GetName();
		
		// 座標
		_json[_category][_num]["PosX"] = obj->GetPos().x;
		_json[_category][_num]["PosY"] = obj->GetPos().y;
		_json[_category][_num]["PosZ"] = obj->GetPos().z;
		
		// 大きさ
		_json[_category][_num]["Size"] = obj->GetSize();
		
		// 角度
		_json[_category][_num]["Angle"] = obj->GetAngle().y;
	}

	// ファイルに保存
	std::ofstream _file(_fileName);
	if (_file.is_open())
	{
		_file << _json.dump();
		_file.close();
	}
}

void ObjectManager::ModelLoad()
{
	// 骨
	std::shared_ptr<KdModelWork>_boneModel = std::make_shared<KdModelWork>();
	_boneModel->SetModelData("Asset/Models/Character/Enemy/Bone/Bone.gltf");

	// 骨色違い
	std::shared_ptr<KdModelWork>_boneAlphaModel = std::make_shared<KdModelWork>();
	_boneAlphaModel->SetModelData("Asset/Models/Character/Enemy/BoneAlpha/BoneAlpha.gltf");
	// 弾
	std::shared_ptr<KdModelWork>_boneAlphaBulletModel = std::make_shared<KdModelWork>();
	_boneAlphaBulletModel->SetEnable("Asset/Models/Character/Enemy/BoneAlpha/Bullet/Bullet.gltf");
	
	// ゴーレム
	std::shared_ptr<KdModelWork>_golemModel = std::make_shared<KdModelWork>();
	_golemModel->SetEnable("Asset/Models/Character/Enemy/Golem/golem.gltf");
	// 弾
	std::shared_ptr<KdModelWork>_golemBulletModel = std::make_shared<KdModelWork>();
	_golemBulletModel->SetModelData("Asset/Models/Character/Enemy/Golem/Bullet/Bullet.gltf");
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

	for (auto& obj : _json)
	{
		// 名前
		std::string _name;
		_name = obj["Name"];

		// 座標
		Math::Vector3 _pos = Math::Vector3::Zero;
		_pos.x = obj["PosX"];
		_pos.y = obj["PosY"];
		_pos.z = obj["PosZ"];

		// 角度
		Math::Vector3 _DegAng = Math::Vector3::Zero;
		_DegAng.x = obj["DegAngX"];
		_DegAng.y = obj["DegAngY"];
		_DegAng.z = obj["DegAngZ"];

		float _ViewingAngle = 0.0f;
		_ViewingAngle = obj["ViewingAngle"];

		// 回転角度変化量
		float _ChangeAngle = 0.0f;
		_ChangeAngle = obj["ChangeAngle"];

		// セット
		std::shared_ptr<TitleCamera> camera = std::make_shared<TitleCamera>();
		camera->SetName(_name);
		camera->SetPos(_pos);
		camera->SetDegAng(_DegAng);
		camera->SetViewingAngle(_ViewingAngle);
		camera->SetChangeAngle(_ChangeAngle);
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

	for (auto& obj : _json)
	{
		// 座標
		Math::Vector2 _pos = Math::Vector2::Zero;
		_pos.x = obj["PosX"];
		_pos.y = obj["PosY"];

		// 大きさ
		float _size = 1.0f;
		_size = obj["Size"];

		// 名前
		std::string _name;
		_name = obj["Name"];
		
		// セット
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

	for (auto& obj : _json)
	{
		// 座標
		Math::Vector2 _pos = Math::Vector2::Zero;
		_pos.x = obj["PosX"];
		_pos.y = obj["PosY"];

		// 大きさ
		float _size = 1.0f;
		_size = obj["Size"];

		// 名前
		std::string _name;
		_name = obj["Name"];

		// セット
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

	for (auto& obj : _json)
	{
		// 座標
		Math::Vector2 _pos = Math::Vector2::Zero;
		_pos.x = obj["PosX"];
		_pos.y = obj["PosY"];

		// 大きさ
		float _size = 1.0f;
		_size = obj["Size"];

		// 名前
		std::string _name;
		_name = obj["Name"];

		// セット
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

void ObjectManager::SetSelectParam()
{
	//jsonファイル
	std::string fileName = "Asset/Json/Title/Select/Select.json";

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& obj : _json)
	{
		// サイズ
		float _size = 1.0f;
		_size = obj["Size"];

		// アルファ値補正値
		float _alphaCorrection = 1.0f;
		_alphaCorrection = obj["AlphaCorrection"];

		// 角度変化量
		float _changeAngle = 1.0f;
		_changeAngle = obj["ChangeAngle"];

		// 名前
		std::string _name;
		_name = obj["Name"];

		// セット
		std::shared_ptr<Select> select = std::make_shared<Select>();
		select->SetSize(_size);
		select->SetAlphaCorrection(_alphaCorrection);
		select->SetChangeAngle(_changeAngle);
		select->SetName(_name);
		select->SetID(m_id);
		select->SetObjManager(shared_from_this());
		select->Init();
		m_id++;

		m_select = select;
		SceneManager::Instance().AddUI(select);
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

	for (auto& obj : _json)
	{
		// サイズ
		float _size = 1.0f;
		_size = obj["Size"];

		// 名前
		std::string _name;
		_name = obj["Name"];

		// セット
		std::shared_ptr<Cursor> cursor = std::make_shared<Cursor>();

		cursor->SetSize(_size);
		cursor->SetName(_name);
		cursor->SetID(m_id);
		cursor->SetObjManager(shared_from_this());
		cursor->Init();
		m_id++;

		m_cursor = cursor;
		SceneManager::Instance().AddUI(cursor);

		SetCursorParticleParam();
	}

	ifs.close();
}

void ObjectManager::SetCursorParticleParam()
{
	//jsonファイル
	std::string fileName = "Asset/Json/Title/Cursor/Particle/Particle.json";

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& obj : _json)
	{
		// 最大数
		int _num = 0;
		_num = obj["Num"];

		// 移動量X軸のランダムの数
		int _XNum = 0;
		_XNum = obj["moveXNum"];

		// 移動量Y軸のランダムの数
		int _YNum = 0;
		_YNum = obj["moveYNum"];

		// 移動量の最低値
		Math::Vector2 _moveMin = Math::Vector2::Zero;
		_moveMin = { obj["moveMinX"],obj["moveMinY"] };

		// 移動量の補正値
		Math::Vector2 _moveCorrection = Math::Vector2::Zero;
		_moveCorrection = { obj["moveCorrectionX"],obj["moveCorrectionY"] };

		// サイズのランダムの数
		int _SizeNum = 0;
		_SizeNum = obj["SizeNum"];

		// サイズ最低値
		float _sizeMin = 0.0f;
		_sizeMin = obj["SizeMin"];

		// サイズ補正値
		float _sizeCorrection = 0.0f;
		_sizeCorrection = obj["SizeCorrection"];

		// サイズ変化量
		float _sizeChange = 0.0f;
		_sizeChange = obj["SizeChange"];


		// セット
		m_cursor.lock()->SetParticleParam(_num, _XNum, _YNum, _moveMin, _moveCorrection, _SizeNum, _sizeMin, _sizeCorrection, _sizeChange);
	}

	ifs.close();
}

void ObjectManager::SetGameCameraParam(std::shared_ptr<StageManager> _stage)
{
	//jsonファイル
	std::string fileName = "Asset/Json/Game/Camera/Camera.json";

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& obj : _json)
	{
		// 名前
		std::string _name;
		_name = obj["Name"];

		// クリア時回転角度変化量
		float _ChangeClearAngle = 0.0f;
		_ChangeClearAngle = obj["ChangeClearAngle"];

		// 振動時sinカーブ変化量
		float _shakeAngle = 0.0f;
		_shakeAngle = obj[ "ChangeShakeAngle" ];

		// 振動移動量
		float _move = 0.0f;
		_move = obj["DefaultMove"];

		// 振動時間
		int _shakeTime = 0;
		_shakeTime = obj["DefaultShakeTime"];

		// プレイヤーステート
		// 座標
		Math::Vector3 _PlayerPos = Math::Vector3::Zero;
		_PlayerPos.x = obj["PlayerPosX"];
		_PlayerPos.y = obj["PlayerPosY"];
		_PlayerPos.z = obj["PlayerPosZ"];

		// 角度
		Math::Vector3 _PlayerAngle = Math::Vector3::Zero;
		_PlayerAngle.x = obj["PlayerAngleX"];
		_PlayerAngle.y = obj["PlayerAngleY"];

		// 視野角
		float _PlayerViewAngle = 0.0f;
		_PlayerViewAngle = obj["PlayerViewAngle"];


		// テレポート開放ステート
		// 座標
		Math::Vector3 _FixedPos = Math::Vector3::Zero;
		_FixedPos.x = obj["FixedPosX"];
		_FixedPos.y = obj["FixedPosY"];
		_FixedPos.z = obj["FixedPosZ"];

		// 角度
		Math::Vector3 _FixedAngle = Math::Vector3::Zero;
		_FixedAngle.x = obj["FixedAngleX"];
		_FixedAngle.y = obj["FixedAngleY"];

		// 視野角
		float _FixedViewAngle = 0.0f;
		_FixedViewAngle = obj["FixedViewAngle"];

		// ボス登場ステート
		// 座標
		Math::Vector3 _BossPos = Math::Vector3::Zero;
		_BossPos.x = obj["BossPosX"];
		_BossPos.y = obj["BossPosY"];
		_BossPos.z = obj["BossPosZ"];

		// 角度
		Math::Vector3 _BossAngle = Math::Vector3::Zero;
		_BossAngle.x = obj["BossAngleX"];
		_BossAngle.y = obj["BossAngleY"];

		// 視野角
		float _BossViewAngle = 0.0f;
		_BossViewAngle = obj["BossViewAngle"];


		// クリアステート
		// 座標
		Math::Vector3 _ClearPos = Math::Vector3::Zero;
		_ClearPos.x = obj["ClearPosX"];
		_ClearPos.y = obj["ClearPosY"];
		_ClearPos.z = obj["ClearPosZ"];

		// 角度
		Math::Vector3 _ClearAngle = Math::Vector3::Zero;
		_ClearAngle.x = obj["ClearAngleX"];
		_ClearAngle.y = obj["ClearAngleY"];

		// 視野角
		float _ClearViewAngle = 0.0f;
		_ClearViewAngle = obj["ClearViewAngle"];


		// セット
		std::shared_ptr<GameCamera> camera = std::make_shared<GameCamera>();
		camera->SetPosList(_PlayerPos, _FixedPos, _BossPos, _ClearPos);
		camera->SetChangeClearAngle(_ChangeClearAngle);
		camera->SetChangeShakeAngle(_shakeAngle);
		camera->SetDefaultShakeMove(_move);
		camera->SetDefaultShakeTime(_shakeTime);
		camera->SetDegAngList(_PlayerAngle, _FixedAngle, _BossAngle, _ClearAngle);
		camera->SetViewAngList(_PlayerViewAngle, _FixedViewAngle, _BossViewAngle, _ClearViewAngle);
		camera->SetName(_name);
		camera->SetObjManager(shared_from_this());
		camera->SetStageManager(_stage);
		camera->SetID(m_id);
		camera->Init();
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

	std::shared_ptr<Ground> ground;      // 地面
	std::shared_ptr<Circle> circle;      // 魔法陣の台
	std::shared_ptr<MagicPolygon> magic; // 魔法陣
	std::shared_ptr<Wall> wall;          // 壁
	std::shared_ptr<SkyBox> skybox;      // 空

	for (auto& category : _json)
	{

		// 種類ごとに読み込んでいく

		for (auto& Object : category)
		{
			// 座標
			Math::Vector3 _pos = Math::Vector3::Zero;
			_pos.x = Object["PosX"];
			_pos.y = Object["PosY"];
			_pos.z = Object["PosZ"];

			// 大きさ
			float _size = 0.0f;
			_size = Object["Size"];

			// 角度
			Math::Vector3 _angle = Math::Vector3::Zero;
			_angle.y = Object["Angle"];

			// 名前
			std::string _name;
			_name = Object["Name"];

			// セット
			std::shared_ptr<KdGameObject> obj;
			if (_name == "Ground") // 地面
			{
				if (!m_ground.expired())break;

				ground = std::make_shared<Ground>();
				m_ground = ground;
				obj = ground;
			}
			if (_name == "Circle") // 魔法陣の台
			{
				circle = std::make_shared<Circle>();
				m_circle = circle;
				obj = circle;
			}
			if (_name == "Magic") // 魔法陣
			{
				// RGB変化量
				float rgb = 0.0f;
				rgb = Object["RGBChange"];

				// 角度変化量
				float angle = 0.0f;
				angle = Object["ChangeAngle"];

				magic = std::make_shared<MagicPolygon>();
				magic->SetStageManager(_stage);
				magic->SetRGBChange(rgb);
				magic->SetChangeAngle(angle);
				m_magic = magic;
				if (m_camera.expired() == false)m_camera.lock()->SetFixedTarget(magic);
				obj = magic;
			}
			if (_name == "Wall") // 壁
			{
				wall = std::make_shared<Wall>();
				m_wall = wall;
				obj = wall;
			}
			if (_name == "SkyBox") // 空
			{
				skybox = std::make_shared<SkyBox>();
				m_skybox = skybox;
				obj = skybox;
			}
			obj->SetPos(_pos);
			obj->SetSize(_size);
			obj->SetAngle(_angle);
			obj->SetName(_name);
			obj->SetID(m_id);
			obj->Init();
			m_id++;

			SceneManager::Instance().AddObject(obj);
		}
	}

	// ステージ数測定
	int _maxStage = 0;
	for (auto& stage : _json["Ground"]) // 登録されている地面の数 ＝ ステージ数
	{
		_maxStage++;
	}

	if (m_nowScene == "Game")_stage->SetMaxStage(_maxStage); // ステージマネジャにステージ数をセット

	ifs.close();

	if (circle && ground)circle->SetGround(ground); // 地面と同じ位置にするためセット
	if (magic && circle) magic->SetCircle(circle);  // 台と同じ位置にするためにセット
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

	for (auto& obj : _json)
	{
		// 名前
		std::string _name;
		_name = obj["Name"];

		// 座標
		Math::Vector3 _pos = Math::Vector3::Zero;
		_pos = m_circle.lock()->GetPos();

		// 大きさ
		float _size = 0.0f;
		_size = obj["Size"];

		// 角度
		Math::Vector3 _angle = Math::Vector3::Zero;
		_angle.y = obj["Angle"];

		// HP
		int _hp = 0;
		_hp = obj["HP"];

		// 素早さ
		float _speed = 0.0f;
		_speed = obj["Speed"];

		// スタミナ
		int _stamina = 0;
		_stamina = obj["Stamina"];

		// 攻撃範囲
		float _atkRange = 0.0f;
		_atkRange = obj["ATKRange"];

		// 前方方向
		Math::Vector3 _forward = Math::Vector3::Zero;
		_forward.x = obj["ForwardX"];
		_forward.y = obj["ForwardY"];
		_forward.z = obj["ForwardZ"];

		// 無敵付与時間
		int _inviTime = 0;
		_inviTime = obj["InviAddTime"];

		// カウンターの攻撃範囲
		float _counterRadius = 0.0f;
		_counterRadius = obj["CounterRadius"];

		// スタミナ回復可能時間
		int _staminaRecoveryTime = 0;
		_staminaRecoveryTime = obj["StaminaRecoveryTime"];

		// 回避したときのスタミナ減少量
		int _rollStamina = 0;
		_rollStamina = obj["RollStamina"];

		// パリィ可能時間
		int _parryTime = 0;
		_parryTime = obj["ParryTime"];

		std::shared_ptr<Player> player = std::make_shared<Player>();
		m_player = player;

		// 武器
		SetWeaponParam((("Asset/Json/") + m_nowScene + ("/Weapon/Sword/Sword.json")), obj["SwordName"]);    // 剣
		SetWeaponParam((("Asset/Json/") + m_nowScene + ("/Weapon/Shield/Shield.json")), obj["ShieldName"]); // 盾

		// セット
		player->SetCamera(m_camera.lock());
		player->SetParam(_hp, player->GetSword().lock()->GetATK(), _speed, _stamina);
		player->SetPos(_pos);
		player->SetSize(_size);
		player->SetAngle(_angle);
		player->SetAtkRange(_atkRange);
		player->SetForward(_forward);
		player->SetObjManager(shared_from_this());
		player->SetStageManager(_stage);
		player->Init();
		player->SetInviAddTime(_inviTime);
		player->SetCounterRadius(_counterRadius);
		player->SetStaminaRecoveryTime(_staminaRecoveryTime);
		player->SetRollStamina(_rollStamina);
		player->SetParryTime(_parryTime);
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
	// HP
	SetPlayerHPParam();
	
	// スタミナ
	SetPlayerStaminaParam();
	
	// ステージ数
	SetFloorParam(_stage);
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

	for (auto& obj : _json)
	{
		// 座標
		Math::Vector2 _pos = Math::Vector2::Zero;
		_pos.x = obj["PosX"];
		_pos.y = obj["PosY"];

		// 大きさ
		float _size = 1.0f;
		_size = obj["Size"];

		// 減少ゲージ減少開始時間
		int _DownTime = 1;
		_DownTime = obj["DownTime"];

		// 減少ゲージ変化量
		float _DownChange = 0.0f;
		_DownChange = obj["DownChange"];

		// 名前
		std::string _name;
		_name = obj["Name"];

		// セット
		std::shared_ptr<Player_HP> hp = std::make_shared<Player_HP>();
		hp->SetPos(_pos);
		hp->SetSize(_size);
		hp->SetDownTime(_DownTime);
		hp->SetDownChange(_DownChange);
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

	for (auto& obj : _json)
	{
		// 座標
		Math::Vector2 _pos = Math::Vector2::Zero;
		_pos.x = obj["PosX"];
		_pos.y = obj["PosY"];

		// 大きさ
		float _size = 1.0f;
		_size = obj["Size"];

		// 名前
		std::string _name;
		_name = obj["Name"];

		// セット
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

	for (auto& obj : _json)
	{
		// 座標
		Math::Vector2 _pos = Math::Vector2::Zero;
		_pos.x = obj["PosX"];
		_pos.y = obj["PosY"];

		// 大きさ
		float _size = 1.0f;
		_size = obj["Size"];

		// 名前
		std::string _name;
		_name = obj["Name"];

		// セット
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

void ObjectManager::SetGameStateParam(bool _IsClear)
{
	if (!m_gameStateUI.expired())return;

	//jsonファイル
	std::string fileName = "Asset/Json/Game/UI/Player/GameStateUI/GameStateUI.json";

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& obj : _json)
	{
		// 座標
		Math::Vector2 _pos = Math::Vector2::Zero;
		_pos.x = obj["PosX"];
		_pos.y = obj["PosY"];

		// 大きさ
		float _size = 1.0f;
		_size = obj["Size"];

		// アルファ値変化量
		float _alphaChange = 0.0f;
		_alphaChange = obj["AlphaChange"];

		// 名前
		std::string _name;
		_name = obj["Name"];

		// セット
		std::shared_ptr<GameStateUI> state = std::make_shared<GameStateUI>();
		state->SetState(_IsClear);
		state->SetPos(_pos);
		state->SetSize(_size);
		state->SetChangeAlpha(_alphaChange);
		state->SetName(_name);
		state->SetID(m_id);
		m_id++;

		m_gameStateUI = state;
		SceneManager::Instance().AddPlayerUI(state);
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

	for (auto& obj : _json)
	{
		// リストに名前を格納
		if (obj["ObjectName"] == "Sword") // 剣
		{
			m_swordNameList.push_back(obj["Name"]);
		}
		else if (obj["ObjectName"] == "Shield") // 盾
		{
			m_shieldNameList.push_back(obj["Name"]);
		}

		if (obj["Name"] != _weaponName)continue;

		// 座標
		Math::Vector3 _pos = Math::Vector3::Zero;
		_pos.x = obj["PosX"];
		_pos.y = obj["PosY"];
		_pos.z = obj["PosZ"];

		// 大きさ
		float _size = 0.0f;
		_size = obj["Size"];

		// 角度
		Math::Vector3 _angle = Math::Vector3::Zero;
		_angle.y = obj["Angle"];

		// 名前
		std::string _name;
		_name = obj["Name"];

		// セット
		std::shared_ptr<WeaponBase> weapon = nullptr;
		if (obj["ObjectName"] == "Sword")
		{
			std::shared_ptr<Sword> sword = std::make_shared<Sword>();
			sword->SetATK(obj["ATK"]);
			sword->SetAttackSphereSize(obj["AttackSphereSize"]);
			sword->SetTrajectPointNUM(obj["Traject"]);
			m_player.lock()->SetSword(sword);
			weapon = sword;
		}
		else if (obj["ObjectName"] == "Shield")
		{
			std::shared_ptr<Shield> shield = std::make_shared<Shield>();
			m_player.lock()->SetShield(shield);
			weapon = shield;
		}

		std::string _modelPath;
		_modelPath = obj["Path"];

		weapon->SetModelPath(_modelPath);
		weapon->SetPos(_pos);
		weapon->SetSize(_size);
		weapon->SetAngle(_angle);
		weapon->SetTarget(m_player.lock());
		weapon->SetName(_name);
		weapon->SetID(m_id);
		weapon->Init();
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

	// ファイル読み込み
	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
		ifs.close();
	}

	int _MaxWave = 0; // 最大ウェーブ数
	int _nowWave = 1; // 現在のウェーブ数

	for (auto& wave : _json)
	{
		// 最大ウェーブ加算
		_MaxWave++;
		// 現在のウェーブまで飛ばす
		if (_nowWave != _stage->GetnowWave())
		{
			_nowWave++; 
			continue;
		}

		if (SceneManager::Instance().GetEnemyList().size() == 0)
		{
			// 種類
			for (auto& category : wave)
			{
				// 個体
				for (auto& obj : category)
				{
					// 名前
					std::string _name;
					_name = obj["Name"];

					// 座標
					Math::Vector3 _pos = Math::Vector3::Zero;
					_pos.x = obj["PosX"];
					_pos.y = m_ground.lock()->GetPos().y;
					_pos.z = obj["PosZ"];

					// 大きさ
					float _size = 0.0f;
					_size = obj["Size"];

					// 角度
					Math::Vector3 _angle = Math::Vector3::Zero;
					_angle.y = obj["Angle"];

					// HP
					int _hp = 0;
					_hp = obj["HP"];

					// 攻撃力
					int _atk = 0;
					_atk = obj["ATK"];

					// 素早さ
					float _speed = 0.0f;
					_speed = obj["Speed"];

					// スタミナ
					int _stamina = 0;
					_stamina = obj["Stamina"];

					// 出現演出時のエフェクトサイズ
					float _appealSize = 0.0f;
					_appealSize = obj["AppealEffectSize"];

					// 攻撃範囲
					float _atkRange = 0.0f;
					_atkRange = obj["ATKRange"];

					// 他の敵との距離判定時のスフィアの半径
					float _radius = 0.0f;
					_radius = obj["EnemyCheckRadius"];

					// 避ける時の距離
					float _leaveDist = 0.0f;
					_leaveDist = obj["LeaveDist"];

					// 攻撃判定のスフィアサイズ
					float _atkSphereSize = 0.0f;
					_atkSphereSize = obj["AttackSphereSize"];

					// よろける時の移動スピード
					float _stumbleMove = 0.0f;
					_stumbleMove = obj["StumbleMove"];

					// ボスフラグ
					bool _boss = false;
					_boss = obj["Boss"];

					// 前方方向
					Math::Vector3 _forward = Math::Vector3::Zero;
					_forward.x = obj["ForwardX"];
					_forward.y = obj["ForwardY"];
					_forward.z = obj["ForwardZ"];

					std::shared_ptr<EnemyBase> enemy = nullptr;
					if (obj["Name"] == "Bone")
					{
						std::shared_ptr<Bone> bone = std::make_shared<Bone>();
						enemy = bone;
					}
					else if (obj["Name"] == "BoneAlpha")
					{
						std::shared_ptr<BoneAlpha> alpha = std::make_shared<BoneAlpha>();
						m_BoneAlphaList.push_back(alpha);
						enemy = alpha;
					}
					else if (obj["Name"] == "Golem")
					{
						std::shared_ptr<Golem> golem = std::make_shared<Golem>();
						m_golem = golem;

						// 弾攻撃の射程角度
						float _Attack1Ang = 0.0f;
						_Attack1Ang = obj["Attack1Angle"];
						
						// ジャンプ攻撃の最短距離
						float _Attack2Dist = 0.0f;
						_Attack2Dist = obj["Attack2Dist"];

						golem->SetAttack1Angle(_Attack1Ang);
						golem->SetAttack2Dist(_Attack2Dist);

						enemy = golem;
					}

					// セット
					enemy->SetCamera(m_camera.lock());
					enemy->SetParam(_hp, _atk, _speed, _stamina);
					enemy->SetPos(_pos);
					enemy->SetSize(_size);
					enemy->SetAngle(_angle);
					enemy->SetAppealEffectSize(_appealSize);
					enemy->SetAtkRange(_atkRange);
					enemy->SetEnemyCheckRadius(_radius);
					enemy->SetLeaveDist(_leaveDist);
					enemy->SetAttackSphereSize(_atkSphereSize);
					enemy->SetStumbleMove(_stumbleMove);
					enemy->SetBossFlg(_boss);
					enemy->SetForward(_forward);
					enemy->SetTarget(m_player.lock());
					enemy->SetName(_name);
					enemy->SetID(m_id);
					enemy->SetObjManager(shared_from_this());
					enemy->Init();
					m_id++;

					SceneManager::Instance().AddEnemy(enemy);

					// ボスセット
					if (_boss)m_camera.lock()->SetBossTarget(enemy);

					// HPUI
					SetEnemyHPParam(enemy);
				}
			}
		}
	}

	if (_stage->GetMaxWave() == 0)_stage->SetMaxWave(_MaxWave); // 最大ウェーブが決まっていなかったら更新
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

	std::vector<std::weak_ptr<Enemy_HP>> _normal;
	std::vector<std::weak_ptr<Enemy_HP>> _boss;

	for (auto& obj : _json)
	{
		Math::Vector2 _pos = Math::Vector2::Zero;
		float _posCorrection = 0.0f;
		float _size = 1.0f;
		if (_enemy->GetBossFlg())
		{
			// 座標
			_pos.x = obj["BossPosX"];
			_pos.y = obj["BossPosY"];

			// 大きさ
			_size = obj["BossSize"];
		}
		else
		{
			// 座標
			_pos.x = obj["NormalPosX"];
			_pos.y = obj["NormalPosY"];

			// 大きさ
			_size = obj["NormalSize"];

			// 位置補正
			_posCorrection = obj["Correction"];
		}

		// 減少ゲージ減少開始時間
		int _DownTime = 1;
		_DownTime = obj["DownTime"];

		// 減少ゲージ変化量
		float _DownChange = 0.0f;
		_DownChange = obj["DownChange"];

		// 名前
		std::string _name;
		_name = obj["Name"];

		// セット
		std::shared_ptr<Enemy_HP> hp = std::make_shared<Enemy_HP>();
		hp->SetPos(_pos);
		hp->SetPosXCorrection(_posCorrection);
		hp->SetSize(_size);
		hp->SetDownTime(_DownTime);
		hp->SetDownChange(_DownChange);
		hp->SetName(_name);
		hp->SetCamera(m_camera.lock());
		hp->SetTarget(_enemy);
		hp->SetID(m_id);
		hp->Init();
		m_id++;

		if (_enemy->GetBossFlg())
		{
			_boss.push_back(hp);
		}
		else
		{
			_normal.push_back(hp);
		}
		SceneManager::Instance().AddEnemyUI(hp);
	}
	m_EnemyHPList[EnemyHPType::Normal] = _normal;
	m_EnemyHPList[EnemyHPType::Boss] = _boss;

	ifs.close();
}

std::shared_ptr<Bullet> ObjectManager::SetBulletParam()
{
	std::shared_ptr<Bullet> _bullet = std::make_shared<Bullet>();
	_bullet->Init();
	_bullet->SetPos(m_golem.lock()->GetBulletPoint());
	_bullet->SetGolem(m_golem.lock());
	_bullet->SetOwner(m_golem.lock());
	_bullet->SetObjManager(shared_from_this());
	_bullet->SetAtk(m_golem.lock()->GetParam().Atk);
	_bullet->SetName("Bullet");
	_bullet->SetID(m_id);
	m_id++;
	SceneManager::Instance().AddObject(_bullet);

	return _bullet;
}

void ObjectManager::SetBoneAlphaBulletParam(int id)
{
	std::shared_ptr<BoneAlpha> _alpha;
	for (auto& alpha : m_BoneAlphaList)
	{
		if (alpha.lock()->GetID() == id)
		{
			_alpha = alpha.lock();
			break;
		}
	}

	if (!_alpha)return;

	std::shared_ptr<BoneAlpha_Bullet> _bullet = std::make_shared<BoneAlpha_Bullet>();
	_bullet->Init();
	_bullet->SetPos(_alpha->GetBulletPoint());
	Math::Matrix _nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_alpha->GetAngle().y));
	Math::Vector3 _nowVec = Math::Vector3::TransformNormal(_alpha->GetForward(), _nowRot);
	_bullet->SetDir(_nowVec);
	_bullet->SetOwner(_alpha);
	_bullet->SetAtk(_alpha->GetParam().Atk);
	_bullet->SetName("AlphaBullet");
	_bullet->SetID(m_id);
	m_id++;
	SceneManager::Instance().AddObject(_bullet);
}

const bool ObjectManager::GetTeleportFlg()
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

void ObjectManager::AddSelect()
{
	if (!m_select.expired())return;

	std::string _name = "Cursor";
	float _size = 1.0f;
	float _alphaCorrection = 1.5f;
	float _changeAngle = 2.0f;
	std::shared_ptr<Select> _select = std::make_shared<Select>();
	_select->SetSize(_size);
	_select->SetAlphaCorrection(_alphaCorrection);
	_select->SetChangeAngle(_changeAngle);
	_select->Init();

	m_select = _select;
	SceneManager::Instance().AddObject(_select);
}

void ObjectManager::AddBone()
{
	std::string _name = "Bone";
	Math::Vector3 _pos = Math::Vector3::Zero;
	_pos.y += m_ground.lock()->GetPos().y;
	Math::Vector3 _dir = Math::Vector3::Zero;
	float _size = 1.5f;
	Math::Vector3 _angle = Math::Vector3::Zero;
	int _hp = 10;
	int _atk = 2;
	float _speed = 1.0f;
	int _stamina = 50;
	float _atkRange = 3.0f;
	float _appealSize = 3.0f;
	float _enemyRadius = 3.0f;
	float _leaveDist = 6.0f;
	float _atkSphere = 1.5f;
	float _stumbleMove = 0.1f;
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
	enemy->SetAppealEffectSize(_appealSize);
	enemy->SetEnemyCheckRadius(_enemyRadius);
	enemy->SetLeaveDist(_leaveDist);
	enemy->SetAttackSphereSize(_atkSphere);
	enemy->SetStumbleMove(_stumbleMove);
	enemy->SetForward(_forward);
	enemy->SetName(_name);
	enemy->SetCamera(m_camera.lock());
	enemy->SetTarget(m_player.lock());
	enemy->SetID(m_id);
	enemy->SetObjManager(shared_from_this());
	enemy->Init();
	m_id++;

	SceneManager::Instance().AddEnemy(enemy);

	SetEnemyHPParam(enemy);
}

void ObjectManager::AddBoneAlpha()
{
	std::string _name = "BoneAlpha";
	Math::Vector3 _pos = Math::Vector3::Zero;
	_pos.y += m_ground.lock()->GetPos().y;
	Math::Vector3 _dir = Math::Vector3::Zero;
	float _size = 1.5f;
	Math::Vector3 _angle = Math::Vector3::Zero;
	int _hp = 10;
	int _atk = 2;
	float _speed = 1.0f;
	int _stamina = 50;
	float _appealSize = 3.0f;
	float _atkRange = 50.0f;
	float _enemyRadius = 3.0f;
	float _leaveDist = 6.0f;
	float _atkSphere = 1.5f;
	float _stumbleMove = 0.1f;
	Math::Vector3 _forward = Math::Vector3::Zero;
	_forward.z = 1.0f;
	float _chaseRange = 1000.0f;

	std::shared_ptr<BoneAlpha> enemy = nullptr;
	enemy = std::make_shared<BoneAlpha>();
	enemy->SetParam(_hp, _atk, _speed, _stamina);
	enemy->SetPos(_pos);
	enemy->SetSize(_size);
	enemy->SetDir(_dir);
	enemy->SetAngle(_angle);
	enemy->SetAppealEffectSize(_appealSize);
	enemy->SetAtkRange(_atkRange);
	enemy->SetEnemyCheckRadius(_enemyRadius);
	enemy->SetLeaveDist(_leaveDist);
	enemy->SetAttackSphereSize(_atkSphere);
	enemy->SetStumbleMove(_stumbleMove);
	enemy->SetForward(_forward);
	enemy->SetName(_name);
	enemy->SetCamera(m_camera.lock());
	enemy->SetTarget(m_player.lock());
	enemy->SetID(m_id);
	enemy->SetObjManager(shared_from_this());
	enemy->Init();
	m_id++;

	SceneManager::Instance().AddEnemy(enemy);

	SetEnemyHPParam(enemy);
}

void ObjectManager::AddGolem()
{
	if (!m_golem.expired())return;

	std::string _name = "Golem";
	Math::Vector3 _pos = Math::Vector3::Zero;
	_pos.y += m_ground.lock()->GetPos().y;
	Math::Vector3 _dir = Math::Vector3::Zero;
	float _size = 1.0f;
	Math::Vector3 _angle = Math::Vector3::Zero;
	_angle.y = 180.0f;
	int _hp = 10;
	int _atk = 2;
	float _speed = 1.0f;
	int _stamina = 50;
	float _appealSize = 5.0f;
	float _atkRange = 50.0f;
	float _enemyRadius = 3.0f;
	float _leaveDist = 6.0f;
	float _atkSphere = 3.0f;
	float _stumbleMove = 0.1f;
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
	enemy->SetAppealEffectSize(_appealSize);
	enemy->SetAtkRange(_atkRange);
	enemy->SetEnemyCheckRadius(_enemyRadius);
	enemy->SetLeaveDist(_leaveDist);
	enemy->SetAttackSphereSize(_atkSphere);
	enemy->SetStumbleMove(_stumbleMove);
	enemy->SetForward(_forward);
	enemy->SetName(_name);
	enemy->SetCamera(m_camera.lock());
	enemy->SetTarget(m_player.lock());
	enemy->SetID(m_id);
	enemy->SetObjManager(shared_from_this());
	enemy->Init();
	m_id++;

	SceneManager::Instance().AddEnemy(enemy);

	SetEnemyHPParam(enemy);

	m_golem = enemy;
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

	for (auto& obj : _json)
	{
		std::string _name;
		_name = obj["Name"];
		if (_name != _weaponName)continue;

		Math::Vector3 _pos = Math::Vector3::Zero;
		_pos.x = obj["PosX"];
		_pos.y = obj["PosY"];
		_pos.z = obj["PosZ"];

		float _size = 0.0f;
		_size = obj["Size"];

		Math::Vector3 _angle = Math::Vector3::Zero;
		_angle.y = obj["Angle"];

		std::shared_ptr<WeaponBase> weapon = nullptr;
		if (obj["ObjectName"] == "Sword")
		{
			std::shared_ptr<Sword> sword = std::make_shared<Sword>();
			sword->SetATK(obj["ATK"]);
			sword->SetAttackSphereSize(obj["AttackSphereSize"]);
			sword->SetTrajectPointNUM(obj["Traject"]);
			m_player.lock()->SetSword(sword);
			weapon = sword;
		}
		else if (obj["ObjectName"] == "Shield")
		{
			std::shared_ptr<Shield> shield = std::make_shared<Shield>();
			m_player.lock()->SetShield(shield);
			weapon = shield;
		}

		std::string _modelPath;
		_modelPath = obj["Path"];
		
		weapon->SetModelPath(_modelPath);
		weapon->Init();
		weapon->SetPos(_pos);
		weapon->SetSize(_size);
		weapon->SetAngle(_angle);
		weapon->SetName(_name);
		weapon->SetTarget(m_player.lock());
		weapon->SetID(m_id);
		m_id++;

		SceneManager::Instance().AddWeapon(weapon);

		break;
	}

	ifs.close();
}

void ObjectManager::AddGround()
{
	if (!m_ground.expired())return;

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

	m_ground = obj;
	SceneManager::Instance().AddObject(obj);
}

void ObjectManager::AddCircle()
{
	if (!m_circle.expired())return;

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
			m_circle = circle;
			obj = circle;
			break;
		case 1:
			magic = std::make_shared<MagicPolygon>();
			_name = "Magic";
			if (circle)_pos = circle->GetMagicPolygonPoint();
			else { _pos = Math::Vector3::Zero; }
			_size = 5.0f;
			_angle.y = 0.0f;
			m_magic = magic;
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


		SceneManager::Instance().AddObject(obj);
	}
}

void ObjectManager::AddWall()
{
	if (!m_wall.expired())return;

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

	m_wall = obj;
	SceneManager::Instance().AddObject(obj);
}

void ObjectManager::AddSkyBox()
{
	if (!m_skybox.expired())return;

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

	m_skybox = obj;
	SceneManager::Instance().AddObject(obj);
}

void ObjectManager::ChangeWeapon(std::string _swordName, std::string _shieldName)
{
	// 現在の剣の名前と一致しなかったら変更
	if (m_player.lock()->GetSword().lock()->GetName() != _swordName)
	{
		// 現在の剣は消滅
		m_player.lock()->GetSword().lock()->Expired();

		AddWeapon("Asset/Json/Game/Weapon/Sword/Sword.json", _swordName);
	}

	// 現在の盾の名前と一致しなかったら変更
	if (m_player.lock()->GetShield().lock()->GetName() != _shieldName)
	{
		// 現在の盾は消滅
		m_player.lock()->GetShield().lock()->Expired();

		AddWeapon("Asset/Json/Game/Weapon/Shield/Shield", _shieldName);
	}
}
