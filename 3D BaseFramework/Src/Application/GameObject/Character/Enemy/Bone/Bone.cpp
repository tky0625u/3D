#include "Bone.h"
#include"../EnemyBase.h"
#include"../../Player/Player.h"
#include"../../../../Scene/SceneManager.h"
#include"../../../Weapon/Sword/Sword.h"

void Bone::Action()
{
	if (m_NextState)
	{
		m_state = m_NextState;
		m_actionType = m_NextActionType;
		m_NextState.reset();
	}
	m_state->StateUpdate(this);
}

void Bone::Init()
{
	EnemyBase::Init();

	m_model->SetModelData("Asset/Models/Character/Enemy/Bone/Bone.gltf");

	std::shared_ptr<Appeal> appeal = std::make_shared<Appeal>();
	m_state = appeal;
	m_flow = EnemyBase::Flow::UpdateType;

	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_model, KdCollider::TypeDamage | KdCollider::TypeBump | KdCollider::TypeSight);
}

void Bone::StateBase::Damage(Bone* owner, int _damage)
{
	owner->m_param.Hp -= _damage;
	if (owner->m_param.Hp <= 0)
	{
		owner->m_param.Hp = 0;

		// Crushing
		std::shared_ptr<Crushing> _crush = std::make_shared<Crushing>();
		owner->m_NextState = _crush;
		owner->m_NextActionType = EnemyBase::Action::CrushingType;
		owner->m_flow = EnemyBase::Flow::UpdateType;
		return;
	}

	// Hit
	std::shared_ptr<Hit> _hit = std::make_shared<Hit>();
	owner->m_NextState = _hit;
	owner->m_NextActionType = EnemyBase::Action::HitType;
	owner->m_flow = EnemyBase::Flow::UpdateType;
}


// Appeal==========================================================================================
void Bone::Appeal::Enter(Bone* owner)
{
}

void Bone::Appeal::Update(Bone* owner)
{
	if (owner->GetAnime() != "Appeal")
	{
		owner->SetAnime("Appeal", false, 1.0f);
		m_handle = KdEffekseerManager::GetInstance().Play("Enemy/MagicDrak.efkefc", owner->m_pos, 3.0f, 1.0f, false).lock()->GetHandle();
		return;
	}

	if (owner->GetIsAnimator() && !KdEffekseerManager::GetInstance().IsPlaying(m_handle))
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = EnemyBase::Action::IdolType;
		owner->m_flow = EnemyBase::Flow::UpdateType;
		return;
	}
}

void Bone::Appeal::Exit(Bone* owner)
{
}
//=================================================================================================


// Idol============================================================================================
void Bone::Idol::Enter(Bone* owner)
{
}

void Bone::Idol::Update(Bone* owner)
{
	if (owner->m_anime != "Idol")
	{
		owner->SetAnime("Idol", true, 1.0f);
		return;
	}
}

void Bone::Idol::Exit(Bone* owner)
{
}
//=================================================================================================


// Run=============================================================================================
void Bone::Run::Enter(Bone* owner)
{
	if (owner->m_anime != "IdolToRun")
	{
		owner->SetAnime("IdolToRun", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->m_flow = EnemyBase::Flow::UpdateType;
		return;
	}

	Move(owner);
}

void Bone::Run::Update(Bone* owner)
{
	if (owner->m_anime != "Run")
	{
		owner->SetAnime("Run", true, 0.7f);
		return;
	}

	Move(owner);
}

void Bone::Run::Exit(Bone* owner)
{
	if (owner->m_anime != "RunToIdol")
	{
		owner->SetAnime("RunToIdol", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = EnemyBase::Action::IdolType;
		owner->m_flow = EnemyBase::Flow::UpdateType;
		return;
	}
}

void Bone::Run::Move(Bone* owner)
{
	std::shared_ptr<Player> _player = owner->m_Target.lock();
	Math::Vector3 _playerPos = _player->GetPos();
	Math::Vector3 _pos = owner->m_pos;
	Math::Vector3 _moveDir = _playerPos - _pos;
	float dist = _moveDir.Length();
	_moveDir.Normalize();

	if (dist >= owner->m_AtkRange)owner->SetMove(_moveDir);
	owner->Rotate(_moveDir);
}
//=================================================================================================


// Attack==========================================================================================
void Bone::Attack::Enter(Bone* owner)
{
	if (owner->m_anime != "IdolTOAttack")
	{
		owner->SetAnime("IdolTOAttack", false, 0.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		KdEffekseerManager::GetInstance().Play("Enemy/BloodLance.efkefc", owner->GetAttackStartPointMat().Translation(), 0.3f, 2.0f, false);
		owner->m_flow = EnemyBase::Flow::UpdateType;
		return;
	}
}

void Bone::Attack::Update(Bone* owner)
{
	if (owner->m_anime != "Attack")
	{
		owner->SetAnime("Attack", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->m_flow = EnemyBase::Flow::ExitType;
		return;
	}

	HitCheck(owner);
	MoveForward(owner);
}

void Bone::Attack::Exit(Bone* owner)
{
	if (owner->m_anime != "AttackToIdol")
	{
		owner->SetAnime("AttackToIdol", false, 0.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = EnemyBase::Action::IdolType;
		owner->m_flow = EnemyBase::Flow::UpdateType;
		return;
	}
}

void Bone::Attack::HitCheck(Bone* owner)
{
	if (SceneManager::Instance().GetPlayer()->IsExpired() ||
		SceneManager::Instance().GetPlayer()->GetParam().Hp <= 0 ||
		SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::HitType ||
		SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::GuardReactionType ||
		SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::ParryType ||
		SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::CounterType ||
		SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::RollType)return;

	std::vector<KdCollider::SphereInfo> sphereInfoList;
	KdCollider::SphereInfo sphereInfo;
	if (owner->GetSword().expired() == false)
	{
		sphereInfo.m_sphere.Center = owner->GetSword().lock()->GetModelTop().Translation();
		sphereInfoList.push_back(sphereInfo);

		sphereInfo.m_sphere.Center =owner->GetSword().lock()->GetModelCenter().Translation();
		sphereInfoList.push_back(sphereInfo);

		sphereInfo.m_sphere.Center = owner->GetSword().lock()->GetModelBottom().Translation();
		sphereInfoList.push_back(sphereInfo);
	}
	else
	{
		sphereInfo.m_sphere.Center = owner->GetSwordMat().Translation();
		sphereInfoList.push_back(sphereInfo);
	}
	sphereInfo.m_sphere.Radius = 1.0f;
	sphereInfo.m_type = KdCollider::TypeDamage;
	std::list<KdCollider::CollisionResult> _List;

	for (int i = 0; i < sphereInfoList.size(); ++i)
	{
		SceneManager::Instance().GetPlayer()->Intersects(sphereInfo, &_List);
	}

	if (_List.size() != 0)
	{
		for (auto& ret : _List)
		{
			if (SceneManager::Instance().GetPlayer()->GetActionType() != Player::Action::GuardType)
			{
				KdEffekseerManager::GetInstance().Play("Player/hit_effe.efkefc", ret.m_hitPos, 1.0f, 0.8f, false);
			}
			SceneManager::Instance().GetPlayer()->Damage(owner->m_param.Atk, owner->shared_from_this());
		}
	}
}
void Bone::Attack::MoveForward(Bone* owner)
{
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->m_angle.y));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(owner->m_forward, nowRot);
	nowVec.Normalize();

	owner->SetMove(nowVec);
}
//=================================================================================================


// Stumble=========================================================================================
void Bone::Stumble::Enter(Bone* owner)
{
}

void Bone::Stumble::Update(Bone* owner)
{
	if (owner->m_anime != "Stumble")
	{
		owner->SetAnime("Stumble", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->m_flow = EnemyBase::Flow::ExitType;
		return;
	}

	StumbleAction(owner);
}

void Bone::Stumble::Exit(Bone* owner)
{
	if (owner->m_anime != "StumbleToIdol")
	{
		owner->SetAnime("StumbleToIdol", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = EnemyBase::Action::IdolType;
		owner->m_flow = EnemyBase::Flow::UpdateType;
		return;
	}
}

void Bone::Stumble::StumbleAction(Bone* owner)
{
	Math::Matrix _nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->m_angle.y));
	Math::Vector3 _nowVec = Math::Vector3::TransformNormal(owner->m_forward, _nowRot);
	_nowVec.y = 0.0f;
	_nowVec *= -1.0f;
	_nowVec.Normalize();
	owner->SetMove(_nowVec, 0.1f);
}
//=================================================================================================


// Hit=============================================================================================
void Bone::Hit::Enter(Bone* owner)
{
}

void Bone::Hit::Update(Bone* owner)
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
		owner->m_NextActionType = EnemyBase::Action::IdolType;
		owner->m_flow = EnemyBase::Flow::UpdateType;
		return;
	}
}

void Bone::Hit::Exit(Bone* owner)
{
}
//=================================================================================================


// Crushing========================================================================================
void Bone::Crushing::Enter(Bone* owner)
{
}

void Bone::Crushing::Update(Bone* owner)
{
	if (owner->m_anime != "Death")
	{
		owner->SetAnime("Death", false, 1.0f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->Expired();
		return;
	}

	owner->CrushingAction();
}

void Bone::Crushing::Exit(Bone* owner)
{
}
//=================================================================================================