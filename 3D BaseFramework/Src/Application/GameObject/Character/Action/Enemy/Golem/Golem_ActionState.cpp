﻿#include "Golem_ActionState.h"
#include"../Enemy_ConText.h"
#include"../../../Enemy/golem/Golem.h"
#include"../../../Player/Player.h"
#include"../../../../ObjectManager.h"

#include"Appeal/Golem_Appeal.h"
#include"Idol/Golem_Idol.h"
#include"Run/Golem_Run.h"
#include"Attack/Golem_Attack.h"
#include"Hit/Golem_Hit.h"
#include"Stumble/Golem_Stumble.h"

void Golem_ActionState::Idol(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Golem_Idol> idol = std::make_shared<Golem_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	idol->SetObjectManager(m_ObjManager.lock());
	context->SetState(idol);
	m_target.lock()->SetNextAction(idol, m_target.lock()->Action::IdolType);
}

void Golem_ActionState::Run(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Golem_Run> run = std::make_shared<Golem_Run>();
	if (m_target.expired())return;
	run->SetTarget(m_target.lock());
	run->SetObjectManager(m_ObjManager.lock());
	context->SetState(run);
	m_target.lock()->SetNextAction(run, m_target.lock()->Action::RunType);
}

void Golem_ActionState::Attack(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Golem_Attack> attack = std::make_shared<Golem_Attack>();
	if (m_target.expired())return;
	attack->SetTarget(m_target.lock());
	attack->SetObjectManager(m_ObjManager.lock());
	context->SetState(attack);
	m_target.lock()->SetNextAction(attack, m_target.lock()->Action::AttackType);
}

void Golem_ActionState::Hit(std::shared_ptr<Enemy_ConText> context, int _damage)
{
	if (m_target.expired())return;
	m_target.lock()->Hit(_damage);
	if (m_target.lock()->GetParam().Hp <= 0)return;
	std::shared_ptr<Golem_Hit> hit = std::make_shared<Golem_Hit>();
	hit->SetTarget(m_target.lock());
	hit->SetObjectManager(m_ObjManager.lock());
	m_ObjManager.lock()->SetStopTime(5);
	context->SetState(hit);
	m_target.lock()->SetNextAction(hit, m_target.lock()->Action::HitType);
}

void Golem_ActionState::Stumble(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Golem_Stumble> stumble = std::make_shared<Golem_Stumble>();
	if (m_target.expired())return;
	stumble->SetTarget(m_target.lock());
	stumble->SetObjectManager(m_ObjManager.lock());
	context->SetState(stumble);
	m_target.lock()->SetNextAction(stumble, m_target.lock()->Action::StumbleType);
}
