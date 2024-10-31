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

	for (auto& sphere : m_ObjManager.lock()->GetEnemyList())
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
			hitEnemyList[enemy]->Hit(m_target.lock()->GetParam().Atk);
			hitEnemyList[enemy]->GetConText()->Hit(m_target.lock()->GetParam().Atk);
			hitEnemyList[enemy]->SetInviTime(m_target.lock()->GetinviTime());
			KdEffekseerManager::GetInstance().Play("hit_eff.efkefc", ret.m_hitPos, 0.8f, 0.5f, false);
			KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/刀で斬る2.WAV", 0.05f, false);
		}
		enemy++;
	}
}

void Player_ActionState::LockON()
{
	if (m_target.lock()->GetConText()->GetLockONFlg() && !m_target.lock()->GetConText()->GetLockONTarget().expired() && m_target.lock()->GetConText()->GetLockONTarget().lock()->GetParam().Hp > 0)
	{
		m_target.lock()->GetConText()->SetLockONFlg(false);
		return;
	}

	float Dist = 0.0f;
	bool HitFlg = false;
	int listNum = 0;

	for (int e = 0; e < m_ObjManager.lock()->GetEnemyList().size(); ++e)
	{
		if (!m_ObjManager.lock()->GetEnemyList()[e].expired() &&
			m_ObjManager.lock()->GetEnemyList()[e].lock()->GetParam().Hp > 0)
		{
			if (!HitFlg)
			{
				float d = (m_ObjManager.lock()->GetEnemyList()[e].lock()->GetPos() - m_target.lock()->GetPos()).Length();
				Dist = d;
				HitFlg = true;
				listNum = e;
			}
			else
			{
				float d = (m_ObjManager.lock()->GetEnemyList()[e].lock()->GetPos() - m_target.lock()->GetPos()).Length();
				if (d < Dist)
				{
					Dist = d;
					listNum = e;
				}
			}
		}
	}

	if (HitFlg == true)
	{
		m_target.lock()->GetConText()->SetLockONTarget(m_ObjManager.lock()->GetEnemyList()[listNum].lock());
		m_target.lock()->GetConText()->SetLockONFlg(true);
	}
	else
	{
		m_target.lock()->GetConText()->SetLockONFlg(false);
	}
}

void Player_ActionState::Update()
{
	ActionBase::Update();
	if (m_target.lock()->GetStaminaRecoveryTime() <= 0)m_target.lock()->StaminaRecovery();
	KeyCheck();
	if (m_target.lock()->GetConText()->GetLockONFlg())
	{
		if (!m_target.lock()->GetConText()->GetLockONTarget().expired())
		{
			if (m_target.lock()->GetConText()->GetLockONTarget().lock()->GetParam().Hp <= 0)LockON();
		}
	}
	ChangeAction();
	m_target.lock()->GetConText()->SetBeforeActionType(m_ActionType);
}

void Player_ActionState::KeyCheck()
{
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

	//ロックオン
	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
	{
		if(!(m_target.lock()->GetConText()->GetBeforeActionType() & Player_ActionConText::ActionType::LockONType))LockON();
		m_ActionType |= Player_ActionConText::ActionType::LockONType;
	}
	else if (m_ActionType & Player_ActionConText::ActionType::LockONType)
	{
		m_ActionType ^= Player_ActionConText::ActionType::LockONType;
	}
}

void Player_ActionState::Idol()
{
	std::shared_ptr<Player_Idol> idol = std::make_shared<Player_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	idol->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetNextState(idol);
}

void Player_ActionState::Run()
{
	std::shared_ptr<Player_Run> run = std::make_shared<Player_Run>();
	if (m_target.expired())return;
	run->SetTarget(m_target.lock());
	run->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetNextState(run);
}

void Player_ActionState::Attack()
{
	std::shared_ptr<Player_Attack> attack = std::make_shared<Player_Attack>();
	if (m_target.expired())return;
	attack->SetTarget(m_target.lock());
	attack->SetObjectManager(m_ObjManager.lock());
	attack->AttackDirCheck();
	m_target.lock()->SetNextState(attack);
}

void Player_ActionState::Guard()
{
	std::shared_ptr<Player_Guard> guard = std::make_shared<Player_Guard>();
	if (m_target.expired())return;
	guard->SetTarget(m_target.lock());
	guard->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetNextState(guard);
}

void Player_ActionState::GuardReaction()
{
	if (m_target.expired())return;
	int sta = m_target.lock()->GetParam().Sm;
	if (sta <= 0)return;
	sta -= 10;
	if (sta <= 0)sta = 0;
	std::shared_ptr<Player_GuardReaction> guardReaction = std::make_shared<Player_GuardReaction>();
	m_target.lock()->SetStamina(sta);
	m_target.lock()->SetStaminaRecoveryTime(60);
	guardReaction->SetTarget(m_target.lock());
	guardReaction->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetNextState(guardReaction);
}

void Player_ActionState::Parry(std::shared_ptr<EnemyBase> _enemy)
{
	std::shared_ptr<Player_Parry> parry = std::make_shared<Player_Parry>();
	if (m_target.expired())return;
	parry->SetTarget(m_target.lock());
	parry->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetParryID(_enemy->GetID());
	m_target.lock()->SetNextState(parry);
	_enemy->GetConText()->Stumble();
}

void Player_ActionState::Counter()
{
	std::shared_ptr<Player_Counter> counter = std::make_shared<Player_Counter>();
	if (m_target.expired())return;
	counter->SetTarget(m_target.lock());
	counter->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetNextState(counter);
}

void Player_ActionState::Roll()
{
	if (m_target.expired())return;
	int sta = m_target.lock()->GetParam().Sm;
	if (sta <= 0)return;
	sta -= 10;
	if (sta <= 0)sta = 0;
	std::shared_ptr<Player_Roll> roll = std::make_shared<Player_Roll>();
	m_target.lock()->SetStamina(sta);
	m_target.lock()->SetStaminaRecoveryTime(60);
	roll->SetTarget(m_target.lock());
	roll->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetNextState(roll);
}

void Player_ActionState::Hit(int _damage, std::shared_ptr<EnemyBase> _enemy)
{
	std::shared_ptr<Player_Hit> hit = std::make_shared<Player_Hit>();
	if (m_target.expired())return;
	hit->SetTarget(m_target.lock());
	hit->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->Hit(_damage);
	m_target.lock()->SetNextState(hit);
}
