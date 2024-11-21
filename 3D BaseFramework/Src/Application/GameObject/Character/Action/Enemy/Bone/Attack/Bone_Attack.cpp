#include"Bone_Attack.h"
#include"../../../../../../Scene/SceneManager.h"
#include"../../Enemy_ConText.h"
#include"../../../../Enemy/Bone/Bone.h"
#include"../../../../Player/Player.h"
#include"../../../../../Weapon/Sword/Sword.h"

void Bone_Attack::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolTOAttack" && m_target.lock()->GetAnime() !="AttackStart")
		{
			m_target.lock()->SetAnime("IdolTOAttack", false, 0.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			KdEffekseerManager::GetInstance().Play("Enemy/BloodLance.efkefc", m_target.lock()->GetAttackStartPointMat().Translation(), 0.3f, 2.0f, false);
			m_flow = Flow::CenterType;
			return;
		}
	}
}

void Bone_Attack::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Attack")
		{
			m_target.lock()->SetAnime("Attack", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::EndType;
			return;
		}

		HitCheck();
		MoveForward();
	}
}
void Bone_Attack::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "AttackToIdol")
		{
			m_target.lock()->SetAnime("AttackToIdol", false, 0.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_target.lock()->GetConText()->Idol();
			return;
		}
	}
}

void Bone_Attack::HitCheck()
{
	if (m_target.expired()                                                                         ||
	    SceneManager::Instance().GetPlayer()->IsExpired()                                          ||
	    SceneManager::Instance().GetPlayer()->GetParam().Hp <= 0                                   ||
	    SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::HitType           ||
	    SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::GuardReactionType ||
	    SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::ParryType         ||
	    SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::CounterType       ||
	    SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::RollType)return;

	std::vector<KdCollider::SphereInfo> sphereInfoList;
	KdCollider::SphereInfo sphereInfo;
	if (m_target.lock()->GetSword().expired() == false)
	{
		sphereInfo.m_sphere.Center = m_target.lock()->GetSword().lock()->GetModelTop().Translation();
		sphereInfoList.push_back(sphereInfo);

		sphereInfo.m_sphere.Center = m_target.lock()->GetSword().lock()->GetModelCenter().Translation();
		sphereInfoList.push_back(sphereInfo);

		sphereInfo.m_sphere.Center = m_target.lock()->GetSword().lock()->GetModelBottom().Translation();
		sphereInfoList.push_back(sphereInfo);
	}
	else
	{
		sphereInfo.m_sphere.Center = m_target.lock()->GetSwordMat().Translation();
		sphereInfoList.push_back(sphereInfo);
	}
	sphereInfo.m_sphere.Radius = 1.0f;
	sphereInfo.m_type = KdCollider::TypeDamage;
	std::list<KdCollider::CollisionResult> _List;

	for (int i = 0; i < sphereInfoList.size(); ++i)
	{
		SceneManager::Instance().GetPlayer()->Intersects(sphereInfo, &_List);
	}

	if (_List.size() != 0)
	{
		for (auto& ret : _List)
		{
			if (SceneManager::Instance().GetPlayer()->GetActionType() != Player::Action::GuardType)
			{
				KdEffekseerManager::GetInstance().Play("Player/hit_effe.efkefc", ret.m_hitPos, 1.0f, 0.8f, false);
			}
			SceneManager::Instance().GetPlayer()->Damage(m_target.lock()->GetParam().Atk, m_target.lock());
		}
	}
}

void Bone_Attack::MoveForward()
{
	if (m_target.expired())return;
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_target.lock()->GetAngle().y));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3(m_target.lock()->GetForward().x, m_target.lock()->GetForward().y, m_target.lock()->GetForward().z), nowRot);
	nowVec.Normalize();

	m_target.lock()->SetMove(nowVec);
}
