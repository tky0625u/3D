#include "LeaderBase.h"
#include"../../Camera/CameraBase.h"

void LeaderBase::Action()
{
	//移動
	Math::Vector3 dir = Math::Vector3::Zero; //ベクトルリセット
	bool          moveFlg = false;
	if (GetAsyncKeyState('W') & 0x8000)
	{
		dir.z = 1.0f;
		moveFlg = true;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		dir.z = -1.0f;
		moveFlg = true;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		dir.x = -1.0f;
		moveFlg = true;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		dir.x = 1.0f;
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

		CharacterBase::Rotation(dir);

		float Move = m_status.SP * m_SpeedCorrection;
		//ダッシュ
		if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		{
			Move *= m_DashCorrection;
		}

		m_pos += Move * dir; //座標更新
	}

}

void LeaderBase::Init()
{
	CharacterBase::Init();
	m_characterType = CharacterType::Leader;
}
