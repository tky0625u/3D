#include "Player_Attack.h"
#include"../../../../../Scene/SceneManager.h"
#include"../../../../ObjectManager.h"
#include"../../../Enemy/EnemyBase.h"
#include"../../Enemy/Enemy_ConText.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"
#include"../Player_ActionState.h"
#include"../../../../Weapon/Sword/Sword.h"
#include"../../../../Camera/GameCamera/GameCamera.h"
#include"../../../../Camera/GameCamera/GameCamera_ConText.h"
#include"../../../../Camera/GameCamera/GameCamera_State.h"


void Player_Attack::Start()
{
	if (m_target.expired())return;
	if (m_target.lock()->GetSword().expired())return;

	if (m_atkNum == 1 || m_atkNum == 2)
	{
		if (m_target.expired() == false)Rotate(m_AttackDir, m_target.lock(), 360.0f);
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
		AttackDamage();

		if (m_target.lock()->GetSword().expired() == false)
		{
			m_target.lock()->GetSword().lock()->SetTrajectMat();
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::EndType;
			return;
		}

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
	m_target.lock()->SetMove(m_AttackDir, 0.5f);
}

void Player_Attack::Attack2()
{
	m_target.lock()->SetMove(m_AttackDir, 1.0f);
}

void Player_Attack::Attack3()
{
	if (m_target.expired() == false)Rotate(m_AttackDir, m_target.lock());
	m_target.lock()->SetMove(m_AttackDir, 1.2f);
}

void Player_Attack::AttackDirCheck()
{
	std::shared_ptr<Player> _player = nullptr;
	if (m_target.expired() == false)_player = m_target.lock();
	m_AttackDir = Math::Vector3::Zero;
	
	if (!_player->GetConText()->GetLockONFlg())
	{
		float Dist = 0.0f;
		bool HitFlg = false;
		int listNum = 0;

		for (int e = 0; e < SceneManager::Instance().GetEnemyList().size(); ++e)
		{
			if (SceneManager::Instance().GetEnemyList()[e]->GetParam().Hp > 0)
			{
				if (!HitFlg)
				{
					float d = (SceneManager::Instance().GetEnemyList()[e]->GetPos() - m_target.lock()->GetPos()).Length();
					Dist = d;
					HitFlg = true;
					listNum = e;
				}
				else
				{
					float d = (SceneManager::Instance().GetEnemyList()[e]->GetPos() - m_target.lock()->GetPos()).Length();
					if (d < Dist)
					{
						Dist = d;
						listNum = e;
					}
				}
			}
		}

		if (HitFlg)
		{
			m_AttackDir = SceneManager::Instance().GetEnemyList()[listNum]->GetPos() - _player->GetPos();
			m_AttackDir.y = 0.0f;
		}
		else
		{
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
				Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_player->GetAngle().y));
				Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3(_player->GetForward().x, _player->GetForward().y, _player->GetForward().z), nowRot);
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
		}
	}
	else
	{
		m_AttackDir = _player->GetConText()->GetLockONTarget().lock()->GetPos() - _player->GetPos();
		m_AttackDir.y = 0.0f;
	}

	m_AttackDir.Normalize(); //正規化
}

void Player_Attack::AttackDamage()
{
	if (m_target.expired())return;

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

	for (int i = 0; i < sphereInfoList.size(); ++i)
	{
		sphereInfoList[i].m_sphere.Radius = 0.8f;
		sphereInfoList[i].m_type = KdCollider::TypeDamage;
	}

	std::list<KdCollider::CollisionResult> retSphereList;
	std::vector<std::shared_ptr<EnemyBase>> hitEnemyList;

	for (auto& sphere : SceneManager::Instance().GetEnemyList())
	{
		if (sphere->IsExpired())continue;
		if (sphere->GetParam().Hp <= 0)continue;

		for (int i = 0; i < sphereInfoList.size(); ++i)
		{
			if (sphere->Intersects(sphereInfoList[i], &retSphereList))
			{
				hitEnemyList.push_back(sphere);
			}
		}
	}

	int enemy = 0;
	for (auto& ret : retSphereList)
	{
		if (hitEnemyList[enemy]->GetParam().Hp > 0 && hitEnemyList[enemy]->GetActionType() != EnemyBase::Action::AppealType && hitEnemyList[enemy]->GetActionType() != EnemyBase::Action::HitType && hitEnemyList[enemy]->GetinviTime() == 0)
		{
			m_target.lock()->GetCamera().lock()->GetConText()->GetState()->SetShakeFlg(true);
			hitEnemyList[enemy]->Hit(m_target.lock()->GetParam().Atk);
			hitEnemyList[enemy]->GetConText()->Hit(m_target.lock()->GetParam().Atk);
			hitEnemyList[enemy]->SetInviTime(m_target.lock()->GetinviTime());
			KdEffekseerManager::GetInstance().Play("hit_eff.efkefc", ret.m_hitPos, 0.8f, 0.5f, false);
			KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/刀で斬る2.WAV", 0.05f, false);
		}
		enemy++;
	}
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
