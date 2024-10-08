#include "Player_Attack.h"
#include"../../../../ObjectManager.h"
#include"../../../Enemy/EnemyBase.h"
#include"../../Enemy/Enemy_ConText.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"
#include"../Player_ActionState.h"
#include"../../../../Weapon/Sword/Sword.h"

void Player_Attack::Start()
{
	if (m_target.expired())return;
	if (m_target.lock()->GetSword().expired())return;

	if (m_atkNum == 1 || m_atkNum == 2)
	{
		if (!AttackRangeCheck())
		{
			if (m_target.expired() == false)Rotate(m_AttackDir, m_target.lock(), 360.0f);
		}
	}

	m_target.lock()->GetSword().lock()->MakeTraject();
	m_flow = Flow::CenterType;
}

void Player_Attack::Center()
{
	if (m_target.expired() == false)
	{
		switch (m_atkNum)
		{
		case 1:
			if (m_target.lock()->GetAnime() != "Attack1")
			{
				m_target.lock()->SetAnime("Attack1", false, 1.5f);
				return;
			}

			Attack1();

			break;
		case 2:
			if (m_target.lock()->GetAnime() != "Attack2")
			{
				m_target.lock()->SetAnime("Attack2", false, 2.5f);
				return;
			}

			Attack2();

			break;
		case 3:
			if (m_target.lock()->GetAnime() != "Attack3")
			{
				m_target.lock()->SetAnime("Attack3", false, 1.5f);
				return;
			}

			Attack3();

			break;
		default:
			break;
		}

		if (m_target.lock()->GetSword().expired() == false)
		{
			m_target.lock()->GetSword().lock()->SetTrajectMat();
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::EndType;
			return;
		}

		AttackDamage();
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
				m_target.lock()->SetAnime("Attack1ToIdol", false, 1.0f);
				return;
			}
			break;
		case 2:
			if (m_target.lock()->GetAnime() != "Attack2ToIdol")
			{
				m_target.lock()->SetAnime("Attack2ToIdol", false, 1.0f);
				return;
			}
			break;
		case 3:
			if (m_target.lock()->GetAnime() != "Attack3ToIdol")
			{
				m_target.lock()->SetAnime("Attack3ToIdol", false, 1.0f);
				return;
			}
			break;
		default:
			break;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_target.lock()->GetConText()->Idol();
			return;
		}
	}
}

void Player_Attack::Attack1()
{
	
}

void Player_Attack::Attack2()
{
	m_target.lock()->SetMove(m_AttackDir, 1.0f);
}

void Player_Attack::Attack3()
{
	if (m_target.expired() == false)Rotate(m_AttackDir, m_target.lock());
	m_target.lock()->SetMove(m_AttackDir, 1.5f);
}

void Player_Attack::AttackDirCheck()
{
	std::shared_ptr<CharacterBase> _player = nullptr;
	if (m_target.expired() == false)_player = m_target.lock();
	m_AttackDir = Math::Vector3::Zero;
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_AttackDir.z = 1.0f;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_AttackDir.z = -1.0f;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_AttackDir.x = -1.0f;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_AttackDir.x = 1.0f;
	}

	if (m_AttackDir == Math::Vector3::Zero)
	{
		//今の方向
		Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_player->GetParam().Angle));
		Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3(_player->GetParam().ForwardX, _player->GetParam().ForwardY, _player->GetParam().ForwardZ), nowRot);
		m_AttackDir = nowVec;
	}
	else
	{
		Math::Matrix cameraRotYMat = Math::Matrix::Identity;
		if (m_target.lock()->GetCamera().expired() == false)
		{
			cameraRotYMat = m_target.lock()->GetCamera().lock()->GetRotationYMatrix();
		}
		m_AttackDir = m_AttackDir.TransformNormal(m_AttackDir, cameraRotYMat);
	}

	m_AttackDir.Normalize(); //正規化
}

void Player_Attack::ChangeAction()
{
	if (m_flow != Flow::EndType)return;

	if (m_ActionType & Player_ActionConText::ActionType::AttackType && !(m_target.lock()->GetConText()->GetBeforeActionType() & Player_ActionConText::ActionType::AttackType))
	{
		m_atkNum++;
		m_flow = Flow::StartType;
		if (m_atkNum > AttackNUM)m_atkNum = 1;
		AttackDirCheck();
	}
	else if (m_ActionType & Player_ActionConText::ActionType::GuardType)
	{
		m_target.lock()->GetConText()->Guard();
	}
	else if (m_ActionType & Player_ActionConText::ActionType::RollType && !(m_target.lock()->GetConText()->GetBeforeActionType() & Player_ActionConText::ActionType::RollType))
	{
		m_target.lock()->GetConText()->Roll();
	}
}

bool Player_Attack::AttackRangeCheck()
{
	if (m_target.expired())return false;

	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = m_target.lock()->GetPos();
	sphere.m_sphere.Radius = m_target.lock()->GetParam().AtkRange;
	sphere.m_type = KdCollider::TypeSight;

	std::shared_ptr<EnemyBase> _target = nullptr;
	std::list<KdCollider::CollisionResult> retSphereList;
	for (auto ret : ObjectManager::Instance().GetEnemyList())
	{
		ret.lock()->Intersects(sphere, &retSphereList);
	}

	bool          isHit      = false;
	Math::Vector3 hitPos     = Math::Vector3::Zero;
	float         maxOverLap = 0.0f;

	for (auto& ret : retSphereList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitPos     = ret.m_hitPos;
			isHit      = true;
		}
	}

	if (isHit)
	{
		Math::Vector3 dir = hitPos - m_target.lock()->GetPos();
		dir.y = 0.0f;
		dir.Normalize();
		Rotate(dir, m_target.lock(), 360.0f);
		Math::Vector3 pos = hitPos - (dir * 4.0f);
		pos.y = 0.0f;
		m_target.lock()->SetPos(pos);
	}

	return isHit;
}
