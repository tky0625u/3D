#include "Player.h"
#include"../../ObjectManager.h"
#include"../../Camera/CameraBase.h"
#include"../Action/Player/Idol/Player_Idol.h"
#include"Application/main.h"
#include"../Action/Player/Player_ConText.h"
#include"../../UI/Player/Player_UI_Manager.h"

void Player::Action()
{
	if (m_NextState != nullptr)
	{
		m_state = m_NextState;
		m_NextState.reset();
	}
	m_state.lock()->Update();
}

void Player::Update()
{
	CharacterBase::Update();
	m_ui->Update();
}

void Player::PostUpdate()
{
	CharacterBase::PostUpdate();
	m_camera.lock()->SlowChange(m_ObjManager.lock()->GetSlowFlg());

	if (m_ObjManager.lock()->GetSlowFlg())
	{
		if (m_FocusBackRange != 10.0f)m_FocusBackRange = 10.0f;
	}
	else
	{
		if (m_FocusBackRange != 2000.0f)m_FocusBackRange = 2000.0f;
	}

	m_camera.lock()->WorkCamera()->SetFocus(8, 5, m_FocusBackRange);
}

void Player::DrawSprite()
{
	m_ui->DrawSprite();
}

void Player::Init()
{
	CharacterBase::Init();
	m_MaxStamina = m_param.Sm;

	m_model->SetModelData("Asset/Models/Character/Player/Player.gltf");
	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	std::shared_ptr<Player_Idol> idol = std::make_shared<Player_Idol>();
	idol->SetTarget(shared_from_this());

	m_context = std::make_shared<Player_ActionConText>(idol);
	m_state = m_context->GetState();

	m_ui = std::make_shared<Player_UI_Manager>();
	m_ui->SetPlayer(shared_from_this());
	m_ui->Init();

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Player", m_model, KdCollider::TypeBump | KdCollider::TypeDamage | KdCollider::TypeEvent);

	m_ObjType = ObjType::oPlayer;
}

void Player::CrushingAction()
{
	CharacterBase::CrushingAction();
	if (m_dossolve >= 1.0f)m_dossolve = 1.0f;
}
