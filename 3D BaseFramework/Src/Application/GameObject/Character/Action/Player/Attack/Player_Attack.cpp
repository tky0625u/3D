#include "Player_Attack.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"

#include"../Idol/Player_Idol.h"
#include"../Run/Player_Run.h"
#include"../Roll/Player_Roll.h"
#include"../Guard/Player_Guard.h"
#include"../Hit/Player_Hit.h"

void Player_Attack::Center()
{
	if (m_target.expired() == false)
	{
		m_target.lock()->Attack(m_target.lock()->GetObjType());
		switch (m_atkNum)
		{
		case 1:
			if (m_target.lock()->GetAnime() != "Attack1")
			{
				m_target.lock()->SetAnime("Attack1", false, 1.5f);
				return;
			}
			break;
		case 2:
			if (m_target.lock()->GetAnime() != "Attack2")
			{
				m_target.lock()->SetAnime("Attack2", false, 1.5f);
				return;
			}
			break;
		case 3:
			if (m_target.lock()->GetAnime() != "Attack3")
			{
				m_target.lock()->SetAnime("Attack3", false, 1.5f);
				return;
			}
			break;
		default:
			break;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::EndType;
			m_ChangeFlg = true;
			return;
		}
	}
}

void Player_Attack::End()
{
	if (m_target.expired() == false)
	{
		switch (m_atkNum)
		{
		case 1:
			if (m_target.lock()->GetAnime() != "Attack1ToIdol")
			{
				m_target.lock()->SetAnime("Attack1ToIdol", false, 1.5f);
				return;
			}
			break;
		case 2:
			if (m_target.lock()->GetAnime() != "Attack2ToIdol")
			{
				m_target.lock()->SetAnime("Attack2ToIdol", false, 1.5f);
				return;
			}
			break;
		case 3:
			if (m_target.lock()->GetAnime() != "Attack3ToIdol")
			{
				m_target.lock()->SetAnime("Attack3ToIdol", false, 1.5f);
				return;
			}
			break;
		default:
			break;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			if (m_target.expired() == false)m_target.lock()->SetNextAction("Idol");
			m_end = true;
			return;
		}
	}
}

void Player_Attack::Init()
{
	m_flow = Flow::CenterType;
}

void Player_Attack::Idol(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Idol> idol = std::make_shared<Player_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	context->SetState(idol);
}

void Player_Attack::Run(std::shared_ptr<Player_ActionConText> context, std::weak_ptr<CameraBase> _camera)
{
	std::shared_ptr<Player_Run> run = std::make_shared<Player_Run>();
	if (m_target.expired())return;
	run->SetTarget(m_target.lock());
	run->SetCamera(_camera);
	context->SetState(run);
}

void Player_Attack::Attack(std::shared_ptr<Player_ActionConText> context)
{
	m_atkNum++;
	if (m_atkNum > 3)m_atkNum = 1;
}

void Player_Attack::Guard(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Guard> guard = std::make_shared<Player_Guard>();
	if (m_target.expired())return;
	guard->SetTarget(m_target.lock());
	context->SetState(guard);
}

void Player_Attack::Roll(std::shared_ptr<Player_ActionConText> context, std::weak_ptr<CameraBase> _camera)
{
	std::shared_ptr<Player_Roll> roll = std::make_shared<Player_Roll>();
	if (m_target.expired())return;
	roll->SetTarget(m_target.lock());
	roll->SetCamera(_camera);
	context->SetState(roll);
}

void Player_Attack::Hit(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Hit> hit = std::make_shared<Player_Hit>();
	if (m_target.expired())return;
	hit->SetTarget(m_target.lock());
	context->SetState(hit);
}

