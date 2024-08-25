#include "Player_Idol.h"
#include"../../../Player/Player.h"

void Player_Idol::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Idol")
		{
			m_target.lock()->SetAnime("Idol", true, 1.0f);
			return;
		}
	}
}

void Player_Idol::Reset()
{
	m_flow = Flow::CenterType;
	m_ChangeFlg = true;
	m_end = false;
}

void Player_Idol::KeyCheck(const UINT key, const UINT before)
{
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
		m_flow = Flow::CenterType;
		m_ChangeFlg = true;
		return;
	}
	if (key & Player::ActionType::Attack && !(before & Player::ActionType::Attack))
	{
		player->SetNextAction("Attack");
		Reset();
		m_flow = Flow::CenterType;
		m_ChangeFlg = true;
		return;
	}
	if (key & Player::ActionType::Move)
	{
		player->SetNextAction("Run");
		Reset();
		m_flow = Flow::CenterType;
		m_ChangeFlg = true;
		return;
	}
}
