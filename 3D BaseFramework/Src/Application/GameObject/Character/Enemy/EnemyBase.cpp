#include "EnemyBase.h"
#include"../../../Scene/SceneManager.h"
#include"../Player/Player.h"
#include"../Action/Enemy/Enemy_ConText.h"

void EnemyBase::PreUpdate()
{
	m_state = m_conText->GetState();
}

void EnemyBase::Action()
{
	m_dir = Math::Vector3::Zero;
	float Move = 0.0f;

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

void EnemyBase::Init()
{
	CharacterBase::Init();

	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Trans;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_model, KdCollider::TypeDamage | KdCollider::TypeBump);

	m_ObjType = ObjType::oEnemy;
}
