#include "Enemy_ActionState.h"
#include"../../../../Scene/SceneManager.h"
#include"../../Enemy/EnemyBase.h"
#include"../../Player/Player.h"
#include"Enemy_ConText.h"

#include"Idol/Enemy_Idol.h"
#include"Run/Enemy_Run.h"
#include"Attack/Enemy_Attack.h"
#include"Hit/Enemy_Hit.h"
#include"Stumble/Enemy_Stumble.h"

bool Enemy_ActionState::ChaseCheck()
{
	if (m_target.expired())return false;
	if (m_target.lock()->GetPlayer().expired())return false;

	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = m_target.lock()->GetPos();
	sphereInfo.m_sphere.Radius = m_target.lock()->GetChaseRange();
	sphereInfo.m_type = KdCollider::TypeEvent;

	if (m_target.lock()->GetPlayer().lock()->Intersects(sphereInfo, nullptr))
	{
		return true;
	}

	return false;
}

void Enemy_ActionState::AttackCheck(bool& _atkFlg)
{
	if (m_target.expired())return;
	if (m_target.lock()->GetPlayer().expired())return;

	KdCollider::SphereInfo sphereInfo;
	Math::Matrix nowRotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_target.lock()->GetParam().Angle));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3{ m_target.lock()->GetParam().ForwardX,m_target.lock()->GetParam().ForwardY,m_target.lock()->GetParam().ForwardZ }, nowRotY);
	nowVec.Normalize();
	sphereInfo.m_sphere.Center = m_target.lock()->GetPos() + nowVec * (m_target.lock()->GetParam().AtkRange / 2.0f);
	sphereInfo.m_sphere.Radius = m_target.lock()->GetParam().AtkRange / 2.0f;
	sphereInfo.m_type = KdCollider::TypeBump;

	if (m_target.lock()->GetPlayer().lock()->Intersects(sphereInfo, nullptr))
	{
		m_flow = Flow::EndType;
		_atkFlg = true;
		return;
	}

	_atkFlg = false;
}

void Enemy_ActionState::Idol(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Idol> idol = std::make_shared<Enemy_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	context->SetState(idol);
	m_target.lock()->SetNextState(idol);
}

void Enemy_ActionState::Run(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Run> run = std::make_shared<Enemy_Run>();
	if (m_target.expired())return;
	run->SetTarget(m_target.lock());
	context->SetState(run);
	m_target.lock()->SetNextState(run);
}

void Enemy_ActionState::Attack(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Attack> attack = std::make_shared<Enemy_Attack>();
	if (m_target.expired())return;
	attack->SetTarget(m_target.lock());
	context->SetState(attack);
	m_target.lock()->SetNextState(attack);
}

void Enemy_ActionState::Hit(std::shared_ptr<Enemy_ConText> context, int _damage)
{
	std::shared_ptr<Enemy_Hit> hit = std::make_shared<Enemy_Hit>();
	if (m_target.expired())return;
	hit->SetTarget(m_target.lock());
	context->SetState(hit);
	m_target.lock()->SetNextState(hit);
	m_target.lock()->GetParam().Hp -= _damage;
}

void Enemy_ActionState::Stumble(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Stumble> stumble = std::make_shared<Enemy_Stumble>();
	if (m_target.expired())return;
	stumble->SetTarget(m_target.lock());
	context->SetState(stumble);
	m_target.lock()->SetNextState(stumble);
}
