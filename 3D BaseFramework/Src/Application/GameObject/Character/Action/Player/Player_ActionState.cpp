#include "Player_ActionState.h"
#include"../../../ObjectManager.h"
#include"../../Enemy/EnemyBase.h"
#include"../Enemy/Enemy_ConText.h"
#include"../../Player/Player.h"
#include"Player_ConText.h"
#include"../../../Weapon/Sword/Sword.h"

#include"Idol/Player_Idol.h"
#include"Run/Player_Run.h"
#include"Attack/Player_Attack.h"
#include"Roll/Player_Roll.h"
#include"Guard/Player_Guard.h"
#include"GuardReaction/Player_GuardReaction.h"
#include"Parry/Player_Parry.h"
#include"Counter/Player_Counter.h"
#include"Hit/Player_Hit.h"

void Player_ActionState::AttackDamage()
{
	if (m_target.expired())return;

	std::vector<KdCollider::SphereInfo> sphereInfoList;
	KdCollider::SphereInfo sphereInfo;

	if (m_target.lock()->GetSword().expired() == false)
	{
		sphereInfo.m_sphere.Center = m_target.lock()->GetSword().lock()->GetModelTop().Translation();
		sphereInfoList.push_back(sphereInfo);

		sphereInfo.m_sphere.Center = m_target.lock()->GetSword().lock()->GetModelCenter().Translation();
		sphereInfoList.push_back(sphereInfo);

		sphereInfo.m_sphere.Center = m_target.lock()->GetSword().lock()->GetModelBottom().Translation();
		sphereInfoList.push_back(sphereInfo);
	}
	else
	{
		sphereInfo.m_sphere.Center = m_target.lock()->GetSwordMat().Translation();
		sphereInfoList.push_back(sphereInfo);
	}

	for (int i = 0; i < sphereInfoList.size(); ++i)
	{
		sphereInfoList[i].m_sphere.Radius = 0.8f;
		sphereInfoList[i].m_type = KdCollider::TypeDamage;
	}

	std::list<KdCollider::CollisionResult> retSphereList;
	std::vector<std::shared_ptr<EnemyBase>> hitEnemyList;

	for (auto& sphere : ObjectManager::Instance().GetEnemyList())
	{
		if (sphere.expired())continue;

		for (int i = 0; i < sphereInfoList.size(); ++i)
		{
			if (sphere.lock()->Intersects(sphereInfoList[i], &retSphereList))
			{
				hitEnemyList.push_back(sphere.lock());
			}
		}
	}

	int enemy = 0;
	for (auto& ret : retSphereList)
	{
		if (hitEnemyList[enemy]->GetParam().Hp > 0 && hitEnemyList[enemy]->GetActionType() != EnemyBase::Action::AppealType && hitEnemyList[enemy]->GetinviTime() == 0)
		{
			hitEnemyList[enemy]->GetConText()->Hit(m_target.lock()->GetParam().Atk);
			hitEnemyList[enemy]->SetInviTime(m_target.lock()->GetinviTime());
			KdEffekseerManager::GetInstance().Play("hit_eff.efkefc", ret.m_hitPos, 0.4f, 0.8f, false);
		}
		enemy++;
	}
}

void Player_ActionState::Update()
{
	ActionBase::Update();
	if (m_staminaRecoveryFlg)StaminaRecovery();
	KeyCheck();
	ChangeAction();
	m_target.lock()->GetConText()->SetBeforeActionType(m_ActionType);
}

void Player_ActionState::KeyCheck()
{
	std::shared_ptr<Player_ActionConText> _context = nullptr;
	if (m_target.expired() == false)_context = m_target.lock()->GetConText();

	//移動
	if (GetAsyncKeyState('W') & 0x8000 | GetAsyncKeyState('A') & 0x8000 | GetAsyncKeyState('S') & 0x8000 | GetAsyncKeyState('D') & 0x8000)
	{
		m_ActionType |= Player_ActionConText::ActionType::MoveType;
	}
	else if (m_ActionType & Player_ActionConText::ActionType::MoveType)
	{
		m_ActionType ^= Player_ActionConText::ActionType::MoveType;
	}

	//攻撃
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_ActionType |= Player_ActionConText::ActionType::AttackType;
	}
	else if (m_ActionType & Player_ActionConText::ActionType::AttackType)
	{
		m_ActionType ^= Player_ActionConText::ActionType::AttackType;
	}

	//防御
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_ActionType |= Player_ActionConText::ActionType::GuardType;
	}
	else if (m_ActionType & Player_ActionConText::ActionType::GuardType)
	{
		m_ActionType ^= Player_ActionConText::ActionType::GuardType;
	}

	//回避
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_ActionType |= Player_ActionConText::ActionType::RollType;
	}
	else if (m_ActionType & Player_ActionConText::ActionType::RollType)
	{
		m_ActionType ^= Player_ActionConText::ActionType::RollType;
	}
}

void Player_ActionState::StaminaRecovery()
{
	if (m_target.lock()->GetParam().Sm < m_target.lock()->GetMaxStamina())
	{
		m_target.lock()->StaminaRecovery();
	}
}

void Player_ActionState::Idol(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Idol> idol = std::make_shared<Player_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	context->SetState(idol);
	m_target.lock()->SetNextState(idol);
}

void Player_ActionState::Run(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Run> run = std::make_shared<Player_Run>();
	if (m_target.expired())return;
	run->SetTarget(m_target.lock());
	context->SetState(run);
	m_target.lock()->SetNextState(run);
}

void Player_ActionState::Attack(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Attack> attack = std::make_shared<Player_Attack>();
	if (m_target.expired())return;
	attack->SetTarget(m_target.lock());
	attack->AttackDirCheck();
	attack->AttackRangeCheck();
	context->SetState(attack);
	m_target.lock()->SetNextState(attack);
}

void Player_ActionState::Guard(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Guard> guard = std::make_shared<Player_Guard>();
	if (m_target.expired())return;
	guard->SetTarget(m_target.lock());
	context->SetState(guard);
	m_target.lock()->SetNextState(guard);
}

void Player_ActionState::GuardReaction(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_GuardReaction> guardReaction = std::make_shared<Player_GuardReaction>();
	if (m_target.expired())return;
	guardReaction->SetTarget(m_target.lock());
	context->SetState(guardReaction);
	m_target.lock()->SetNextState(guardReaction);
}

void Player_ActionState::Parry(std::shared_ptr<Player_ActionConText> context, std::shared_ptr<EnemyBase> _enemy)
{
	std::shared_ptr<Player_Parry> parry = std::make_shared<Player_Parry>();
	if (m_target.expired())return;
	parry->SetTarget(m_target.lock());
	context->SetState(parry);
	m_target.lock()->SetNextState(parry);
	_enemy->GetConText()->Stumble();
}

void Player_ActionState::Counter(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Counter> counter = std::make_shared<Player_Counter>();
	if (m_target.expired())return;
	counter->SetTarget(m_target.lock());
	context->SetState(counter);
	m_target.lock()->SetNextState(counter);
}

void Player_ActionState::Roll(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Roll> roll = std::make_shared<Player_Roll>();
	if (m_target.expired())return;
	roll->SetTarget(m_target.lock());
	context->SetState(roll);
	m_target.lock()->SetNextState(roll);
}

void Player_ActionState::Hit(std::shared_ptr<Player_ActionConText> context, int _damage, std::shared_ptr<EnemyBase> _enemy)
{
	std::shared_ptr<Player_Hit> hit = std::make_shared<Player_Hit>();
	if (m_target.expired())return;
	hit->SetTarget(m_target.lock());
	m_target.lock()->Hit(_damage);
	context->SetState(hit);
	m_target.lock()->SetNextState(hit);
}
