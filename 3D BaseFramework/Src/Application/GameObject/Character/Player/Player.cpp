#include "Player.h"
#include"../../Camera/CameraBase.h"

void Player::Action()
{
	//攻撃
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_atkFlg = true;
		m_animeFlg = false;
		m_Action = "Attack1";
	}

	if (m_atkFlg)return;

	//移動
	Math::Vector3 dir = Math::Vector3::Zero; //ベクトルリセット
	bool          moveFlg = false;
	if (GetAsyncKeyState('W') & 0x8000)
	{
		dir.z = 1.0f;
		m_Action = "Run";
		moveFlg = true;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		dir.z = -1.0f;
		m_Action = "Run";
		moveFlg = true;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		dir.x = -1.0f;
		m_Action = "Run";
		moveFlg = true;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		dir.x = 1.0f;
		m_Action = "Run";
		moveFlg = true;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		//今の方向
		Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_param.Angle));
		Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3(m_param.ForwardX, m_param.ForwardY, m_param.ForwardZ), nowRot);
		dir = nowVec;
		m_Action = "Roll";
		//moveFlg = true;


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
		dir.Normalize(); //正規化

		CharacterBase::Rotation(dir);

		float Move = m_param.Sp * m_SpeedCorrection;
		//ダッシュ
		if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		{
			Move *= m_DashCorrection;
		}

		m_pos += Move * dir; //座標更新

		m_animeFlg = true;
	}
	else
	{
		if (m_Action=="Run")
		{
			m_Action = "RunToIdol";
			m_animeFlg = false;
		}
		if (m_animator->IsAnimationEnd())
		{
			m_Action = "Idol";
			m_animeFlg = true;
		}
	}

}

void Player::Init()
{
	CharacterBase::Init();
	m_model->SetModelData("Asset/Models/Character/Player/Player.gltf");
	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_Action), m_animeFlg);

	ParamLoad("CSV/Character/Param/Player/Player.csv");
}

void Player::CrushingAction()
{
	CharacterBase::CrushingAction();
}
