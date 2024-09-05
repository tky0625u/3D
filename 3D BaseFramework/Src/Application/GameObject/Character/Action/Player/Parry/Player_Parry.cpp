#include "Player_Parry.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"

#include"../Idol/Player_Idol.h"
#include"../Run/Player_Run.h"
#include"../Counter/Player_Counter.h"
#include"../Roll/Player_Roll.h"
#include"../Guard/Player_Guard.h"

void Player_Parry::Start()
{
	m_ChangeFlg = true;
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Parrying")
		{
			m_target.lock()->SetAnime("Parrying", false, 1.5f);
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
			if (m_target.expired() == false)
			{
				m_target.lock()->SetNextAction("Idol");
				m_target.lock()->InviOFF();
			}
			m_end = true;
			return;
		}
	}
}

void Player_Parry::Idol(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Idol> idol = std::make_shared<Player_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	context->SetState(idol);
}

void Player_Parry::Run(std::shared_ptr<Player_ActionConText> context, std::weak_ptr<CameraBase> _camera)
{
	std::shared_ptr<Player_Run> run = std::make_shared<Player_Run>();
	if (m_target.expired())return;
	run->SetTarget(m_target.lock());
	run->SetCamera(_camera);
	context->SetState(run);
}

void Player_Parry::Attack(std::shared_ptr<Player_ActionConText> context)
{
	Counter(context);
}

void Player_Parry::Guard(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Guard> guard = std::make_shared<Player_Guard>();
	if (m_target.expired())return;
	guard->SetTarget(m_target.lock());
	context->SetState(guard);
}

void Player_Parry::Roll(std::shared_ptr<Player_ActionConText> context, std::weak_ptr<CameraBase> _camera)
{
	std::shared_ptr<Player_Roll> roll = std::make_shared<Player_Roll>();
	if (m_target.expired())return;
	roll->SetTarget(m_target.lock());
	roll->SetCamera(_camera);
	context->SetState(roll);
}

void Player_Parry::Counter(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Counter> counter = std::make_shared<Player_Counter>();
	if (m_target.expired())return;
	counter->SetTarget(m_target.lock());
	context->SetState(counter);
}
