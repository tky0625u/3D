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
			m_target.lock()->SetAnime("IdolToAttack2", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::CenterType;
			return;
		}
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
			_playerPos.y = 0.0f;
			m_playerPos = _playerPos;
			Math::Vector3 _pos = m_target.lock()->GetPos();
			_pos.y = 0.0f;
			Math::Vector3 _moveDir = _playerPos - _pos;
			float dist = _moveDir.Length();
			_moveDir.Normalize();
			_dir = _moveDir;

			return;
		}

		if (m_target.lock()->GetPos().y <= 0.0f)
		{
			AttackDamage();
			KdEffekseerManager::GetInstance().Play("Enemy/Golem/smash.efkefc", m_target.lock()->GetPos(), Math::Vector3{60.0f,15.0f,60.0f}, 1.0f, false);
			m_flow = Flow::EndType;
			return;
		}


		Math::Vector3 _pos = m_target.lock()->GetPos();
		Math::Vector3 _moveDir = m_playerPos - _pos;
		float dist = _moveDir.Length();
		_moveDir.Normalize();
		_dir = _moveDir;
		_pos.y = 0.0f;

		if (_pos != m_playerPos)m_target.lock()->SetMove(_dir, 10.0f);
	}
}

void Golem_Attack2::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Attack2ToIdol")
		{
			m_target.lock()->SetAnime("Attack2ToIdol", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_target.lock()->GetConText()->Idol();
			return;
		}
	}
}

void Golem_Attack2::AttackDamage()
{
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = m_target.lock()->GetPos();
	sphereInfo.m_sphere.Radius = 60.0f;
	sphereInfo.m_type = KdCollider::Type::TypeDamage;

	// デバッグ
	//Math::Color _color = { 1,0,0,1 };
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, _color);

	if (m_target.lock()->GetTarget().lock()->Intersects(sphereInfo, nullptr))
	{
		m_target.lock()->GetTarget().lock()->Damage(5, m_target.lock());
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
