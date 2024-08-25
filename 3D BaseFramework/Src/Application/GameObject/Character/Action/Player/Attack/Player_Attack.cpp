#include "Player_Attack.h"
#include"../../../Player/Player.h"

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

void Player_Attack::KeyCheck(const UINT key, const UINT before)
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
		m_ChangeFlg = false;
		m_end = false;
		m_flow = Flow::CenterType;
		m_atkNum++;
		if (m_atkNum > AttackNUM)m_atkNum = 1;
		return;
	}
	if (key & Player::ActionType::Move && !(before & Player::ActionType::Move))
	{
		player->SetNextAction("Run");
		Reset();
		return;
	}
}

void Player_Attack::Init()
{
	m_flow = Flow::CenterType;
}

void Player_Attack::Reset()
{
	m_ChangeFlg = false;
	m_end = false;
	m_atkNum = 1;
	m_flow = Flow::CenterType;
}
