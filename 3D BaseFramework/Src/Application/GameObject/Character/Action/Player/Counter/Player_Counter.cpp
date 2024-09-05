#include "Player_Counter.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"

#include"../Idol/Player_Idol.h"
#include"../Run/Player_Run.h"
#include"../Attack/Player_Attack.h"
#include"../Roll/Player_Roll.h"
#include"../Guard/Player_Guard.h"

void Player_Counter::Start()
{
	if (m_target.expired() == false)
	{
		m_target.lock()->Attack(m_target.lock()->GetObjType());

		if (m_target.lock()->GetAnime() != "Counter")
		{
			m_target.lock()->SetAnime("Counter", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::EndType;
			m_ChangeFlg = true;
			return;
		}
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

void Player_Counter::Idol(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Idol> idol = std::make_shared<Player_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	context->SetState(idol);
}

void Player_Counter::Run(std::shared_ptr<Player_ActionConText> context, std::weak_ptr<CameraBase> _camera)
{
	std::shared_ptr<Player_Run> run = std::make_shared<Player_Run>();
	if (m_target.expired())return;
	run->SetTarget(m_target.lock());
	run->SetCamera(_camera);
	context->SetState(run);
}

void Player_Counter::Attack(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Attack> attack = std::make_shared<Player_Attack>();
	if (m_target.expired())return;
	attack->SetTarget(m_target.lock());
	context->SetState(attack);
}

void Player_Counter::Guard(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Guard> guard = std::make_shared<Player_Guard>();
	if (m_target.expired())return;
	guard->SetTarget(m_target.lock());
	context->SetState(guard);
}

void Player_Counter::Roll(std::shared_ptr<Player_ActionConText> context, std::weak_ptr<CameraBase> _camera)
{
	std::shared_ptr<Player_Roll> roll = std::make_shared<Player_Roll>();
	if (m_target.expired())return;
	roll->SetTarget(m_target.lock());
	roll->SetCamera(_camera);
	context->SetState(roll);
}
