#include "Player.h"
#include"../../Camera/CameraBase.h"
#include"../Action/Player/Idol/Player_Idol.h"
#include"Application/main.h"
#include"../Action/Player/Player_ConText.h"

void Player::PreUpdate()
{
	//移動
	if (GetAsyncKeyState('W') & 0x8000 | GetAsyncKeyState('A') & 0x8000 | GetAsyncKeyState('S') & 0x8000 | GetAsyncKeyState('D') & 0x8000)
	{
		m_ActionType |= ActionType::MoveType;
		m_context->Run();
	}
	else if(m_ActionType & ActionType::MoveType)
	{
		m_ActionType ^= ActionType::MoveType;
	}

	//攻撃
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_ActionType |= ActionType::AttackType;
		if (!(m_BeforeActionType & ActionType::AttackType))m_context->Attack();
	}
	else if(m_ActionType & ActionType::AttackType)
	{
		m_ActionType ^= ActionType::AttackType;
	}

	//防御
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_ActionType |= ActionType::GuardType;
		m_context->Guard();
	}
	else if (m_ActionType & ActionType::GuardType)
	{
		m_ActionType ^= ActionType::GuardType;
	}

	//回避
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_ActionType |= ActionType::RollType;
		if (!(m_BeforeActionType & ActionType::RollType))m_context->Roll();
	}
	else if (m_ActionType & ActionType::RollType)
	{
		m_ActionType ^= ActionType::RollType;
	}

	m_BeforeActionType = m_ActionType;

	m_state = m_context->GetState();
	if (m_state.expired() == false)
	{
		m_state.lock()->SetConText(m_context);
		m_state.lock()->SetActionType(m_ActionType);
	}
}

void Player::Action()
{
	m_dir = Math::Vector3::Zero; //ベクトルリセット
	float         Move = 0.0f;

	if (m_state.expired() == false)
	{
		int flow = m_state.lock()->GetFlow();

		     if (flow == m_state.lock()->Flow::StartType)  { m_state.lock()->Start(); }
		else if (flow == m_state.lock()->Flow::CenterType) { m_state.lock()->Center(); }
		else if (flow == m_state.lock()->Flow::EndType)    { m_state.lock()->End(); }
	}


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
