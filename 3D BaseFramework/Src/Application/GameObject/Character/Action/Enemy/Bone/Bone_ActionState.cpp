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

void Bone_ActionState::Idol(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Bone_Idol> idol = std::make_shared<Bone_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	context->SetState(idol);
	m_target.lock()->SetNextAction(idol, m_target.lock()->Action::IdolType);
}

void Bone_ActionState::Run(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Bone_Run> run = std::make_shared<Bone_Run>();
	if (m_target.expired())return;
	run->SetTarget(m_target.lock());
	context->SetState(run);
	m_target.lock()->SetNextAction(run, m_target.lock()->Action::RunType);
}

void Bone_ActionState::Attack(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Bone_Attack> attack = std::make_shared<Bone_Attack>();
	if (m_target.expired())return;
	attack->SetTarget(m_target.lock());
	context->SetState(attack);
	m_target.lock()->SetNextAction(attack, m_target.lock()->Action::AttackType);
}

void Bone_ActionState::Hit(std::shared_ptr<Enemy_ConText> context, int _damage)
{
	if (m_target.expired())return;
	m_target.lock()->Hit(_damage);
	if (m_target.lock()->GetParam().Hp <= 0)return;
	std::shared_ptr<Bone_Hit> hit = std::make_shared<Bone_Hit>();
	hit->SetTarget(m_target.lock());
	ObjectManager::Instance().SetStopTime(5);
	context->SetState(hit);
	m_target.lock()->SetNextAction(hit, m_target.lock()->Action::HitType);
}

void Bone_ActionState::Stumble(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Bone_Stumble> stumble = std::make_shared<Bone_Stumble>();
	if (m_target.expired())return;
	stumble->SetTarget(m_target.lock());
	context->SetState(stumble);
	m_target.lock()->SetNextAction(stumble, m_target.lock()->Action::StumbleType);
}
