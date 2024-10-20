﻿#include "Player.h"
#include"../../ObjectManager.h"
#include"../../Camera/TPSCamera/TPSCamera.h"
#include"../Action/Player/Idol/Player_Idol.h"
#include"Application/main.h"
#include"../Action/Player/Player_ConText.h"
#include"../../UI/Player/Player_UI_Manager.h"
#include"../../../Scene/SceneManager.h"
#include"../Enemy/EnemyBase.h"

void Player::Action()
{
	if (ObjectManager::Instance().GetCamera().lock()->GetFixedFlg())return;

	if (m_NextState != nullptr)
	{
		m_state = m_NextState;
		m_NextState.reset();
	}
	m_state.lock()->Update();

	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
	{
		if (!m_keyFlg)
		{
			if (m_lockOnFlg)m_lockOnFlg = false;
			else { m_lockOnFlg = true; }
			m_keyFlg = true;
		}
	}
	else
	{
		m_keyFlg = false;
	}

	if (m_lockOnFlg)
	{
		KdCollider::SphereInfo sphereInfo;
		Math::Matrix _mat = m_model->FindWorkNode("spine")->m_worldTransform * (Math::Matrix::CreateTranslation(m_mWorld.Translation()));
		sphereInfo.m_sphere.Center = _mat.Translation();
		sphereInfo.m_sphere.Radius = 100.0f;
		sphereInfo.m_type = KdCollider::TypeBump;

		std::list<KdCollider::CollisionResult>retSphereList;
		std::vector<std::shared_ptr<EnemyBase>> enemyList;
		for (auto& ret : ObjectManager::Instance().GetEnemyList())
		{
			if (ret.expired())continue;

			if (m_id != ret.lock()->GetID())
			{
				if (ret.lock()->Intersects(sphereInfo, &retSphereList))
				{
					enemyList.push_back(ret.lock());
				}
			}
		}

		Math::Vector3 HitDir = Math::Vector3::Zero;
		float maxOverLap = 0.0f;
		bool HitFlg = false;
		int listNum = -1;

		for (auto& sphere : retSphereList)
		{
			if (maxOverLap < sphere.m_overlapDistance)
			{
				maxOverLap = sphere.m_overlapDistance;
				HitDir = sphere.m_hitDir;
				HitFlg = true;
				listNum++;
			}
		}

		if (HitFlg == true)
		{
			HitDir.y = 0.0f;
			HitDir.Normalize();
			m_camera.lock()->LockON(enemyList[listNum]);
			m_lockONTarget = enemyList[listNum];
		}
		else
		{
			m_lockOnFlg = false;
		}
	}
}

void Player::Update()
{
	CharacterBase::Update();
	m_ui->Update();
}

void Player::PostUpdate()
{
	if (ObjectManager::Instance().GetEnemyList().size() == 0)
	{
		KdCollider::RayInfo rayInfo;
		rayInfo.m_pos = m_pos;
		float LitleUP = 1.0f;
		rayInfo.m_pos.y += LitleUP;
		rayInfo.m_dir = Math::Vector3::Down;
		rayInfo.m_range = m_gravity + LitleUP;
		rayInfo.m_type = KdCollider::TypeEvent;

		//デバッグ用
		Math::Color color = { 1,1,1,1 };
		m_pDebugWire->AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range, color);

		std::list<KdCollider::CollisionResult> retRayList;
		for (auto& ret : ObjectManager::Instance().GetObjectList())
		{
			if (ret.lock()->Intersects(rayInfo, &retRayList))
			{
				ObjectManager::Instance().Clear();
			}
		}
	}

	CharacterBase::PostUpdate();

	m_camera.lock()->SlowChange(ObjectManager::Instance().GetSlowFlg());

	if (ObjectManager::Instance().GetSlowFlg())
	{
		if (m_FocusBackRange != 10.0f)m_FocusBackRange = 10.0f;
	}
	else
	{
		if (m_FocusBackRange != 2000.0f)m_FocusBackRange = 2000.0f;
	}

	m_camera.lock()->WorkCamera()->SetFocus(8, 5, m_FocusBackRange);

	if (m_StaminaRecoveryTime > 0)m_StaminaRecoveryTime--;
}

void Player::DrawSprite()
{
	m_ui->DrawSprite();
}

void Player::Init()
{
	CharacterBase::Init();
	m_MaxStamina = m_param.Sm;

	m_model->SetModelData("Asset/Models/Character/Player/Player.gltf");
	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	std::shared_ptr<Player_Idol> idol = std::make_shared<Player_Idol>();
	idol->SetTarget(shared_from_this());

	m_context = std::make_shared<Player_ActionConText>(idol);
	m_state = m_context->GetState();

	m_ui = std::make_shared<Player_UI_Manager>();
	m_ui->SetPlayer(shared_from_this());
	m_ui->Init();

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Player", m_model, KdCollider::TypeBump | KdCollider::TypeDamage | KdCollider::TypeEvent);

	m_ObjType = ObjType::oPlayer;
}

void Player::CrushingAction()
{
	CharacterBase::CrushingAction();
	if (m_dossolve >= 1.0f)m_dossolve = 1.0f;
}
