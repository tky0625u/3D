#include "Bone_ActionState.h"
#include"../Enemy_ConText.h"
#include"../../../Enemy/Bone/Bone.h"
#include"../../../Player/Player.h"
#include"../../../../ObjectManager.h"

#include"Appeal/Bone_Appeal.h"
#include"Idol/Bone_Idol.h"
#include"Run/Bone_Run.h"
#include"Attack/Bone_Attack.h"
#include"Hit/Bone_Hit.h"
#include"Stumble/Bone_Stumble.h"
#include"Crushing/Bone_Crushing.h"

void Bone_ActionState::Idol()
{
	std::shared_ptr<Bone_Idol> idol = std::make_shared<Bone_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	idol->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetNextAction(idol, EnemyBase::Action::IdolType);
}

void Bone_ActionState::Run()
{
	std::shared_ptr<Bone_Run> run = std::make_shared<Bone_Run>();
	if (m_target.expired())return;
	run->SetTarget(m_target.lock());
	run->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetNextAction(run, EnemyBase::Action::RunType);
}

void Bone_ActionState::Attack()
{
	std::shared_ptr<Bone_Attack> attack = std::make_shared<Bone_Attack>();
	if (m_target.expired())return;
	attack->SetTarget(m_target.lock());
	attack->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetNextAction(attack, EnemyBase::Action::AttackType);
}

void Bone_ActionState::Hit(int _damage)
{
	if (m_target.expired())return;
	m_target.lock()->Hit(_damage);
	if (m_target.lock()->GetParam().Hp <= 0)
	{
		Crushing();
		return;
	}
	std::shared_ptr<Bone_Hit> hit = std::make_shared<Bone_Hit>();
	hit->SetTarget(m_target.lock());
	hit->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetNextAction(hit, EnemyBase::Action::HitType);
}

void Bone_ActionState::Stumble()
{
	std::shared_ptr<Bone_Stumble> stumble = std::make_shared<Bone_Stumble>();
	if (m_target.expired())return;
	stumble->SetTarget(m_target.lock());
	stumble->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetNextAction(stumble, EnemyBase::Action::StumbleType);
}

void Bone_ActionState::Crushing()
{
	std::shared_ptr<Bone_Crushing> crushing = std::make_shared<Bone_Crushing>();
	if (m_target.expired())return;
	crushing->SetTarget(m_target.lock());
	crushing->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetNextAction(crushing, EnemyBase::Action::CrushingType);
}
