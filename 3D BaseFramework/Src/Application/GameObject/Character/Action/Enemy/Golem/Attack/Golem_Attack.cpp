#include "Golem_Attack.h"
#include"../../../../../ObjectManager.h"
#include"../../Enemy_ConText.h"
#include"../../../../Enemy/golem/Golem.h"
#include"../../../../Player/Player.h"
#include"../../../Player/Player_ConText.h"
#include"../../../../../Weapon/Sword/Sword.h"

void Golem_Attack::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolToAttack" && m_target.lock()->GetAnime() != "AttackStart")
		{
			m_target.lock()->SetAnime("IdolToAttack", false, 0.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			if (m_target.lock()->GetAnime() == "IdolToAttack")
			{
				m_target.lock()->SetAnime("AttackStart", false, 0.5f);
				KdEffekseerManager::GetInstance().Play("Enemy/BloodLance.efkefc", m_target.lock()->GetAttackStartPointMat().Translation(), 0.3f, 2.0f, false);
				return;
			}
			m_flow = Flow::CenterType;
			return;
		}
	}
}

void Golem_Attack::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Attack")
		{
			m_target.lock()->SetAnime("Attack", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::EndType;
			return;
		}

		Attack();
	}
}

void Golem_Attack::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "AttackToIdol")
		{
			m_target.lock()->SetAnime("AttackToIdol", false, 0.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			//AttackCheck(m_atkFlg);
			if (m_atkFlg) { m_flow = Flow::StartType; }
			else { m_target.lock()->GetConText()->Idol(); }
			return;
		}
	}
}

void Golem_Attack::Attack()
{

}