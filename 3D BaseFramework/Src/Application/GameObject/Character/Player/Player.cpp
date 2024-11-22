#include "Player.h"
#include"../../../Scene/SceneManager.h"
#include"../../StageManager.h"
#include"../../ObjectManager.h"
#include"../../Camera/GameCamera/GameCamera.h"
#include"../Enemy/EnemyBase.h"
#include"../BulletBase/BulletBase.h"
#include"../../Weapon/Sword/Sword.h"
#include"../../Weapon/Shield/Shield.h"

#include"../Action/Enemy/Enemy_ConText.h"

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
	m_state->StateUpdate(this);
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
	if (m_LockONFlg && !m_LockONTarget.expired() && m_LockONTarget.lock()->GetParam().Hp > 0)
	{
		m_LockONFlg = false;
		m_LockONTarget.lock()->SetLockONFlg(false);
		return;
	}

	bool HitFlg = false;
	int listNum = 0;
	Math::Vector3 _2dPos = Math::Vector3::Zero;

	for (int e = 0; e < SceneManager::Instance().GetEnemyList().size(); ++e)
	{
		if (SceneManager::Instance().GetEnemyList()[e]->GetParam().Hp > 0)
		{
			if (!HitFlg)
			{
				m_camera.lock()->GetCamera()->ConvertWorldToScreenDetail(SceneManager::Instance().GetEnemyList()[e]->GetPos(), _2dPos);
				HitFlg = true;
				listNum = e;
			}
			else
			{
				Math::Vector3 _tmp2dPos = Math::Vector3::Zero;
				m_camera.lock()->GetCamera()->ConvertWorldToScreenDetail(SceneManager::Instance().GetEnemyList()[e]->GetPos(), _tmp2dPos);
				if (_2dPos.z < 0.0f && _tmp2dPos.z >= 0.0f)
				{
					_2dPos = _tmp2dPos;
					listNum = e;
				}
				else if ((_2dPos.z >= 0.0f && _tmp2dPos.z >= 0.0f) || (_2dPos.z < 0.0f && _tmp2dPos.z < 0.0f))
				{
					if (_2dPos.Length() > _tmp2dPos.Length())
					{
						_2dPos = _tmp2dPos;
						listNum = e;
					}
				}
			}
		}
	}

	if (HitFlg == true)
	{
		SceneManager::Instance().GetEnemyList()[listNum]->SetLockONFlg(true);
		m_LockONTarget = SceneManager::Instance().GetEnemyList()[listNum];
		m_LockONFlg = true;
	}
	else
	{
		m_LockONFlg = false;
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

void Player::IdolChange()
{
	std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
	m_NextState = _idol;
	m_NextActionType = Action::IdolType;
	m_flow = CharacterBase::Flow::UpdateType;
}

void Player::TeleportChange()
{
	std::shared_ptr<Teleport> _teleport = std::make_shared<Teleport>();
	m_NextState = _teleport;
	m_NextActionType = Action::TeleportType;
	m_flow = CharacterBase::Flow::EnterType;
}

void Player::StateBase::StateUpdate(Player* owner)
{
	switch (owner->m_flow)
	{
	case Player::Flow::EnterType:
		Enter(owner);
		break;
	case Player::Flow::UpdateType:
		Update(owner);
		break;
	case Player::Flow::ExitType:
		Exit(owner);
		break;
	}

	if (owner->m_camera.lock()->GetCameraType() != GameCamera::CameraType::PlayerType)return;

	owner->KeyCheck();
	if (owner->m_LockONFlg)
	{
		if (!owner->m_LockONTarget.expired())
		{
			if (owner->m_LockONTarget.lock()->GetParam().Hp <= 0)
			{
				owner->m_LockONTarget.lock()->SetLockONFlg(false);
				owner->LockONCheck();
			}
		}
	}
	ChangeState(owner);
	owner->m_BeforeKeyType = owner->m_keyType;
}

void Player::StateBase::Damage(Player* owner, int _damage, std::shared_ptr<EnemyBase> _enemy)
{
	owner->m_param.Hp -= _damage;
	if (owner->m_param.Hp <= 0)
	{
		// Crushing
		std::shared_ptr<Crushing> _crush = std::make_shared<Crushing>();
		owner->m_NextState = _crush;
		owner->m_NextActionType = Player::Action::CrushingType;
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
		return;
	}
	// Hit
	std::shared_ptr<Hit> _hit = std::make_shared<Hit>();
	owner->m_NextState = _hit;
	owner->m_NextActionType = Player::Action::HitType;
	owner->m_flow = CharacterBase::Flow::EnterType;
	return;
}

void Player::StateBase::Damage(Player* owner, int _damage, std::shared_ptr<BulletBase> _bullet)
{
	_bullet->SetCrush(true);
	owner->m_param.Hp -= _damage;
	if (owner->m_param.Hp <= 0)
	{
		// Crushing
		std::shared_ptr<Crushing> _crush = std::make_shared<Crushing>();
		owner->m_NextState = _crush;
		owner->m_NextActionType = Player::Action::CrushingType;
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
		return;
	}
	// Hit
	std::shared_ptr<Hit> _hit = std::make_shared<Hit>();
	owner->m_NextState = _hit;
	owner->m_NextActionType = Player::Action::HitType;
	owner->m_flow = CharacterBase::Flow::EnterType;
	return;
}

void Player::StateBase::AttackHit(Player* owner)
{
	std::vector<KdCollider::SphereInfo> sphereInfoList;
	KdCollider::SphereInfo sphereInfo;

	if (owner->GetSword().expired() == false)
	{
		sphereInfo.m_sphere.Center = owner->GetSword().lock()->GetModelTop().Translation();
		sphereInfoList.push_back(sphereInfo);

		sphereInfo.m_sphere.Center = owner->GetSword().lock()->GetModelCenter().Translation();
		sphereInfoList.push_back(sphereInfo);

		sphereInfo.m_sphere.Center = owner->GetSword().lock()->GetModelBottom().Translation();
		sphereInfoList.push_back(sphereInfo);
	}
	else
	{
		sphereInfo.m_sphere.Center = owner->GetSwordMat().Translation();
		sphereInfoList.push_back(sphereInfo);
	}

	for (int i = 0; i < sphereInfoList.size(); ++i)
	{
		sphereInfoList[i].m_sphere.Radius = 0.8f;
		sphereInfoList[i].m_type = KdCollider::TypeDamage;
	}

	std::list<KdCollider::CollisionResult> retSphereList;
	std::shared_ptr<EnemyBase> hitEnemy;

	for (auto& sphere : SceneManager::Instance().GetEnemyList())
	{
		if (owner->m_ParryID != -1 && sphere->GetID() != owner->m_ParryID)continue;

		for (int i = 0; i < sphereInfoList.size(); ++i)
		{
			if (sphere->Intersects(sphereInfoList[i], &retSphereList))
			{
				hitEnemy = sphere;
			}
		}
	}

	for (auto& ret : retSphereList)
	{
		if (hitEnemy->GetParam().Hp > 0 && hitEnemy->GetActionType() != EnemyBase::Action::AppealType && hitEnemy->GetinviTime() == 0)
		{
			owner->m_camera.lock()->GetState()->SetShakeFlg(true);
			hitEnemy->Damage(owner->m_param.Atk);
			hitEnemy->SetInviTime(owner->m_inviTime);
			KdEffekseerManager::GetInstance().Play("Enemy/hit_eff.efkefc", ret.m_hitPos, 1.0f, 0.8f, false);
			KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/刀で斬る2.WAV", 0.05f, false);
		}
	}

	if (owner->m_ParryID != -1)owner->m_ParryID = -1;
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
		if (!(m_BeforeKeyType & Player::KeyType::LockONKey))LockONCheck();
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
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
	}
}
//=================================================================================================


// Run=============================================================================================
void Player::Run::Enter(Player* owner)
{
	if (owner->m_anime != "IdolToRun")
	{
		owner->SetAnime("IdolToRun", false, 3.0f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->m_flow = Player::Flow::UpdateType;
		return;
	}

	if (!(owner->m_keyType & Player::KeyType::MoveKey))
	{
		owner->m_flow = Flow::ExitType;
		return;
	}

	Event(owner);
}

void Player::Run::Update(Player* owner)
{
	if (owner->m_anime != "Run")
	{
		owner->SetAnime("Run", true, 1.0f);
		return;
	}

	if (!(owner->m_keyType & Player::KeyType::MoveKey))
	{
		owner->m_flow = Player::Flow::ExitType;
		return;
	}

	Event(owner);
}

void Player::Run::Exit(Player* owner)
{
	if (owner->m_anime != "RunToIdol")
	{
		owner->SetAnime("RunToIdol", false, 3.0f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = Player::Action::IdolType;
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
	}

	if (owner->m_keyType & Player::MoveKey)
	{
		owner->m_flow = Player::Flow::EnterType;
		return;
	}
}

void Player::Run::Event(Player* owner)
{
	Math::Vector3 dir = Math::Vector3::Zero;
	if (GetAsyncKeyState('W') & 0x8000)
	{
		dir.z = 1.0f;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		dir.z = -1.0f;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		dir.x = -1.0f;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		dir.x = 1.0f;
	}
	if (dir == Math::Vector3::Zero)return;
	owner->CameraTransform(dir);
	owner->Rotate(dir);
	owner->SetMove(dir);
	if (owner->m_flow == Player::Flow::ExitType)owner->m_flow = Player::Flow::UpdateType;
}

void Player::Run::ChangeState(Player* owner)
{
	if (owner->m_keyType & Player::KeyType::AttackKey && !(owner->m_BeforeKeyType & Player::KeyType::AttackKey))
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
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
	}
}
//=================================================================================================


// Attack==========================================================================================
void Player::Attack::Enter(Player* owner)
{
	AttackDirCheck(owner);
	if (owner->GetSword().expired())return;

	if (m_atkNum == 1 || m_atkNum == 2)
	{
		owner->Rotate(m_AttackDir, 360.0f);
	}

	owner->GetSword().lock()->MakeTraject();
	owner->m_flow = Player::Flow::UpdateType;
}

void Player::Attack::Update(Player* owner)
{
	switch (m_atkNum)
	{
	case 1:
		if (owner->m_anime != "Attack1")
		{
			owner->SetAnime("Attack1", false, 1.5f);
			return;
		}
		break;
	case 2:
		if (owner->m_anime != "Attack2")
		{
			owner->SetAnime("Attack2", false, 2.5f);
			return;
		}
		break;
	case 3:
		if (owner->m_anime != "Attack3")
		{
			owner->SetAnime("Attack3", false, 1.5f);
			return;
		}
		break;
	default:
		break;
	}

	Event(owner);

	if (owner->GetSword().expired() == false)
	{
		owner->GetSword().lock()->SetTrajectMat();
	}

	if (owner->GetIsAnimator())
	{
		owner->m_flow = Player::Flow::ExitType;
		return;
	}
}

void Player::Attack::Exit(Player* owner)
{
	switch (m_atkNum)
	{
	case 1:
		if (owner->m_anime != "Attack1ToIdol")
		{
			owner->SetAnime("Attack1ToIdol", false, 1.0f);
			return;
		}
		break;
	case 2:
		if (owner->m_anime != "Attack2ToIdol")
		{
			owner->SetAnime("Attack2ToIdol", false, 1.0f);
			return;
		}
		break;
	case 3:
		if (owner->m_anime != "Attack3ToIdol")
		{
			owner->SetAnime("Attack3ToIdol", false, 1.0f);
			return;
		}
		break;
	default:
		break;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = Player::Action::IdolType;
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
	}
}

void Player::Attack::Event(Player* owner)
{
	switch (m_atkNum)
	{
	case 1:
		Attack1(owner);
		break;
	case 2:
		Attack2(owner);
		break;
	case 3:
		Attack3(owner);
		break;
	default:
		break;
	}

	AttackHit(owner);
}

void Player::Attack::Attack1(Player* owner)
{
	owner->SetMove(m_AttackDir, 0.5f);
}

void Player::Attack::Attack2(Player* owner)
{
	owner->SetMove(m_AttackDir, 1.0f);
}

void Player::Attack::Attack3(Player* owner)
{
	owner->Rotate(m_AttackDir);
	owner->SetMove(m_AttackDir, 1.2f);
}

void Player::Attack::AttackDirCheck(Player* owner)
{
	m_AttackDir = Math::Vector3::Zero;

	if (!owner->m_LockONFlg)
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
					float d = (SceneManager::Instance().GetEnemyList()[e]->GetPos() - owner->m_pos).Length();
					Dist = d;
					HitFlg = true;
					listNum = e;
				}
				else
				{
					float d = (SceneManager::Instance().GetEnemyList()[e]->GetPos() - owner->m_pos).Length();
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
			m_AttackDir = SceneManager::Instance().GetEnemyList()[listNum]->GetPos() - owner->m_pos;
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
				Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->m_angle.y));
				Math::Vector3 nowVec = Math::Vector3::TransformNormal(owner->m_forward, nowRot);
				m_AttackDir = nowVec;
			}
			else
			{
				Math::Matrix cameraRotYMat = Math::Matrix::Identity;
				if (owner->m_camera.expired() == false)
				{
					cameraRotYMat = owner->m_camera.lock()->GetRotationYMatrix();
				}
				m_AttackDir = m_AttackDir.TransformNormal(m_AttackDir, cameraRotYMat);
			}
		}
	}
	else
	{
		m_AttackDir = owner->m_LockONTarget.lock()->GetPos() - owner->m_pos;
		m_AttackDir.y = 0.0f;
	}

	m_AttackDir.Normalize(); //正規化
}

void Player::Attack::ChangeState(Player* owner)
{
	if (owner->m_flow != Player::Flow::ExitType)return;

	if (owner->m_keyType & Player::KeyType::AttackKey && !(owner->m_BeforeKeyType & Player::KeyType::AttackKey))
	{
		m_atkNum++;
		owner->m_flow = Player::Flow::EnterType;
		if (m_atkNum > AttackNUM)m_atkNum = 1;
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
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
	}
}
//=================================================================================================


// Counter==========================================================================================
void Player::Counter::Enter(Player* owner)
{
	if (owner->m_anime != "ParryingToCounter")
	{
		owner->SetAnime("ParryingToCounter", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->m_flow = Player::Flow::UpdateType;
		if (owner->GetSword().expired() == false)
		{
			owner->GetSword().lock()->MakeTraject();
		}
		return;
	}
}

void Player::Counter::Update(Player* owner)
{
	if (owner->m_anime != "Counter")
	{
		owner->SetAnime("Counter", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->m_flow = Player::Flow::ExitType;
		return;
	}

	if (owner->GetSword().expired() == false)
	{
		owner->GetSword().lock()->SetTrajectMat();
	}
	AttackHit(owner);
}

void Player::Counter::Exit(Player* owner)
{
	if (owner->m_anime != "CounterToIdol")
	{
		owner->SetAnime("CounterToIdol", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = Player::Action::IdolType;
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
	}
}

void Player::Counter::ChangeState(Player* owner)
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
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
	}
}
//=================================================================================================


// Roll============================================================================================
void Player::Roll::Enter(Player* owner)
{
}

void Player::Roll::Update(Player* owner)
{
	Event(owner);
	if (owner->m_anime != "Roll")
	{
		owner->SetAnime("Roll", false, 1.2f);
		KdEffekseerManager::GetInstance().Play("Player/Smoke.efkefc", owner->m_pos, 0.5f, 1.0f, false);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->m_flow = Player::Flow::ExitType;
		return;
	}
}

void Player::Roll::Exit(Player* owner)
{
	if (owner->m_anime != "RollToIdol")
	{
		owner->SetAnime("RollToIdol", false, 1.0f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = Player::Action::IdolType;
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
	}
}

void Player::Roll::Event(Player* owner)
{
	Math::Vector3 dir = Math::Vector3::Zero;

	//今の方向
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->m_angle.y));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(owner->m_forward, nowRot);
	dir = nowVec;
	dir.Normalize();

	owner->SetMove(dir, 2.5f);
}

void Player::Roll::ChangeState(Player* owner)
{
}
//=================================================================================================


// Guard===========================================================================================
void Player::Guard::Enter(Player* owner)
{
	if (owner->m_anime != "IdolToGuard")
	{
		owner->SetAnime("IdolToGuard", false, 1.0f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->m_flow = Player::Flow::UpdateType;
		return;
	}

	if (!(owner->m_keyType & Player::KeyType::GuardKey))
	{
		owner->m_flow = Player::Flow::ExitType;
		return;
	}

	m_guardTime++;
}

void Player::Guard::Update(Player* owner)
{
	if (owner->m_anime != "Guard")
	{
		owner->SetAnime("Guard", true, 1.0f);
		return;
	}

	if (!(owner->m_keyType & Player::KeyType::GuardKey))
	{
		owner->m_flow = Player::Flow::ExitType;
		return;
	}

	m_guardTime++;
}

void Player::Guard::Exit(Player* owner)
{
	if (owner->m_anime != "GuardToIdol")
	{
		owner->SetAnime("GuardToIdol", false, 1.0f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = Player::Action::IdolType;
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
	}

	if (owner->m_keyType & Player::KeyType::GuardKey)
	{
		owner->m_flow = Player::Flow::EnterType;
		return;
	}
}

void Player::Guard::Event(Player* owner)
{
}

void Player::Guard::ChangeState(Player* owner)
{
}

void Player::Guard::GuardRotate(Player* owner, Math::Vector3 _pos)
{
	//今の方向
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->m_angle.y));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(owner->m_forward, nowRot);

	//向きたい方向
	Math::Vector3 toVec = _pos - owner->m_pos;
	toVec.Normalize();

	//内角 回転する角を求める
	float d = nowVec.Dot(toVec);
	d = std::clamp(d, -1.0f, 1.0f); //誤差修正

	//回転角度を求める
	float ang = DirectX::XMConvertToDegrees(acos(d));

	//角度変更
	//外角　どっち回転かを求める
	Math::Vector3 c = toVec.Cross(nowVec);
	if (c.y >= 0)
	{
		//右回転
		owner->m_angle.y -= ang;
		if (owner->m_angle.y < 0.0f)
		{
			owner->m_angle.y += 360.0f;
		}
	}
	else
	{
		//左回転
		owner->m_angle.y += ang;
		if (owner->m_angle.y >= 360.0f)
		{
			owner->m_angle.y -= 360.0f;
		}
	}
}
void Player::Guard::Damage(Player* owner, int _damage, std::shared_ptr<EnemyBase> _enemy)
{
	GuardRotate(owner, _enemy->GetPos());

	if (m_guardTime <= 30)
	{
		owner->m_ObjectManager.lock()->SlowChange();
		// Parry
		std::shared_ptr<Parry> _parry = std::make_shared<Parry>();
		owner->m_NextState = _parry;
		owner->m_NextActionType = Player::Action::ParryType;
		owner->m_flow = CharacterBase::Flow::UpdateType;

		owner->m_ParryID = _enemy->GetID();
		_enemy->StumbleChange();
	}
	else
	{
		owner->m_param.Sm -= _damage;
		if (owner->m_param.Sm < 0)
		{
			owner->m_param.Sm = 0;
			StateBase::Damage(owner, _damage, _enemy);
		}
		else
		{
			// GuardReaction
			std::shared_ptr<GuardReaction> _reaction = std::make_shared<GuardReaction>();
			owner->m_NextState = _reaction;
			owner->m_NextActionType = Player::Action::GuardReactionType;
			owner->m_flow = CharacterBase::Flow::UpdateType;
		}
	}
}

void Player::Guard::Damage(Player* owner, int _damage, std::shared_ptr<BulletBase> _bullet)
{
	if (m_guardTime <= 30)
	{
		owner->m_ObjectManager.lock()->SlowChange();
		// Parry
		std::shared_ptr<Parry> _parry = std::make_shared<Parry>();
		owner->m_NextState = _parry;
		owner->m_NextActionType = Player::Action::ParryType;
		owner->m_flow = CharacterBase::Flow::UpdateType;

		Math::Matrix RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->m_angle.y));
		Math::Vector3 _dir = Math::Vector3::TransformNormal(owner->m_forward, RotY);
		_bullet->SetDir(_dir);
		_bullet->SetOwner(owner->m_ObjType);
	}
	else
	{
		owner->m_param.Sm -= _damage;
		if (owner->m_param.Sm < 0)
		{
			owner->m_param.Sm = 0;
			StateBase::Damage(owner, _damage, _bullet);
		}
		else
		{
			// GuardReaction
			std::shared_ptr<GuardReaction> _reaction = std::make_shared<GuardReaction>();
			owner->m_NextState = _reaction;
			owner->m_NextActionType = Player::Action::GuardReactionType;
			owner->m_flow = CharacterBase::Flow::UpdateType;

			_bullet->SetCrush(true);
		}
	}
}
//=================================================================================================


// GuardReaction===================================================================================
void Player::GuardReaction::Enter(Player* owner)
{
}

void Player::GuardReaction::Update(Player* owner)
{
	if (owner->m_anime != "GuardReaction")
	{
		owner->SetAnime("GuardReaction", false, 1.0f);
		KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/ロボットを殴る1.WAV", 0.05f, false);
		KdEffekseerManager::GetInstance().Play("Player/Spark.efkefc",owner->GetShieldMat().Translation(), 1.0f, 1.0f, false);
		return;
	}

	if (owner->GetIsAnimator())
	{
		std::shared_ptr<Guard> _guard = std::make_shared<Guard>();
		owner->m_NextState = _guard;
		owner->m_NextActionType = Player::Action::GuardType;
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
	}
}

void Player::GuardReaction::Exit(Player* owner)
{
}

void Player::GuardReaction::ChangeState(Player* owner)
{
}
//=================================================================================================


// Parry===========================================================================================
void Player::Parry::Enter(Player* owner)
{
}

void Player::Parry::Update(Player* owner)
{
	if (owner->m_anime != "Parrying")
	{
		owner->SetAnime("Parrying", false, 1.0f);
		KdEffekseerManager::GetInstance().Play("Player/hit_hanmado_0409.efkefc", owner->GetShield().lock()->GetParryPoint().Translation(), 1.0f, 0.5f, false);
		KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/maou_se_magic_ice05.WAV", 0.1f, false);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->m_flow = Player::Flow::ExitType;
		return;
	}
}

void Player::Parry::Exit(Player* owner)
{
	if (owner->m_anime != "ParryingToIdol")
	{
		owner->SetAnime("ParryingToIdol", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = Player::Action::IdolType;
		owner->m_flow = CharacterBase::Flow::UpdateType;

		owner->m_ObjectManager.lock()->SlowChange();
		return;
	}
}

void Player::Parry::ChangeState(Player* owner)
{
	if (owner->m_keyType & Player::KeyType::AttackKey && !(owner->m_BeforeKeyType & Player::KeyType::AttackKey))
	{
		if (owner->m_ParryID != -1)
		{
			std::shared_ptr<Counter> _counter = std::make_shared<Counter>();
			owner->m_NextState = _counter;
			owner->m_NextActionType = Player::Action::CounterType;
			owner->m_flow = CharacterBase::Flow::EnterType;
		}
		else 
		{
			std::shared_ptr<Attack> _attack = std::make_shared<Attack>();
			owner->m_NextState = _attack;
			owner->m_NextActionType = Player::Action::AttackType;
			owner->m_flow = CharacterBase::Flow::EnterType;
		}
		owner->m_ObjectManager.lock()->SlowChange();
		return;
	}

	if (owner->m_flow != Player::Flow::ExitType)return;

	if (owner->m_keyType & Player::KeyType::MoveKey)
	{
		std::shared_ptr<Run> _run = std::make_shared<Run>();
		owner->m_NextState = _run;
		owner->m_NextActionType = Player::Action::RunType;
		owner->m_flow = CharacterBase::Flow::EnterType;
		owner->m_ObjectManager.lock()->SlowChange();
		return;
	}
	else if (owner->m_keyType & Player::KeyType::GuardKey)
	{
		std::shared_ptr<Guard> _guard = std::make_shared<Guard>();
		owner->m_NextState = _guard;
		owner->m_NextActionType = Player::Action::GuardType;
		owner->m_flow = CharacterBase::Flow::EnterType;
		owner->m_ObjectManager.lock()->SlowChange();
		return;
	}
	else if (owner->m_keyType & Player::KeyType::RollKey && !(owner->m_BeforeKeyType & Player::KeyType::RollKey))
	{
		std::shared_ptr<Roll> _roll = std::make_shared<Roll>();
		owner->m_NextState = _roll;
		owner->m_NextActionType = Player::Action::RollType;
		owner->m_flow = CharacterBase::Flow::UpdateType;
		owner->m_ObjectManager.lock()->SlowChange();
		return;
	}
}
//=================================================================================================


// Hit=============================================================================================
void Player::Hit::Enter(Player* owner)
{
}

void Player::Hit::Update(Player* owner)
{
	if (owner->m_anime != "Hit")
	{
		owner->SetAnime("Hit", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = Player::Action::IdolType;
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
	}
}

void Player::Hit::Exit(Player* owner)
{
}

void Player::Hit::ChangeState(Player* owner)
{
}
//=================================================================================================


// Crushing========================================================================================
void Player::Crushing::Enter(Player* owner)
{
}

void Player::Crushing::Update(Player* owner)
{
	if (owner->m_anime != "Death")
	{
		owner->SetAnime("Death", false, 1.0f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->Expired();
		owner->m_ObjectManager.lock()->SetGameOverParam();
		return;
	}

	owner->CrushingAction();
}

void Player::Crushing::Exit(Player* owner)
{
}

void Player::Crushing::ChangeState(Player* owner)
{
}
//=================================================================================================


// Teleport========================================================================================
void Player::Teleport::Enter(Player* owner)
{
	if (owner->m_anime != "IdolToTeleport")
	{
		owner->SetAnime("IdolToTeleport", false, 1.0f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->m_flow = Player::Flow::UpdateType;
		return;
	}
}

void Player::Teleport::Update(Player* owner)
{
	if (owner->m_anime != "Teleport")
	{
		owner->SetAnime("Teleport", true, 1.0f);
		owner->m_dissolve = 1.0f;
		m_handle = KdEffekseerManager::GetInstance().Play("Player/LightEnd.efkefc", owner->m_pos, owner->m_size, 1.0f, false).lock()->GetHandle();
		return;
	}


	if (!owner->m_TeleportFlg)
	{
		owner->m_flow = Player::Flow::ExitType;
		return;
	}

	if (!KdEffekseerManager::GetInstance().IsPlaying(m_handle))SceneManager::Instance().BlackAlphaChange(0.01f, true);
}

void Player::Teleport::Exit(Player* owner)
{
	if (owner->m_anime != "TeleportToIdol")
	{
		owner->SetAnime("TeleportToIdol", false, 1.0f);
		owner->m_dissolve = 0.0f;
		KdEffekseerManager::GetInstance().Play("Player/LightEnd.efkefc", owner->m_pos, owner->m_size, 1.0f, false);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = Player::Action::IdolType;
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
	}
}

void Player::Teleport::ChangeState(Player* owner)
{
}
//=================================================================================================