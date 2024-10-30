#include "GameCamera_State.h"

#include"GameCamera.h"

// プレイヤーカメラ
#include"PlayerCamera/GameCamera_Player.h"
// 定点カメラ
#include"FixedCamera/FixedCamera.h"

void GameCamera_State::Player()
{
	std::shared_ptr<PlayerCamera> _player = std::make_shared<PlayerCamera>();
	_player->SetTarget(m_target.lock());
	_player->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetCameraType(GameCamera::CameraType::PlayerType);
	m_target.lock()->SetNextState(_player);
}

void GameCamera_State::Fixed()
{
	std::shared_ptr<FixedCamera> _fixed = std::make_shared<FixedCamera>();
	_fixed->SetTarget(m_target.lock());
	_fixed->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetCameraType(GameCamera::CameraType::FixedType);
	m_target.lock()->SetNextState(_fixed);
}

void GameCamera_State::Clear()
{
}
