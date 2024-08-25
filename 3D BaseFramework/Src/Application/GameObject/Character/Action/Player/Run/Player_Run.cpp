#include "Player_Run.h"
#include"../../../Player/Player.h"

void Player_Run::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolToRun")
		{
			m_target.lock()->SetAnime("IdolToRun", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::CenterType;
			m_ChangeFlg = true;
			return;
		}
	}
}

void Player_Run::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Run")
		{
			m_target.lock()->SetAnime("Run", true, 0.7f);
			return;
		}
	}
}

void Player_Run::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "RunToIdol")
		{
			m_target.lock()->SetAnime("RunToIdol", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			if (m_target.expired() == false)m_target.lock()->SetNextAction("Idol");
			m_end = true;
			return;
		}
	}
}

void Player_Run::KeyCheck(UINT key, const UINT before)
{
	std::shared_ptr<CharacterBase> player = nullptr;
	if (m_target.expired() == false)player = m_target.lock();

	if (m_end)
	{
		Reset();
		return;
	}

	if (!(key & Player::ActionType::Move))
	{
		m_flow = Flow::EndType;
	}
	else
	{
		Move(player);
	}

	if (!m_ChangeFlg)return;

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
		Reset();
		return;
	}
}

void Player_Run::Move(std::shared_ptr<CharacterBase>& _player)
{
	Math::Vector3 dir = Math::Vector3::Zero;
	if (GetAsyncKeyState('W') & 0x8000)
	{
		dir.z = 1.0f;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		dir.z = -1.0f;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		dir.x = -1.0f;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		dir.x = 1.0f;
	}
	_player->SetMove(dir, true);
	if (m_flow == Flow::EndType)m_flow = Flow::CenterType;
}
