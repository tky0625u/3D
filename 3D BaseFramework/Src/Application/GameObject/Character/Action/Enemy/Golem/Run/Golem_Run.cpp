#include "Golem_Run.h"
#include"../../../../../ObjectManager.h"
#include"../../Enemy_ConText.h"
#include"../../../../Enemy/golem/Golem.h"
#include"../../../../Player/Player.h"

void Golem_Run::Start()
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
			return;
		}

		Chace();
	}
}

void Golem_Run::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Run")
		{
			m_target.lock()->SetAnime("Run", true, 0.7f);
			return;
		}

		Chace();
	}
}

void Golem_Run::End()
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
			if (m_target.expired() == false)
			{
				m_target.lock()->GetConText()->Attack();
			}
			return;
		}
	}
}

void Golem_Run::Chace()
{
	if (m_target.lock()->GetTarget().expired())return;
	std::shared_ptr<Player> _player = m_target.lock()->GetTarget().lock();
	Math::Vector3 _playerPos = _player->GetPos();
	Math::Vector3 _pos = m_target.lock()->GetPos();
	Math::Vector3 _moveDir = _playerPos - _pos;
	float dist = _moveDir.Length();
	_moveDir.Normalize();

	if (dist >= m_target.lock()->GetAtkRange())m_target.lock()->SetMove(_moveDir);
	if (m_target.expired() == false)Rotate(_moveDir, m_target.lock());
	AttackCheck();
}
