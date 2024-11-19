#include "Golem_Attack1.h"
#include"../../../../../../ObjectManager.h"
#include"../../../Enemy_ConText.h"
#include"../../../../../Player/Player.h"
#include"../../../../../Enemy/golem/Golem.h"
#include"../../../../../Enemy/Golem/Bullet/Bullet.h"

void Golem_Attack1::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolToAttack1")
		{
			m_target.lock()->SetAnime("IdolToAttack1", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::CenterType;
			return;
		}
	}
}

void Golem_Attack1::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Attack1")
		{
			m_target.lock()->SetAnime("Attack1", false, 1.5f);
			m_bullet = m_ObjectManager.lock()->SetBulletParam(m_target.lock());
			return;
		}

		if (!m_bullet.expired() && m_bullet.lock()->GetSize() < m_bullet.lock()->GetMaxSize())
		{
			m_bullet.lock()->SetPos(m_target.lock()->GetBulletPoint().Translation());

			Math::Vector3 _dir = m_target.lock()->GetTarget().lock()->GetPos() - m_target.lock()->GetPos();
			_dir.y = 0.0f;
			_dir.Normalize();
			Rotate(_dir, m_target.lock());
		}
		else if (!m_bullet.expired() && m_bullet.lock()->GetSize() >= m_bullet.lock()->GetMaxSize() && m_bullet.lock()->GetDir() == Math::Vector3::Zero)
		{
			Math::Matrix RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_target.lock()->GetAngle().y));
			Math::Vector3 _dir = Math::Vector3::TransformNormal(m_target.lock()->GetForward(), RotY);
			m_bullet.lock()->SetDir(_dir);
		}

		if (m_bullet.expired())
		{
			m_flow = Flow::EndType;
			return;
		}
	}
}

void Golem_Attack1::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Attack1ToIdol")
		{
			m_target.lock()->SetAnime("Attack1ToIdol", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_target.lock()->GetConText()->Idol();
			return;
		}
	}
}

void Golem_Attack1::Hit(int _damage)
{
	if (m_target.expired())return;
	m_target.lock()->Hit(_damage);
	if (m_target.lock()->GetParam().Hp <= 0)
	{
		Crushing();
		return;
	}
}
