#include "Enemy_Run.h"
#include"../../../Enemy/EnemyBase.h"
#include"../../../../../Scene/SceneManager.h"
#include"../Enemy_ConText.h"
#include"../../../Player/Player.h"

void Enemy_Run::Init()
{
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Enemy_Run::Start()
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

		Event();
	}
}

void Enemy_Run::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Run")
		{
			m_target.lock()->SetAnime("Run", true, 0.7f);
			return;
		}

		Event();
	}
}

void Enemy_Run::End()
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
				if (!m_atkFlg)m_target.lock()->GetConText()->Idol();
				else{ m_target.lock()->GetConText()->Attack();}
			}
			return;
		}
	}
}

void Enemy_Run::Event()
{
	if (!ChaseCheck())
	{
		m_flow = Flow::EndType;
		return;
	}
	AttackCheck(m_atkFlg);

	if (m_target.lock()->GetPlayer().expired())return;
	std::shared_ptr<Player> _player = m_target.lock()->GetPlayer().lock();
	Math::Vector3 _playerPos = _player->GetPos();
	Math::Vector3 _pos = m_target.lock()->GetPos();
	Math::Vector3 _moveDir = _playerPos - _pos;
	float dist = _moveDir.Length();
	_moveDir.Normalize();

	if (m_target.expired() == false)Rotate(_moveDir, m_target.lock());
	if(dist>=m_target.lock()->GetParam().AtkRange)m_target.lock()->SetMove(_moveDir);
}