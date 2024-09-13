#include "EnemyBase.h"
#include"../../../Scene/SceneManager.h"
#include"../Player/Player.h"
#include"../Action/Enemy/Enemy_ConText.h"
#include"../../UI/Enemy/Enemy_UI.h"

void EnemyBase::Update()
{
	CharacterBase::Update();

	m_ui->Update();
}

void EnemyBase::Action()
{
	m_dir = Math::Vector3::Zero;
	float Move = 0.0f;

	if (m_NextState != nullptr)
	{
		m_state = m_NextState;
		m_NextState.reset();
	}
	if (m_state.expired() == false)m_state.lock()->Update();

	Move = m_param.Sp * m_SpeedCorrection;
	m_pos += Move * m_dir; //座標更新
}

void EnemyBase::DrawSprite()
{
	m_ui->DrawSprite();
}

void EnemyBase::Init()
{
	CharacterBase::Init();

	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Trans;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_model, KdCollider::TypeDamage | KdCollider::TypeBump);

	m_ObjType = ObjType::oEnemy;

	m_ui = std::make_shared<Enemy_UI>();
	m_ui->SetTarget(shared_from_this());
	m_ui->SetCamera(m_player.lock()->GetCamera().lock());
	m_ui->Init();
}
