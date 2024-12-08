#include "Player.h"
#include"../../../Scene/SceneManager.h"
#include"../../StageManager.h"
#include"../../ObjectManager.h"
#include"../../Camera/GameCamera/GameCamera.h"
#include"../Enemy/EnemyBase.h"
#include"../BulletBase/BulletBase.h"
#include"../../Weapon/Sword/Sword.h"
#include"../../Weapon/Shield/Shield.h"


void Player::Update()
{
	if (m_TeleportFlg && m_actionType != Action::TeleportType)
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
	m_state->StateUpdate(shared_from_this());
}

void Player::PostUpdate()
{
	if (m_ObjectManager.lock()->GetTeleportFlg())
	{
		NextStageCheck();
	}

	CharacterBase::PostUpdate();

	if (m_StaminaRecoveryTime > 0)m_StaminaRecoveryTime--;
	else if (m_param.Sm < m_MaxStamina) { StaminaRecovery(); }
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
	m_sword.lock()->ClearTraject();

	std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
	m_NextState = _idol;
	m_NextActionType = Action::IdolType;
	m_flow = CharacterBase::Flow::UpdateType;
}

void Player::TeleportChange()
{
	m_sword.lock()->ClearTraject();

	std::shared_ptr<Teleport> _teleport = std::make_shared<Teleport>();
	m_NextState = _teleport;
	m_NextActionType = Action::TeleportType;
	m_flow = CharacterBase::Flow::UpdateType;
}

void Player::StateBase::StateUpdate(std::shared_ptr<Player> owner)
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

void Player::StateBase::Damage(std::shared_ptr<Player> owner, int _damage, std::shared_ptr<EnemyBase> _enemy)
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
	owner->m_flow = CharacterBase::Flow::UpdateType;
	return;
}

void Player::StateBase::Damage(std::shared_ptr<Player> owner, int _damage, std::shared_ptr<BulletBase> _bullet)
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
	owner->m_flow = CharacterBase::Flow::UpdateType;
	return;
}

void Player::StateBase::AttackHit(std::shared_ptr<Player> owner)
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
void Player::Idol::Enter(std::shared_ptr<Player> owner)
{
}

void Player::Idol::Update(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("Idol"))
	{
		owner->SetAnime("Idol", true, 1.0f);
	}
}

void Player::Idol::Exit(std::shared_ptr<Player> owner)
{
}

void Player::Idol::ChangeState(std::shared_ptr<Player> owner)
{
	if (owner->m_keyType & Player::KeyType::RollKey && !(owner->m_BeforeKeyType & Player::KeyType::RollKey))
	{
		if (owner->m_param.Sm <= 0)return;

		std::shared_ptr<Roll> _roll = std::make_shared<Roll>();
		owner->m_NextState = _roll;
		owner->m_NextActionType = Player::Action::RollType;
		owner->m_flow = CharacterBase::Flow::EnterType;
		return;
	}
	else if (owner->m_keyType & Player::KeyType::MoveKey)
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

}
//=================================================================================================


// Run=============================================================================================
void Player::Run::Enter(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("IdolToRun"))
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

void Player::Run::Update(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("Run"))
	{
		owner->SetAnime("Run", true, 1.5f);
		return;
	}

	if (!(owner->m_keyType & Player::KeyType::MoveKey))
	{
		owner->m_flow = Player::Flow::ExitType;
		return;
	}

	Event(owner);
}

void Player::Run::Exit(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("RunToIdol"))
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

void Player::Run::Event(std::shared_ptr<Player> owner)
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

void Player::Run::ChangeState(std::shared_ptr<Player> owner)
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
		if (owner->m_param.Sm <= 0)return;

		std::shared_ptr<Roll> _roll = std::make_shared<Roll>();
		owner->m_NextState = _roll;
		owner->m_NextActionType = Player::Action::RollType;
		owner->m_flow = CharacterBase::Flow::EnterType;
		return;
	}
}
//=================================================================================================


// Attack==========================================================================================
void Player::Attack::Enter(std::shared_ptr<Player> owner)
{
	AttackDirCheck(owner);
	if (owner->GetSword().expired())return;

	if (m_atkNum == 1 || m_atkNum == 2)
	{
		owner->Rotate(m_AttackDir, 360.0f);
	}

	switch (m_atkNum)
	{
	case 1:
		m_ChangeTime = 30;
		break;

	case 2:
		m_ChangeTime = 20;
		break;
	case 3:
		m_ChangeTime = 35;
		break;
	default:
		break;
	}

	owner->GetSword().lock()->MakeTraject();
	owner->m_flow = Player::Flow::UpdateType;
}

void Player::Attack::Update(std::shared_ptr<Player> owner)
{
	switch (m_atkNum)
	{
	case 1:
		if (!owner->IsAnimCheck("Attack1"))
		{
			owner->SetAnime("Attack1", false, 1.5f);
			return;
		}
		break;
	case 2:
		if (!owner->IsAnimCheck("Attack2"))
		{
			owner->SetAnime("Attack2", false, 1.5f);
			return;
		}
		break;
	case 3:
		if (!owner->IsAnimCheck("Attack3"))
		{
			owner->SetAnime("Attack3", false, 1.5f);
			return;
		}
		break;
	default:
		break;
	}

	Event(owner);

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = Player::Action::IdolType;
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
	}

	m_ActionFPS++;
}

void Player::Attack::Exit(std::shared_ptr<Player> owner)
{

}

void Player::Attack::Event(std::shared_ptr<Player> owner)
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
}

void Player::Attack::Attack1(std::shared_ptr<Player> owner)
{
	owner->SetMove(m_AttackDir, 0.5f);

	if (m_ActionFPS > 30 && owner->GetSword().lock()->GetTrajectPolygon())
	{
		owner->GetSword().lock()->ClearTraject();
		return;
	}

	if (m_ActionFPS < 15 && m_ActionFPS > 30)return;
	AttackHit(owner);

	if (owner->GetSword().expired())return;
	owner->GetSword().lock()->SetTrajectMat();
}

void Player::Attack::Attack2(std::shared_ptr<Player> owner)
{
	owner->SetMove(m_AttackDir, 1.0f);

	if (m_ActionFPS > 25 && owner->GetSword().lock()->GetTrajectPolygon())
	{
		owner->GetSword().lock()->ClearTraject();
		return;
	}

	if (m_ActionFPS < 10 && m_ActionFPS > 25)return;
	AttackHit(owner);

	if (owner->GetSword().expired())return;
	owner->GetSword().lock()->SetTrajectMat();
}

void Player::Attack::Attack3(std::shared_ptr<Player> owner)
{
	owner->Rotate(m_AttackDir);
	owner->SetMove(m_AttackDir, 1.2f);

	if (m_ActionFPS > 36 && owner->GetSword().lock()->GetTrajectPolygon())
	{
		owner->GetSword().lock()->ClearTraject();
		return;
	}

	if (m_ActionFPS < 12 && m_ActionFPS > 36)return;
	AttackHit(owner);

	if (owner->GetSword().expired())return;
	owner->GetSword().lock()->SetTrajectMat();
}

void Player::Attack::AttackDirCheck(std::shared_ptr<Player> owner)
{
	m_AttackDir = Math::Vector3::Zero;

	if (!owner->m_LockONFlg)
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
			KdCollider::SphereInfo sphereInfo;
			sphereInfo.m_sphere.Center = owner->m_pos;
			sphereInfo.m_sphere.Radius = 5.0f;
			sphereInfo.m_type = KdCollider::Type::TypeBump;

			std::list<KdCollider::CollisionResult> retSphereList;
			for (auto& ret : SceneManager::Instance().GetEnemyList())
			{
				ret->Intersects(sphereInfo, &retSphereList);
			}

			float _maxOverLap     = 0.0f;
			Math::Vector3 _hitDir = Math::Vector3::Zero;
			bool _hitFlg          = false;

			for (auto& sphere : retSphereList)
			{
				if (_maxOverLap < sphere.m_overlapDistance)
				{
					_maxOverLap = sphere.m_overlapDistance;
					_hitDir     = sphere.m_hitDir;
					_hitFlg     = true;
				}
			}

			if (_hitFlg)
			{
				_hitDir.y = 0.0f;
				_hitDir *= -1.0f;
				_hitDir.Normalize();
				m_AttackDir = _hitDir;
			}
			else
			{
				//今の方向
				Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->m_angle.y));
				Math::Vector3 nowVec = Math::Vector3::TransformNormal(owner->m_forward, nowRot);
				m_AttackDir = nowVec;
			}
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
	else
	{
		m_AttackDir = owner->m_LockONTarget.lock()->GetPos() - owner->m_pos;
		m_AttackDir.y = 0.0f;
	}

	m_AttackDir.Normalize(); //正規化
}

void Player::Attack::ChangeState(std::shared_ptr<Player> owner)
{
	if (m_ActionFPS < m_ChangeTime)return;

	if (owner->m_keyType & Player::KeyType::AttackKey && !(owner->m_BeforeKeyType & Player::KeyType::AttackKey))
	{
		owner->GetSword().lock()->ClearTraject();

		m_atkNum++;
		owner->m_flow = Player::Flow::EnterType;
		if (m_atkNum > AttackNUM)m_atkNum = 1;
		m_ActionFPS = 0;
	}
	else if (owner->m_keyType & Player::KeyType::GuardKey)
	{
		owner->GetSword().lock()->ClearTraject();

		std::shared_ptr<Guard> _guard = std::make_shared<Guard>();
		owner->m_NextState = _guard;
		owner->m_NextActionType = Player::Action::GuardType;
		owner->m_flow = CharacterBase::Flow::EnterType;
		return;
	}
	else if (owner->m_keyType & Player::KeyType::RollKey && !(owner->m_BeforeKeyType & Player::KeyType::RollKey))
	{
		if (owner->m_param.Sm <= 0)return;
		owner->GetSword().lock()->ClearTraject();

		std::shared_ptr<Roll> _roll = std::make_shared<Roll>();
		owner->m_NextState = _roll;
		owner->m_NextActionType = Player::Action::RollType;
		owner->m_flow = CharacterBase::Flow::EnterType;
		return;
	}
}

void Player::Attack::Damage(std::shared_ptr<Player> owner, int _damage, std::shared_ptr<EnemyBase> _enemy)
{
	owner->GetSword().lock()->ClearTraject();
	StateBase::Damage(owner, _damage, _enemy);
}

void Player::Attack::Damage(std::shared_ptr<Player> owner, int _damage, std::shared_ptr<BulletBase> _bullet)
{
	owner->GetSword().lock()->ClearTraject();
	StateBase::Damage(owner, _damage, _bullet);
}
//=================================================================================================


// Counter==========================================================================================
void Player::Counter::Enter(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("ParryingToCounter"))
	{
		owner->SetAnime("ParryingToCounter", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{

		for (auto& enemy : SceneManager::Instance().GetEnemyList())
		{
			if (enemy->GetID() != owner->m_ParryID)continue;
			m_CounterEnemy = enemy;
		}

		owner->m_flow = Player::Flow::UpdateType;
		if (owner->GetSword().expired() == false)
		{
			owner->GetSword().lock()->MakeTraject();
		}
		return;
	}
}

void Player::Counter::Update(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("Counter"))
	{
		owner->SetAnime("Counter", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->m_ParryID = -1;
		owner->GetSword().lock()->ClearTraject();

		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = Player::Action::IdolType;
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
	}

	if (m_ActionFPS > 66 && owner->GetSword().lock()->GetTrajectPolygon())
	{
		owner->GetSword().lock()->ClearTraject();
	}

	if (40 <= m_ActionFPS && m_ActionFPS <= 66)
	{
		AttackHit(owner);
		owner->GetSword().lock()->SetTrajectMat();
	}

	CounterMove(owner);

	m_ActionFPS++;
}

void Player::Counter::Exit(std::shared_ptr<Player> owner)
{

}

void Player::Counter::ChangeState(std::shared_ptr<Player> owner)
{

}
void Player::Counter::AttackHit(std::shared_ptr<Player> owner)
{
	if (m_CounterEnemy.lock()->GetParam().Hp <= 0 || m_CounterEnemy.lock()->GetinviTime() > 0)return;

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
		sphereInfoList[i].m_sphere.Radius = 1.0f;
		sphereInfoList[i].m_type = KdCollider::TypeDamage;
	}

	std::list<KdCollider::CollisionResult> retSphereList;

	for (int i = 0; i < sphereInfoList.size(); ++i)
	{
		if (m_CounterEnemy.lock()->Intersects(sphereInfoList[i], &retSphereList))break;
	}

	for (auto& ret : retSphereList)
	{
		owner->m_camera.lock()->GetState()->SetShakeFlg(true);
		m_CounterEnemy.lock()->Damage(owner->m_param.Atk * 5);
		m_CounterEnemy.lock()->SetInviTime(owner->m_inviTime);
		m_handle = KdEffekseerManager::GetInstance().Play("Player/CounterHit.efkefc", ret.m_hitPos, 1.0f, 0.8f, false).lock()->GetHandle();
		KdEffekseerManager::GetInstance().SetRotation(m_handle, Math::Vector3{ 0.0f,1.0f,0.0f }, DirectX::XMConvertToRadians(owner->GetAngle().y));
		KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/刀で斬る2.WAV", 0.05f, false);
	}
}

void Player::Counter::CounterMove(std::shared_ptr<Player> owner)
{
	Math::Vector3 _dir = m_CounterEnemy.lock()->GetPos() - owner->m_pos;
	_dir.Normalize();
	owner->Rotate(_dir);

	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = owner->m_pos;
	sphereInfo.m_sphere.Radius = 6.0f;
	sphereInfo.m_type = KdCollider::TypeBump;

	if (m_CounterEnemy.lock()->Intersects(sphereInfo, nullptr))return;

	owner->SetMove(_dir,10.0f);
}
//=================================================================================================


// Roll============================================================================================
void Player::Roll::Enter(std::shared_ptr<Player> owner)
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
	if (dir != Math::Vector3::Zero)
	{
		owner->CameraTransform(dir);
		owner->Rotate(dir, 360.0f);
	}

	owner->m_param.Sm -= 5 * 10;
	owner->m_StaminaRecoveryTime = 60 * 3;
	if (owner->m_param.Sm <= 0)owner->m_param.Sm = 0;
	owner->m_flow = Player::Flow::UpdateType;
}

void Player::Roll::Update(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("Roll"))
	{
		owner->SetAnime("Roll", false, 1.0f);
		KdEffekseerManager::GetInstance().Play("Player/Smoke.efkefc", owner->m_pos, 0.5f, 1.0f, false);
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

	if (m_ActionFPS < 32)Event(owner);

	m_ActionFPS++;
}

void Player::Roll::Exit(std::shared_ptr<Player> owner)
{

}

void Player::Roll::Event(std::shared_ptr<Player> owner)
{
	Math::Vector3 dir = Math::Vector3::Zero;

	//今の方向
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->m_angle.y));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(owner->m_forward, nowRot);
	dir = nowVec;
	dir.Normalize();

	owner->SetMove(dir, 2.0f);
}

void Player::Roll::ChangeState(std::shared_ptr<Player> owner)
{
}
//=================================================================================================


// Guard===========================================================================================
void Player::Guard::Enter(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("IdolToGuard"))
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

void Player::Guard::Update(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("Guard"))
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

void Player::Guard::Exit(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("GuardToIdol"))
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

void Player::Guard::Event(std::shared_ptr<Player> owner)
{
}

void Player::Guard::ChangeState(std::shared_ptr<Player> owner)
{
}

void Player::Guard::GuardRotate(std::shared_ptr<Player> owner, Math::Vector3 _dir)
{
	//今の方向
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->m_angle.y));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(owner->m_forward, nowRot);

	//内角 回転する角を求める
	float d = nowVec.Dot(_dir);
	d = std::clamp(d, -1.0f, 1.0f); //誤差修正

	//回転角度を求める
	float ang = DirectX::XMConvertToDegrees(acos(d));

	//角度変更
	//外角　どっち回転かを求める
	Math::Vector3 c = _dir.Cross(nowVec);
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
void Player::Guard::Damage(std::shared_ptr<Player> owner, int _damage, std::shared_ptr<EnemyBase> _enemy)
{
	Math::Vector3 toVec = _enemy->GetPos() - owner->m_pos;
	toVec.y = 0.0f;
	toVec.Normalize();
	GuardRotate(owner, toVec);

	if (m_guardTime <= 10)
	{
		if (owner->m_ObjectManager.lock()->GetSlowFlg())return;

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
		if (owner->m_param.Sm <= 0)
		{
			StateBase::Damage(owner, _damage, _enemy);
		}
		else
		{
			owner->m_param.Sm -= _damage * 10;
			if (owner->m_param.Sm < 0)owner->m_param.Sm = 0;
			owner->m_StaminaRecoveryTime = 60 * 3;

			// GuardReaction
			std::shared_ptr<GuardReaction> _reaction = std::make_shared<GuardReaction>();
			owner->m_NextState = _reaction;
			owner->m_NextActionType = Player::Action::GuardReactionType;
			owner->m_flow = CharacterBase::Flow::UpdateType;
		}
	}
}

void Player::Guard::Damage(std::shared_ptr<Player> owner, int _damage, std::shared_ptr<BulletBase> _bullet)
{
	if (m_guardTime <= 10)
	{
		if (owner->m_ObjectManager.lock()->GetSlowFlg())return;

		Math::Vector3 toVec = _bullet->GetOwner().lock()->GetPos() - owner->m_pos;
		toVec.y = 0.0f;
		toVec.Normalize();
		GuardRotate(owner, toVec);

		owner->m_ObjectManager.lock()->SlowChange();
		// Parry
		std::shared_ptr<Parry> _parry = std::make_shared<Parry>();
		owner->m_NextState = _parry;
		owner->m_NextActionType = Player::Action::ParryType;
		owner->m_flow = CharacterBase::Flow::UpdateType;

		Math::Matrix RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->m_angle.y));
		Math::Vector3 _dir = Math::Vector3::TransformNormal(owner->m_forward, RotY);
		_bullet->SetDir(_dir);
		_bullet->SetOwner(owner);
	}
	else
	{
		Math::Vector3 _bulletDir = _bullet->GetDir();
		_bulletDir.y = 0.0f;
		_bulletDir *= -1.0f;
		_bulletDir.Normalize();
		GuardRotate(owner, _bulletDir);

		if (owner->m_param.Sm <= 0)
		{
			StateBase::Damage(owner, _damage, _bullet);
		}
		else
		{
			owner->m_param.Sm -= _damage * 10;
			if (owner->m_param.Sm < 0)owner->m_param.Sm = 0;
			owner->m_StaminaRecoveryTime = 60 * 3;

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
void Player::GuardReaction::Enter(std::shared_ptr<Player> owner)
{
}

void Player::GuardReaction::Update(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("GuardReaction"))
	{
		owner->SetAnime("GuardReaction", false, 1.0f);
		KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/ロボットを殴る1.WAV", 0.05f, false);
		KdEffekseerManager::GetInstance().Play("Player/Spark.efkefc",owner->GetShieldMat().Translation(), 1.0f, 1.0f, false);
		return;
	}

	if (owner->GetIsAnimator())
	{
		std::shared_ptr<Guard> _guard = std::make_shared<Guard>();
		_guard->SetGuardTime(11);
		owner->m_NextState = _guard;
		owner->m_NextActionType = Player::Action::GuardType;
		owner->m_flow = CharacterBase::Flow::UpdateType;
		return;
	}
}

void Player::GuardReaction::Exit(std::shared_ptr<Player> owner)
{
}

void Player::GuardReaction::ChangeState(std::shared_ptr<Player> owner)
{
}
//=================================================================================================


// Parry===========================================================================================
void Player::Parry::Enter(std::shared_ptr<Player> owner)
{
}

void Player::Parry::Update(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("Parrying"))
	{
		owner->SetAnime("Parrying", false, 1.0f);
		KdEffekseerManager::GetInstance().Play("Player/hit_hanmado_0409.efkefc", owner->GetShield().lock()->GetParryPoint().Translation(), 1.0f, 0.5f, false);
		KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/maou_se_magic_ice05.WAV", 0.1f, false);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->m_ParryID = -1;

		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = Player::Action::IdolType;
		owner->m_flow = CharacterBase::Flow::UpdateType;

		owner->m_ObjectManager.lock()->SlowChange();
		return;
	}

	m_ActionFPS++;
}

void Player::Parry::Exit(std::shared_ptr<Player> owner)
{

}

void Player::Parry::ChangeState(std::shared_ptr<Player> owner)
{
	if (m_ActionFPS < 30)return;

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

	if (owner->m_keyType & Player::KeyType::MoveKey)
	{
		owner->m_ParryID = -1;

		std::shared_ptr<Run> _run = std::make_shared<Run>();
		owner->m_NextState = _run;
		owner->m_NextActionType = Player::Action::RunType;
		owner->m_flow = CharacterBase::Flow::EnterType;
		owner->m_ObjectManager.lock()->SlowChange();
		return;
	}
	else if (owner->m_keyType & Player::KeyType::GuardKey)
	{
		owner->m_ParryID = -1;

		std::shared_ptr<Guard> _guard = std::make_shared<Guard>();
		owner->m_NextState = _guard;
		owner->m_NextActionType = Player::Action::GuardType;
		owner->m_flow = CharacterBase::Flow::EnterType;
		owner->m_ObjectManager.lock()->SlowChange();
		return;
	}
	else if (owner->m_keyType & Player::KeyType::RollKey && !(owner->m_BeforeKeyType & Player::KeyType::RollKey))
	{
		if (owner->m_param.Sm <= 0)return;
		owner->m_ParryID = -1;

		std::shared_ptr<Roll> _roll = std::make_shared<Roll>();
		owner->m_NextState = _roll;
		owner->m_NextActionType = Player::Action::RollType;
		owner->m_flow = CharacterBase::Flow::EnterType;
		owner->m_ObjectManager.lock()->SlowChange();
		return;
	}
}
//=================================================================================================


// Hit=============================================================================================
void Player::Hit::Enter(std::shared_ptr<Player> owner)
{
}

void Player::Hit::Update(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("Hit"))
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

void Player::Hit::Exit(std::shared_ptr<Player> owner)
{
}

void Player::Hit::ChangeState(std::shared_ptr<Player> owner)
{
}
//=================================================================================================


// Crushing========================================================================================
void Player::Crushing::Enter(std::shared_ptr<Player> owner)
{
}

void Player::Crushing::Update(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("Death"))
	{
		owner->SetAnime("Death", false, 1.0f);
		return;
	}

	if (owner->GetDissolve() == 1.0f)
	{
		owner->Expired();
		return;
	}

	owner->CrushingAction();
}

void Player::Crushing::Exit(std::shared_ptr<Player> owner)
{
}

void Player::Crushing::ChangeState(std::shared_ptr<Player> owner)
{
}
//=================================================================================================


// Teleport========================================================================================
void Player::Teleport::Enter(std::shared_ptr<Player> owner)
{

}

void Player::Teleport::Update(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("Teleport"))
	{
		owner->SetAnime("Teleport", true, 1.0f);
		return;
	}


	if (!owner->m_TeleportFlg)
	{
		owner->m_flow = Player::Flow::ExitType;
		return;
	}

	if (m_ActionFPS == 38)
	{
		owner->m_dissolve = 1.0f;
		m_handle = KdEffekseerManager::GetInstance().Play("Player/LightEnd.efkefc", owner->m_pos, owner->m_size, 1.0f, false).lock()->GetHandle();
	}

	if (m_ActionFPS >= 38 && !KdEffekseerManager::GetInstance().IsPlaying(m_handle))SceneManager::Instance().BlackAlphaChange(0.01f, true);

	m_ActionFPS++;
}

void Player::Teleport::Exit(std::shared_ptr<Player> owner)
{
	if (!owner->IsAnimCheck("TeleportToIdol"))
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

void Player::Teleport::ChangeState(std::shared_ptr<Player> owner)
{
}
//=================================================================================================