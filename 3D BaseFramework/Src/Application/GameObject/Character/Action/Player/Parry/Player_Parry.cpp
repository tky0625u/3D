﻿#include "Player_Parry.h"
#include"../../../../ObjectManager.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"
#include"../Player_ActionState.h"
#include"../../../../Weapon/Shield/Shield.h"

void Player_Parry::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Parrying")
		{
			m_target.lock()->SetAnime("Parrying", false, 1.0f);
			KdEffekseerManager::GetInstance().Play("Player/hit_hanmado_0409.efkefc", m_target.lock()->GetShield().lock()->GetParryPoint().Translation(), 0.5f, 0.5f, false);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::EndType;
			return;
		}
	}
}

void Player_Parry::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "ParryingToIdol")
		{
			m_target.lock()->SetAnime("ParryingToIdol", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_target.lock()->GetConText()->Idol();
			m_ObjManager.lock()->SlowChange();
			return;
		}
	}
}

void Player_Parry::ChangeAction()
{
	//if (m_flow != Flow::EndType)return;

	if (m_ActionType & Player_ActionConText::ActionType::AttackType && !(m_target.lock()->GetConText()->GetBeforeActionType() & Player_ActionConText::ActionType::AttackType))
	{
		m_target.lock()->GetConText()->Counter();
		m_ObjManager.lock()->SlowChange();
		return;
	}

	if (m_flow != Flow::EndType)return;

	if (m_ActionType & Player_ActionConText::ActionType::MoveType)
	{
		m_target.lock()->GetConText()->Run();
		m_ObjManager.lock()->SlowChange();
	}
	else if (m_ActionType & Player_ActionConText::ActionType::GuardType)
	{
		m_target.lock()->GetConText()->Guard();
		m_ObjManager.lock()->SlowChange();
	}
	else if (m_ActionType & Player_ActionConText::ActionType::RollType && !(m_target.lock()->GetConText()->GetBeforeActionType() & Player_ActionConText::ActionType::RollType))
	{
		m_target.lock()->GetConText()->Roll();
		m_ObjManager.lock()->SlowChange();
	}
}