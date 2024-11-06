#include "Player_Counter.h"
#include"../../../../../Scene/SceneManager.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"
#include"../Player_ActionState.h"
#include"../../../../Weapon/Sword/Sword.h"
#include"../../../../ObjectManager.h"
#include"../../../Enemy/EnemyBase.h"
#include"../../../Action/Enemy/Enemy_ConText.h"
#include"../../../../Camera/GameCamera/GameCamera.h"
#include"../../../../Camera/GameCamera/GameCamera_ConText.h"
#include"../../../../Camera/GameCamera/GameCamera_State.h"

void Player_Counter::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "ParryingToCounter")
		{
			m_target.lock()->SetAnime("ParryingToCounter", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::CenterType;
			if (m_target.lock()->GetSword().expired() == false)
			{
				m_target.lock()->GetSword().lock()->MakeTraject();
			}
			return;
		}
	}
}

void Player_Counter::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Counter")
		{
			m_target.lock()->SetAnime("Counter", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::EndType;
			return;
		}

		if (m_target.lock()->GetSword().expired() == false)
		{
			m_target.lock()->GetSword().lock()->SetTrajectMat();
		}
		CounterDamage();
	}
}

void Player_Counter::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "CounterToIdol")
		{
			m_target.lock()->SetAnime("CounterToIdol", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_target.lock()->GetConText()->Idol();
			return;
		}
	}
}

void Player_Counter::ChangeAction()
{
	if (m_flow != Flow::EndType)return;

	if (m_ActionType & Player_ActionConText::ActionType::MoveType)
	{
		m_target.lock()->GetConText()->Run();
	}
	else if (m_ActionType & Player_ActionConText::ActionType::AttackType && !(m_target.lock()->GetConText()->GetBeforeActionType() & Player_ActionConText::ActionType::AttackType))
	{
		m_target.lock()->GetConText()->Attack();
	}
	else if (m_ActionType & Player_ActionConText::ActionType::GuardType)
	{
		m_target.lock()->GetConText()->Guard();
	}
	else if (m_ActionType & Player_ActionConText::ActionType::RollType && !(m_target.lock()->GetConText()->GetBeforeActionType() & Player_ActionConText::ActionType::RollType))
	{
		m_target.lock()->GetConText()->Roll();
	}
}

void Player_Counter::CounterDamage()
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
	std::shared_ptr<EnemyBase> hitEnemy;

	for (auto& sphere : SceneManager::Instance().GetEnemyList())
	{
		if (sphere->GetID() != m_target.lock()->GetParryID())continue;

		for (int i = 0; i < sphereInfoList.size(); ++i)
		{
			if (sphere->Intersects(sphereInfoList[i], &retSphereList))
			{
				hitEnemy = sphere;
			}
		}
	}

	for (auto& ret : retSphereList)
	{
		if (hitEnemy->GetParam().Hp > 0 && hitEnemy->GetActionType() != EnemyBase::Action::AppealType && hitEnemy->GetinviTime() == 0)
		{
			m_target.lock()->GetCamera().lock()->GetConText()->GetState()->SetShakeFlg(true);
			hitEnemy->Hit(m_target.lock()->GetParam().Atk);
			hitEnemy->GetConText()->Hit(m_target.lock()->GetParam().Atk);
			hitEnemy->SetInviTime(m_target.lock()->GetinviTime());
			KdEffekseerManager::GetInstance().Play("hit_eff.efkefc", ret.m_hitPos, 0.4f, 0.8f, false);
			KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/刀で斬る2.WAV", 0.05f, false);
		}
	}
}
