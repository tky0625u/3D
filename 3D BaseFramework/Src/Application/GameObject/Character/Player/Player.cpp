#include "Player.h"
#include"../../../Scene/SceneManager.h"
#include"../../StageManager.h"
#include"../../ObjectManager.h"
#include"../../Camera/GameCamera/GameCamera.h"
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
		m_actionType = m_NextActionType;
		m_NextState.reset();
	}
	switch (m_flow)
	{
	case Flow::EnterType:
		m_state->Enter(this);
		break;
	case Flow::UpdateType:
		m_state->Update(this);
		break;
	case Flow::ExitType:
		m_state->Exit(this);
		break;
	}
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

	std::shared_ptr<Idol> idol = std::make_shared<Idol>();
	m_state = idol;
	m_actionType = Action::IdolType;
	m_flow = CharacterBase::Flow::UpdateType;

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

void Player::LockONCheck()
{
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

void Player::KeyCheck()
{
	//移動
	if (GetAsyncKeyState('W') & 0x8000 | GetAsyncKeyState('A') & 0x8000 | GetAsyncKeyState('S') & 0x8000 | GetAsyncKeyState('D') & 0x8000)
	{
		m_keyType |= Player::KeyType::MoveKey;
	}
	else if (m_keyType & Player::KeyType::MoveKey)
	{
		m_keyType ^= Player::KeyType::MoveKey;
	}

	//攻撃
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_keyType |= Player::KeyType::AttackKey;
	}
	else if (m_keyType & Player::KeyType::AttackKey)
	{
		m_keyType ^= Player::KeyType::AttackKey;
	}

	//防御
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_keyType |= Player::KeyType::GuardKey;
	}
	else if (m_keyType & Player::KeyType::GuardKey)
	{
		m_keyType ^= Player::KeyType::GuardKey;
	}

	//回避
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_keyType |= Player::KeyType::RollKey;
	}
	else if (m_keyType & Player::KeyType::RollKey)
	{
		m_keyType ^= Player::KeyType::RollKey;
	}

	//ロックオン
	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
	{
		if (!(m_BeforeKeyType & Player::KeyType::LockONKey))LockON();
		m_keyType |= Player::KeyType::LockONKey;
	}
	else if (m_keyType & Player::KeyType::LockONKey)
	{
		m_keyType ^= Player::KeyType::LockONKey;
	}
}

// Idol============================================================================================
void Player::Idol::Enter(Player* owner)
{
}

void Player::Idol::Update(Player* owner)
{
	if (owner->m_anime != "Idol")
	{
		owner->SetAnime("Idol", true, 1.0f);
	}

	ChangeState(owner);
}

void Player::Idol::Exit(Player* owner)
{
}

void Player::Idol::ChangeState(Player* owner)
{
	if (owner->m_keyType & Player::KeyType::MoveKey)
	{
		std::shared_ptr<Run> _run = std::make_shared<Run>();
		owner->m_NextState = _run;
		owner->m_NextActionType = Player::Action::RunType;
		owner->m_flow = CharacterBase::Flow::EnterType;
		return;
	}
	else if (owner->m_keyType & Player::KeyType::AttackKey && !(owner->m_BeforeKeyType & Player::KeyType::AttackKey))
	{
		std::shared_ptr<Attack> _attack = std::make_shared<Attack>();
		owner->m_NextState = _attack;
		owner->m_NextActionType = Player::Action::AttackType;
		owner->m_flow = CharacterBase::Flow::EnterType;
		return;
	}
	else if (owner->m_keyType & Player::KeyType::GuardKey)
	{
		std::shared_ptr<Guard> _guard = std::make_shared<Guard>();
		owner->m_NextState = _guard;
		owner->m_NextActionType = Player::Action::GuardType;
		owner->m_flow = CharacterBase::Flow::EnterType;
		return;
	}
	else if (owner->m_keyType & Player::KeyType::RollKey && !(owner->m_BeforeKeyType & Player::KeyType::RollKey))
	{
		std::shared_ptr<Roll> _roll = std::make_shared<Roll>();
		owner->m_NextState = _roll;
		owner->m_NextActionType = Player::Action::RollType;
		owner->m_flow = CharacterBase::Flow::EnterType;
		return;
	}
}
//=================================================================================================