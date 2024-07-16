#include "Player.h"
#include"../../Camera/CameraBase.h"

void Player::Action()
{
	//移動
	Math::Vector3 dir = Math::Vector3::Zero; //ベクトルリセット
	bool          moveFlg = false;
	if (GetAsyncKeyState('W') & 0x8000)
	{
		dir.z   = 1.0f;
		moveFlg = true;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		dir.z   = -1.0f;
		moveFlg = true;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		dir.x   = -1.0f;
		moveFlg = true;
	}	
	if (GetAsyncKeyState('D') & 0x8000)
	{
		dir.x   = 1.0f;
		moveFlg = true;
	}

	Math::Matrix cameraRotYMat = Math::Matrix::Identity;
	if (m_camera.expired() == false)
	{
		cameraRotYMat = m_camera.lock()->GetRotationYMatrix();
	}
	dir = dir.TransformNormal(dir, cameraRotYMat);

	//回転
	if (moveFlg)
	{
		dir.Normalize();       //正規化
		
		//今の方向
		Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
		Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3(0, 0, 1), nowRot);

		//向きたい方向
		Math::Vector3 toVec = dir;
		toVec.Normalize();

		//内角 回転する各を求める
		float d = nowVec.Dot(toVec);
		d = std::clamp(d, -1.0f, 1.0f); //誤差修正

		//回転角度を求める
		float ang = DirectX::XMConvertToDegrees(acos(d));

		//角度変更
		if (ang >= 0.1f)
		{
			if (ang > 20)
			{
				ang = 20.0f; //変更角度
			}

			//外角　どっち回転かを求める
			Math::Vector3 c = toVec.Cross(nowVec);
			if (c.y >= 0)
			{
				//右回転
				m_angle -= ang;
				if (m_angle < 0.0f)
				{
					m_angle += 360.0f;
				}
			}
			else
			{
				//左回転
				m_angle += ang;
				if (m_angle >= 360.0f)
				{
					m_angle -= 360.0f;
				}
			}
		}

		float Move = m_status.SP * m_SpeedCorrection;
		//ダッシュ
		if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		{
			Move *= m_DashCorrection;
		}

		m_pos += Move * dir; //座標更新
	}

}

void Player::Init()
{
	CharacterBase::Init();
	m_model->Load("Asset/Models/Character/Player/tank.gltf");

	StatusLoad("CSV/Character/Status/Player/Player.csv");

	m_sizeHalf = 1.0f;
	m_DashCorrection = 2.0f;

}

void Player::CrushingAction()
{
	CharacterBase::CrushingAction();
}
