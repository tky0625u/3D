#include "GameCamera_State.h"

#include"GameCamera.h"

// プレイヤーカメラ
#include"PlayerCamera/GameCamera_Player.h"
// 定点カメラ
#include"FixedCamera/FixedCamera.h"
// クリアカメラ
#include"ClearCamera/GameCamera_Clear.h"

void GameCamera_State::PlayerChange()
{
	std::shared_ptr<PlayerCamera> _player = std::make_shared<PlayerCamera>();
	_player->SetTarget(m_target.lock());
	m_target.lock()->SetCameraType(GameCamera::CameraType::PlayerType);
	m_target.lock()->SetNextState(_player);
}

void GameCamera_State::FixedChange()
{
	std::shared_ptr<FixedCamera> _fixed = std::make_shared<FixedCamera>();
	_fixed->SetTarget(m_target.lock());
	m_target.lock()->SetCameraType(GameCamera::CameraType::FixedType);
	m_target.lock()->SetNextState(_fixed);
}

void GameCamera_State::ClearChange()
{
	std::shared_ptr<ClearCamera> _clear = std::make_shared<ClearCamera>();
	_clear->SetTarget(m_target.lock());
	m_target.lock()->SetCameraType(GameCamera::CameraType::ClearType);
	m_target.lock()->SetNextState(_clear);
}

void GameCamera_State::Shake()
{
	Math::Vector3 _pos = m_target.lock()->GetNowPos();

	m_angle.y += 100.0f;
	if (m_angle.y > 360.0f)m_angle.y -= 360.0f;
	_pos.y += m_move * (cos(DirectX::XMConvertToRadians(m_angle.y)));
	m_mLocalPos = Math::Matrix::CreateTranslation(_pos);
	m_shakeTime--;
	if (m_shakeTime <= 0.0f)
	{
		m_shakeTime = 10.0f;
		m_angle.y = 0.0f;
		m_shakeFlg = false;
	}
}
