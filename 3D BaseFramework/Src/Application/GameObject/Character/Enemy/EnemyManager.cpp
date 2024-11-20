#include "EnemyManager.h"
#include"EnemyBase.h"
#include"../Action/Enemy/Enemy_ConText.h"
#include"../Player/Player.h"

void EnemyManager::DeleteEnemyList()
{
	auto enemy = m_EnemyList.begin();

	while (enemy != m_EnemyList.end())
	{
		if ((*enemy)->IsExpired())
		{
			enemy = m_EnemyList.erase(enemy);
		}
		else
		{
			++enemy;
		}
	}
}

void EnemyManager::EnemyAttack()
{
	if (m_EnemyAttackList.size() == 0)return;

	for (auto& enemy : m_EnemyList)
	{
		if (enemy->GetActionType() == EnemyBase::Action::AttackType)
		{
			float _dist = 0.0f;
			_dist = (enemy->GetTarget().lock()->GetPos() - enemy->GetPos()).Length();
			if (_dist <= 5.0f)return;
		}
	}

	if (m_EnemyAttackList[0].expired() == false && m_EnemyAttackList[0].lock()->GetActionType() == EnemyBase::Action::IdolType)m_EnemyAttackList[0].lock()->GetConText()->Attack();

	auto enemy = m_EnemyAttackList.begin();
	while (enemy != m_EnemyAttackList.end())
	{
		if (enemy == m_EnemyAttackList.begin() || (*enemy).expired() || (*enemy).lock()->GetActionType() != EnemyBase::Action::IdolType)enemy = m_EnemyAttackList.erase(enemy);
		else { ++enemy; }
	}
}

void EnemyManager::EnemyRun()
{
	for (auto& enemy : m_EnemyList)
	{
		if (enemy->GetActionType() != EnemyBase::Action::RunType  &&
			enemy->GetActionType() != EnemyBase::Action::IdolType)continue;

		if (enemy->GetName() == "Bone")
		{
			KdCollider::RayInfo rayInfo;
			Math::Matrix nowRotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(enemy->GetAngle().y));
			Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3{ enemy->GetForward().x,enemy->GetForward().y,enemy->GetForward().z }, nowRotY);
			nowVec.Normalize();
			rayInfo.m_pos = enemy->GetPos();
			rayInfo.m_pos.y = (enemy->GetTarget().lock()->GetEnemyAttackPointMat().Translation().y);
			rayInfo.m_dir = nowVec;
			rayInfo.m_range = enemy->GetAtkRange();
			rayInfo.m_type = KdCollider::Type::TypeBump;

			std::list<KdCollider::CollisionResult> _List;
			if (enemy->GetTarget().lock()->Intersects(rayInfo, &_List))
			{
				enemy->GetConText()->Idol();

				bool atkFlg = false;
				for (auto& atk : m_EnemyAttackList)
				{
					if (atk.expired())continue;
					if (atk.lock()->GetID() == enemy->GetID())
					{
						atkFlg = true;
						break;
					}
				}
				if (atkFlg)continue;

				m_EnemyAttackList.push_back(enemy);

				continue;
			}
			else if (enemy->GetActionType() != EnemyBase::Action::RunType)
			{
				enemy->GetConText()->Run();
			}
		}
		else if (enemy->GetName() == "Golem")
		{
			KdCollider::SphereInfo sphereInfo;
			sphereInfo.m_sphere.Center = enemy->GetPos();
			sphereInfo.m_sphere.Center.y = (enemy->GetTarget().lock()->GetEnemyAttackPointMat().Translation().y);
			sphereInfo.m_sphere.Radius = enemy->GetAtkRange();
			sphereInfo.m_type = KdCollider::Type::TypeBump;

			std::list<KdCollider::CollisionResult> _List;
			if (enemy->GetTarget().lock()->Intersects(sphereInfo, &_List))
			{
				enemy->GetConText()->Idol();

				bool atkFlg = false;
				for (auto& atk : m_EnemyAttackList)
				{
					if (atk.expired())continue;
					if (atk.lock()->GetID() == enemy->GetID())
					{
						atkFlg = true;
						break;
					}
				}
				if (atkFlg)continue;

				m_EnemyAttackList.push_back(enemy);

				continue;
			}
			else if (enemy->GetActionType() != EnemyBase::Action::RunType)
			{
				enemy->GetConText()->Run();
			}
		}
	}
}

void EnemyManager::PreUpdate()
{
	for (auto& enemy : m_EnemyList) enemy->PreUpdate();
}

void EnemyManager::Update()
{
	EnemyRun();
	EnemyAttack();
	for (auto& enemy : m_EnemyList) enemy->Update();
}

void EnemyManager::PostUpdate()
{
	DeleteEnemyList();
	for (auto& enemy : m_EnemyList) enemy->PostUpdate();
}

void EnemyManager::GenerateDepthMapFromLight()
{
	for (auto& enemy : m_EnemyList) enemy->GenerateDepthMapFromLight();
}

void EnemyManager::PreDraw()
{
	for (auto& enemy : m_EnemyList) enemy->PreDraw();
}

void EnemyManager::DrawLit()
{
	for (auto& enemy : m_EnemyList)
	{
		enemy->DrawLit();
		enemy->DrawOutLine();
	}
}

void EnemyManager::DrawUnLit()
{
	for (auto& enemy : m_EnemyList) enemy->DrawUnLit();
}

void EnemyManager::DrawBright()
{
	for (auto& enemy : m_EnemyList) enemy->DrawBright();
}

void EnemyManager::DrawSprite()
{
	for (auto& enemy : m_EnemyList) enemy->DrawSprite();
}

void EnemyManager::DrawDebug()
{
	for (auto& enemy : m_EnemyList)enemy->DrawDebug();
}

void EnemyManager::Init()
{
	for (auto& enemy : m_EnemyList) enemy->Init();
}
