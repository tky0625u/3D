﻿#include"Bone_Attack.h"
#include"../../Enemy_ConText.h"
#include"../../../../Enemy/Bone/Bone.h"
#include"../../../../Player/Player.h"
#include"../../../Player/Player_ConText.h"
#include"../../../../../Weapon/Sword/Sword.h"

void Bone_Attack::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolTOAttack")
		{
			m_target.lock()->SetAnime("IdolTOAttack", false, 0.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
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

		Attack();
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
			//AttackCheck(m_atkFlg);
			if (m_atkFlg) { m_flow = Flow::StartType; }
			else { m_target.lock()->GetConText()->Idol(); }
			return;
		}
	}
}

void Bone_Attack::Attack()
{
	if (m_target.expired())return;
	if (m_target.lock()->GetPlayer().expired())return;

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
	sphereInfo.m_sphere.Radius = 0.8f;
	sphereInfo.m_type = KdCollider::TypeDamage;

	for (int i = 0; i < sphereInfoList.size(); ++i)
	{
		if (m_target.lock()->GetPlayer().lock()->Intersects(sphereInfo, nullptr))
		{
			m_target.lock()->GetPlayer().lock()->GetConText()->Hit(m_target.lock()->GetParam().Atk, m_target.lock());
		}
	}
}

void Bone_Attack::MoveForward()
{
	if (m_target.expired())return;
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_target.lock()->GetParam().Angle));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3(m_target.lock()->GetParam().ForwardX, m_target.lock()->GetParam().ForwardY, m_target.lock()->GetParam().ForwardZ), nowRot);
	nowVec.Normalize();

	m_target.lock()->SetMove(nowVec);
}