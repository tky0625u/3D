#include "Player.h"
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
	if (m_ObjManager.lock()->GetCamera().lock()->GetFixedFlg())return;

	if (m_NextState != nullptr)
	{
		m_state = m_NextState;
		m_NextState.reset();
	}
	m_state.lock()->Update();

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		if (!m_keyFlg)
		{
			if (m_lockOnFlg)m_lockOnFlg = false;
			else
			{
				LockON();
			}
			m_keyFlg = true;
		}
	}
	else
	{
		m_keyFlg = false;
	}

	if (m_lockOnFlg)
	{
		if (m_lockONTarget.lock()->GetParam().Hp <= 0)
		{
			LockON();
		}
		else
		{
			m_camera.lock()->SetLockONTarget(m_lockONTarget.lock());
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
	if (m_ObjManager.lock()->GetEnemyList().size() == 0 &&
		!m_ObjManager.lock()->GetCamera().lock()->GetFixedFlg())
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
		//m_pDebugWire->AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range, color);

		std::list<KdCollider::CollisionResult> retRayList;
		for (auto& ret : m_ObjManager.lock()->GetObjectList())
		{
			if (ret.expired())continue;
			if (ret.lock()->Intersects(rayInfo, &retRayList))
			{
				m_ObjManager.lock()->Clear();
			}
		}
	}

	CharacterBase::PostUpdate();

	m_camera.lock()->SlowChange(m_ObjManager.lock()->GetSlowFlg());

	if (m_ObjManager.lock()->GetSlowFlg())
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
	m_state.lock()->SetObjectManager(m_ObjManager.lock());

	m_ui = std::make_shared<Player_UI_Manager>();
	m_ui->SetPlayer(shared_from_this());
	m_ui->SetObjectManager(m_ObjManager.lock());
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

void Player::LockON()
{
	float Dist = 0.0f;
	bool HitFlg = false;
	int listNum = 0;

	for (int e = 0; e < m_ObjManager.lock()->GetEnemyList().size(); ++e)
	{
		if (!m_ObjManager.lock()->GetEnemyList()[e].expired() &&
			m_ObjManager.lock()->GetEnemyList()[e].lock()->GetParam().Hp > 0)
		{
			if (!HitFlg)
			{
				float d = (m_ObjManager.lock()->GetEnemyList()[e].lock()->GetPos() - m_pos).Length();
				Dist = d;
				HitFlg = true;
				listNum = e;
			}
			else
			{
				float d = (m_ObjManager.lock()->GetEnemyList()[e].lock()->GetPos() - m_pos).Length();
				if (d < Dist)
				{
					Dist = d;
					listNum = e;
				}
			}
		}
	}

	if (HitFlg == true)
	{
		m_lockONTarget = m_ObjManager.lock()->GetEnemyList()[listNum];
		m_lockOnFlg = true;
	}
	else
	{
		m_lockOnFlg = false;
	}
}
