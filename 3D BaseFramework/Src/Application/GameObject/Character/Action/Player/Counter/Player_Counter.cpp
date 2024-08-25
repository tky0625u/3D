#include "Player_Counter.h"
#include"../../../Player/Player.h"

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

void Player_Counter::KeyCheck(const UINT key, const UINT before)
{
	if (m_end)
	{
		Reset();
		return;
	}

	if (!m_ChangeFlg)return;

	std::shared_ptr<CharacterBase> player = nullptr;
	if (m_target.expired() == false)player = m_target.lock();

	if (key & Player::ActionType::Roll && !(before & Player::ActionType::Roll))
	{
		player->SetNextAction("Roll");
		Reset();
		if (m_target.expired() == false)
		{
			m_target.lock()->InviOFF();
		}
		return;
	}
	if (key & Player::ActionType::Guard && !(before & Player::ActionType::Guard))
	{
		player->SetNextAction("Guard");
		Reset();
		return;
	}
	if (key & Player::ActionType::Attack && !(before & Player::ActionType::Attack))
	{
		player->SetNextAction("Attack");
		Reset();
		return;
	}
	if (key & Player::ActionType::Move && !(before & Player::ActionType::Move))
	{
		player->SetNextAction("Run");
		Reset();
		if (m_target.expired() == false)
		{
			m_target.lock()->InviOFF();
		}
		return;
	}
}
