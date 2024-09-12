#include "Player.h"
#include"../../Camera/CameraBase.h"
#include"../Action/Player/Idol/Player_Idol.h"
#include"Application/main.h"
#include"../Action/Player/Player_ConText.h"

void Player::Action()
{
	m_dir = Math::Vector3::Zero; //ベクトルリセット
	float         Move = 0.0f;
	
	if (m_NextState.expired() == false)
	{
		m_state = m_NextState.lock();
		m_NextState.reset();
	}
	m_state.lock()->Update();

	Move = m_param.Sp * m_SpeedCorrection;
	m_pos += Move * m_dir; //座標更新

}

void Player::Init()
{
	CharacterBase::Init();
	m_model->SetModelData("Asset/Models/Character/Player/Player.gltf");
	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	std::shared_ptr<Player_Idol> idol = std::make_shared<Player_Idol>();
	idol->SetTarget(shared_from_this());

	m_context = std::make_shared<Player_ActionConText>(idol);
	m_state = m_context->GetState();

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Player", m_model, KdCollider::TypeBump | KdCollider::TypeDamage | KdCollider::TypeEvent);

	m_ObjType = ObjType::oPlayer;
}

void Player::CrushingAction()
{
	if (m_anime != "Death")
	{
		m_anime = "Death";
		m_animeFlg = false;
		m_animeSpeed = 1.0f;
	}
}
