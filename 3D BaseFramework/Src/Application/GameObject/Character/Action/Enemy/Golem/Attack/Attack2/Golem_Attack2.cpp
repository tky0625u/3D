#include "Golem_Attack2.h"
#include"../../../Enemy_ConText.h"
#include"../../../../../Enemy/golem/Golem.h"
#include"../../../../../Player/Player.h"

void Golem_Attack2::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolToAttack2")
		{
			m_target.lock()->SetAnime("IdolToAttack2", false, 0.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::CenterType;
			return;
		}

		std::shared_ptr<Player> _player = m_target.lock()->GetTarget().lock();
		Math::Vector3 _playerPos = _player->GetPos();
		Math::Vector3 _pos = m_target.lock()->GetPos();
		Math::Vector3 _moveDir = _playerPos - _pos;
		float dist = _moveDir.Length();
		_moveDir.Normalize();
		Rotate(_moveDir, m_target.lock());
	}
}

void Golem_Attack2::Center()
{
	if (m_target.expired() == false)
	{
		static Math::Vector3 _dir = Math::Vector3::Zero;
		if (m_target.lock()->GetAnime() != "Attack2")
		{
			m_target.lock()->SetAnime("Attack2", false, 1.5f);
			m_target.lock()->SetJump(5.0f);

			std::shared_ptr<Player> _player = m_target.lock()->GetTarget().lock();
			Math::Vector3 _playerPos = _player->GetPos();
			Math::Vector3 _pos = m_target.lock()->GetPos();
			Math::Vector3 _moveDir = _playerPos - _pos;
			float dist = _moveDir.Length();
			_moveDir.Normalize();
			_dir = _moveDir;

			return;
		}

		if (m_target.lock()->GetPos().y <= 0.0f)
		{
			m_flow = Flow::EndType;
			return;
		}

		Math::Vector3 _pos = m_target.lock()->GetPos();
		_pos.y = 0.0f;
		Math::Vector3 _playerPos = m_target.lock()->GetTarget().lock()->GetPos();
		_playerPos.y = 0.0f;

		if (_pos == _playerPos)m_target.lock()->SetMove(_dir, 4.0f);
	}
}

void Golem_Attack2::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Attack2ToIdol")
		{
			m_target.lock()->SetAnime("Attack2ToIdol", false, 0.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_target.lock()->GetConText()->Idol();
			return;
		}
	}
}

void Golem_Attack2::Hit(int _damage)
{
	if (m_target.expired())return;
	m_target.lock()->Hit(_damage);
	if (m_target.lock()->GetParam().Hp <= 0)
	{
		Crushing();
		return;
	}
}
