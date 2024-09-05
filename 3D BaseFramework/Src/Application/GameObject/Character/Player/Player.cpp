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
		m_ActionType |= ActionType::Move;
		m_context->Run();
	}

	//攻撃
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_ActionType |= ActionType::Attack;
		m_context->Attack();
	}

	//防御
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_ActionType |= ActionType::Guard;
		m_context->Guard();
	}

	//回避
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_ActionType |= ActionType::Roll;
		m_context->Roll();
	}

	m_state = m_context->GetState();
	if (m_state.expired() == false)m_state.lock()->SetConText(m_context);
}

void Player::Action()
{
	m_dir = Math::Vector3::Zero; //ベクトルリセット
	float         Move = 0.0f;
	m_moveFlg = false;

	if (m_state.expired() == false)
	{
		int flow = m_state.lock()->GetFlow();

		if (flow == m_state.lock()->StartType) { m_state.lock()->Start(); }
		else if (flow == m_state.lock()->CenterType) { m_state.lock()->Center(); }
		else if (flow == m_state.lock()->EndType) { m_state.lock()->End(); }
	}


	Move = m_param.Sp * m_SpeedCorrection;
	m_pos += Move * m_dir; //座標更新
	m_BeforeActionType = m_ActionType;
}

void Player::Init()
{
	CharacterBase::Init();
	m_model->SetModelData("Asset/Models/Character/Player/Player.gltf");
	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	std::shared_ptr<Player_Idol> idol = std::make_shared<Player_Idol>();
	idol->SetTarget(shared_from_this());

	m_context = std::make_shared<Player_ActionConText>(idol);
	if(m_camera.expired()==false)m_context->SetCamera(m_camera.lock());
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
