#include "Golem_ActionState.h"
#include"../Enemy_ConText.h"
#include"../../../Enemy/golem/Golem.h"
#include"../../../Player/Player.h"

#include"Appeal/Golem_Appeal.h"
#include"Idol/Golem_Idol.h"
#include"Run/Golem_Run.h"
#include"Attack/Attack1/Golem_Attack1.h"
#include"Attack/Attack2/Golem_Attack2.h"
#include"Hit/Golem_Hit.h"
#include"Stumble/Golem_Stumble.h"
#include"Crushing/Golem_Crushing.h"

void Golem_ActionState::Idol()
{
	std::shared_ptr<Golem_Idol> idol = std::make_shared<Golem_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	idol->SetObjManager(m_ObjectManager.lock());
	m_target.lock()->SetNextAction(idol, EnemyBase::Action::IdolType);
}

void Golem_ActionState::Run()
{
	std::shared_ptr<Golem_Run> run = std::make_shared<Golem_Run>();
	if (m_target.expired())return;
	run->SetTarget(m_target.lock());
	run->SetObjManager(m_ObjectManager.lock());
	m_target.lock()->SetNextAction(run, EnemyBase::Action::RunType);
}

void Golem_ActionState::Attack()
{
	Math::Vector3 _pos = m_target.lock()->GetPos();
	Math::Vector3 _playerPos = m_target.lock()->GetTarget().lock()->GetPos();
	float _dist = (_playerPos - _pos).Length();

	if (_dist <= 50.0f)
	{
		std::shared_ptr<Golem_Attack1> attack1 = std::make_shared<Golem_Attack1>();
		if (m_target.expired())return;
		attack1->SetTarget(m_target.lock());
		attack1->SetObjManager(m_ObjectManager.lock());
		m_target.lock()->SetNextAction(attack1, EnemyBase::Action::AttackType);
	}
	else
	{
		std::shared_ptr<Golem_Attack2> attack2 = std::make_shared<Golem_Attack2>();
		if (m_target.expired())return;
		attack2->SetTarget(m_target.lock());
		attack2->SetObjManager(m_ObjectManager.lock());
		m_target.lock()->SetNextAction(attack2, EnemyBase::Action::AttackType);
	}
}

void Golem_ActionState::Hit(int _damage)
{
	if (m_target.expired())return;
	m_target.lock()->Hit(_damage);
	if (m_target.lock()->GetParam().Hp <= 0)
	{
		Crushing();
		return;
	}
	std::shared_ptr<Golem_Hit> hit = std::make_shared<Golem_Hit>();
	hit->SetTarget(m_target.lock());
	hit->SetObjManager(m_ObjectManager.lock());
	m_target.lock()->SetNextAction(hit, EnemyBase::Action::HitType);
}

void Golem_ActionState::Stumble()
{
	std::shared_ptr<Golem_Stumble> stumble = std::make_shared<Golem_Stumble>();
	if (m_target.expired())return;
	stumble->SetTarget(m_target.lock());
	stumble->SetObjManager(m_ObjectManager.lock());
	m_target.lock()->SetNextAction(stumble, EnemyBase::Action::StumbleType);
}

void Golem_ActionState::Crushing()
{
	std::shared_ptr<Golem_Crushing> crushing = std::make_shared<Golem_Crushing>();
	if (m_target.expired())return;
	crushing->SetTarget(m_target.lock());
	crushing->SetObjManager(m_ObjectManager.lock());
	m_target.lock()->SetNextAction(crushing,EnemyBase::Action::CrushingType);
}
