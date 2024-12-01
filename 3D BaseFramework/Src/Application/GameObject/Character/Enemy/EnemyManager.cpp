#include "EnemyManager.h"
#include"EnemyBase.h"
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

	if (m_EnemyAttackList[0].expired() == false && m_EnemyAttackList[0].lock()->GetParam().Hp > 0 && m_EnemyAttackList[0].lock()->GetActionType() == EnemyBase::Action::IdolType)m_EnemyAttackList[0].lock()->AttackChange();

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
		if (enemy->GetActionType() != EnemyBase::Action::RunType &&
			enemy->GetActionType() != EnemyBase::Action::IdolType )continue;

		if (enemy->GetParam().Hp <= 0)continue;

		if (enemy->GetName() == "Bone")
		{
			KdCollider::RayInfo rayInfo;
			Math::Matrix nowRotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(enemy->GetAngle().y));
			Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3{ enemy->GetForward().x,enemy->GetForward().y,enemy->GetForward().z }, nowRotY);
			nowVec.Normalize();
			rayInfo.m_pos = enemy->GetPos();
			rayInfo.m_pos.y = (enemy->GetTarget().lock()->GetEnemyAttackPointMat().Translation().y);
			rayInfo.m_dir = nowVec;
			rayInfo.m_range = enemy->GetParam().AtkRange;
			rayInfo.m_type = KdCollider::Type::TypeBump;

			std::list<KdCollider::CollisionResult> _List;
			if (enemy->GetTarget().lock()->Intersects(rayInfo, &_List))
			{
				enemy->IdolChange();

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
				enemy->RunChange();
			}
		}
		else if (enemy->GetName() == "BoneAlpha")
		{
			Math::Vector3 _playerPos = enemy->GetTarget().lock()->GetPos();
			_playerPos.y = 0.0f;
			Math::Vector3 _enemyPos = enemy->GetPos();
			_enemyPos.y = 0.0f;
			float _dist = (_playerPos - _enemyPos).Length();
			if (_dist > enemy->GetParam().AtkRange)
			{
				if (enemy->GetActionType() != EnemyBase::Action::RunType)enemy->RunChange();
				continue;
			}

			//今の方向
			Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(enemy->GetAngle().y));
			Math::Vector3 nowVec = Math::Vector3::TransformNormal(enemy->GetForward(), nowRot);
			nowVec.y = 0.0f;

			//向きたい方向
			Math::Vector3 toVec = enemy->GetTarget().lock()->GetPos() - enemy->GetPos();
			toVec.y = 0.0f;
			toVec.Normalize();

			//内角 回転する角を求める
			float d = nowVec.Dot(toVec);
			d = std::clamp(d, -1.0f, 1.0f); //誤差修正

			//回転角度を求める
			float ang = DirectX::XMConvertToDegrees(acos(d));

			if (fabs(ang) <= 50.0f)
			{
				enemy->IdolChange();

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
			else if(enemy->GetActionType() != EnemyBase::Action::RunType)
			{
				enemy->RunChange();
			}
		}
		else if (enemy->GetName() == "Golem")
		{
			Math::Vector3 _playerPos = enemy->GetTarget().lock()->GetPos();
			_playerPos.y = 0.0f;
			Math::Vector3 _enemyPos = enemy->GetPos();
			_enemyPos.y = 0.0f;
			float _dist = (_playerPos - _enemyPos).Length();
			if (_dist <= enemy->GetParam().AtkRange)
			{
				enemy->IdolChange();

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
				enemy->RunChange();
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
