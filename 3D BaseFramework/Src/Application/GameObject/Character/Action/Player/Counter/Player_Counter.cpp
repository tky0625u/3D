﻿#include "Player_Counter.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"
#include"../Player_ActionState.h"
#include"../../../../Weapon/Sword/Sword.h"

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
		AttackDamage();
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