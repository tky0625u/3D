#include "Player.h"
#include"../../../Scene/SceneManager.h"
#include"../../StageManager.h"
#include"../../ObjectManager.h"
#include"../../Camera/GameCamera/GameCamera.h"
#include"../Action/Player/Idol/Player_Idol.h"
#include"Application/main.h"
#include"../Action/Player/Player_ConText.h"
#include"../Enemy/EnemyBase.h"

void Player::Update()
{
	if (m_TeleportFlg)
	{
		if (GetAsyncKeyState('E') & 0x8000)m_ObjectManager.lock()->NextTeleport();
	}
	CharacterBase::Update();
}

void Player::Action()
{
	if (m_NextState != nullptr)
	{
		m_state = m_NextState;
		m_context->SetState(m_NextState);
		m_actionType = m_NextActionType;
		m_NextState.reset();
	}
	if (!m_state.expired())m_state.lock()->Update();
}

void Player::PostUpdate()
{
	if (m_ObjectManager.lock()->GetTeleportFlg())
	{
		NextStageCheck();
	}

	CharacterBase::PostUpdate();

	if (m_StaminaRecoveryTime > 0)m_StaminaRecoveryTime--;
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

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Player", m_model, KdCollider::TypeBump | KdCollider::TypeDamage | KdCollider::TypeEvent);

	m_ObjType = ObjType::oPlayer;
}

void Player::CrushingAction()
{
	CharacterBase::CrushingAction();
	if (m_dissolve >= 1.0f)
	{
		m_dissolve = 1.0f;
	}
}

void Player::NextStageCheck()
{
	if (SceneManager::Instance().GetEnemyList().size() == 0 &&
		m_camera.lock()->GetCameraType() == GameCamera::CameraType::PlayerType)
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
		for (auto& ret : SceneManager::Instance().GetObjList())
		{
			if (ret->Intersects(rayInfo, &retRayList))
			{
				/*m_StageManager.lock()->NextStage();*/
				m_TeleportFlg = true;
				return;
			}
		}
	}

	m_TeleportFlg = false;
}
