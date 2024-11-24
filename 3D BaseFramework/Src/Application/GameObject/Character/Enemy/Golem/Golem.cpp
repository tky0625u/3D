#include "Golem.h"
#include"../EnemyBase.h"
#include"../../Player/Player.h"
#include"../../../ObjectManager.h"
#include"Bullet/Bullet.h"

void Golem::Action()
{
	if (m_NextState != nullptr)
	{
		m_state = m_NextState;
		m_actionType = m_NextActionType;
		m_NextState.reset();
	}
	m_state->StateUpdate(this);
}

void Golem::Init()
{
	EnemyBase::Init();

	m_model->SetModelData("Asset/Models/Character/Enemy/Golem/golem.gltf");

	std::shared_ptr<Appeal> appeal = std::make_shared<Appeal>();
	m_state = appeal;
	m_flow = EnemyBase::Flow::UpdateType;

	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_model, KdCollider::TypeDamage | KdCollider::TypeBump | KdCollider::TypeSight);
}

void Golem::AttackChange()
{
	Math::Vector3 _playerPos = m_Target.lock()->GetPos();
	float _dist = (_playerPos - m_pos).Length();

	if (_dist > 50.0f)
	{
		// Attack2
		std::shared_ptr<Attack2> _attack2 = std::make_shared<Attack2>();
		m_NextState = _attack2;
		m_NextActionType = m_NextActionType = EnemyBase::Action::AttackType;
		m_flow = EnemyBase::Flow::EnterType;
		return;
	}

	//今の方向
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(m_forward, nowRot);
	nowVec.y = 0.0f;

	//向きたい方向
	Math::Vector3 toVec = m_Target.lock()->GetPos() - m_pos;
	toVec.y = 0.0f;
	toVec.Normalize();

	//内角 回転する角を求める
	float d = nowVec.Dot(toVec);
	d = std::clamp(d, -1.0f, 1.0f); //誤差修正

	//回転角度を求める
	float ang = DirectX::XMConvertToDegrees(acos(d));

	if (fabs(ang) <= 50.0f)
	{
		std::shared_ptr<Attack1> _attack1 = std::make_shared<Attack1>();
		m_NextState = _attack1;
		m_NextActionType = m_NextActionType = EnemyBase::Action::AttackType;
		m_flow = EnemyBase::Flow::EnterType;
		return;
	}
	else
	{
		std::shared_ptr<Attack3> _attack3 = std::make_shared<Attack3>();
		m_NextState = _attack3;
		m_NextActionType = m_NextActionType = EnemyBase::Action::AttackType;
		m_flow = EnemyBase::Flow::EnterType;
		return;
	}

}

void Golem::StateBase::Damage(Golem* owner, int _damage)
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
	}
}


// Appeal==========================================================================================
void Golem::Appeal::Enter(Golem* owner)
{
}

void Golem::Appeal::Update(Golem* owner)
{
	if (owner->m_anime != "Appeal")
	{
		owner->SetAnime("Appeal", false, 1.0f);
		m_handle = KdEffekseerManager::GetInstance().Play("Enemy/MagicDrak.efkefc", owner->m_pos, 5.0f, 1.0f, false).lock()->GetHandle();
		return;
	}

	if (owner->GetIsAnimator() && !KdEffekseerManager::GetInstance().IsPlaying(m_handle))
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->m_NextState = _idol;
		owner->m_NextActionType = EnemyBase::Action::IdolType;
		owner->m_flow=EnemyBase::Flow::UpdateType;
		return;
	}
}

void Golem::Appeal::Exit(Golem* owner)
{
}
//=================================================================================================


// Idol============================================================================================
void Golem::Idol::Enter(Golem* owner)
{
}

void Golem::Idol::Update(Golem* owner)
{
	if (owner->m_anime != "Idol")
	{
		owner->SetAnime("Idol", true, 1.0f);
		return;
	}
}

void Golem::Idol::Exit(Golem* owner)
{
}
//=================================================================================================


// Run=============================================================================================
void Golem::Run::Enter(Golem* owner)
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

void Golem::Run::Update(Golem* owner)
{
	if (owner->m_anime != "Run")
	{
		owner->SetAnime("Run", true, 0.7f);
		return;
	}

	Move(owner);
}

void Golem::Run::Exit(Golem* owner)
{
	if (owner->m_anime != "RunToIdol")
	{
		owner->SetAnime("RunToIdol", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->AttackChange();
		return;
	}
}
void Golem::Run::Move(Golem* owner)
{
	std::shared_ptr<Player> _player = owner->m_Target.lock();
	Math::Vector3 _playerPos = _player->GetPos();
	Math::Vector3 _pos = owner->m_pos;
	Math::Vector3 _moveDir = _playerPos - _pos;
	float dist = _moveDir.Length();
	_moveDir.Normalize();

	if (dist >= owner->m_AtkRange)owner->SetMove(_moveDir);
	owner->Rotate(_moveDir, 5.0f);
}
//=================================================================================================


// Attack1=========================================================================================
void Golem::Attack1::Enter(Golem* owner)
{
	if (owner->m_anime != "IdolToAttack1")
	{
		owner->SetAnime("IdolToAttack1", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->m_flow = EnemyBase::Flow::UpdateType;
		return;
	}
}

void Golem::Attack1::Update(Golem* owner)
{
	if (owner->m_anime != "Attack1")
	{
		owner->SetAnime("Attack1", false, 1.5f);
		m_bullet = owner->m_ObjectManager.lock()->SetBulletParam(owner->shared_from_this());
		return;
	}

	if (!m_bullet.expired() && m_bullet.lock()->GetSize() < m_bullet.lock()->GetMaxSize())
	{
		m_bullet.lock()->SetPos(owner->GetBulletPoint().Translation());

		Math::Vector3 _dir = owner->m_Target.lock()->GetPos() - owner->m_pos;
		_dir.y = 0.0f;
		_dir.Normalize();
		owner->Rotate(_dir, 1.0f);
	}
	else if (!m_bullet.expired() && m_bullet.lock()->GetSize() >= m_bullet.lock()->GetMaxSize() && m_bullet.lock()->GetDir() == Math::Vector3::Zero)
	{
		KdEffekseerManager::GetInstance().Play("Enemy/BloodLance.efkefc", owner->GetAttackStartPointMat().Translation(), 2.0f, 2.0f, false);
		Math::Matrix RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->m_angle.y));
		Math::Vector3 _dir = Math::Vector3::TransformNormal(owner->m_forward, RotY);
		m_bullet.lock()->SetDir(_dir);
	}

	if (m_bullet.expired())
	{
		owner->m_flow = EnemyBase::Flow::ExitType;
		return;
	}
}

void Golem::Attack1::Exit(Golem* owner)
{
	if (owner->m_anime != "Attack1ToIdol")
	{
		owner->SetAnime("Attack1ToIdol", false, 1.5f);
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
//=================================================================================================


// Attack2=========================================================================================
void Golem::Attack2::Enter(Golem* owner)
{
	if (owner->m_anime != "IdolToAttack2")
	{
		owner->SetAnime("IdolToAttack2", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->m_flow = EnemyBase::Flow::UpdateType;
		return;
	}
}

void Golem::Attack2::Update(Golem* owner)
{
	static Math::Vector3 _dir = Math::Vector3::Zero;
	if (owner->m_anime != "Attack2")
	{
		owner->SetAnime("Attack2", false, 1.5f);
		owner->m_JumpPow = 5.0f;

		std::shared_ptr<Player> _player = owner->m_Target.lock();
		Math::Vector3 _playerPos = _player->GetPos();
		_playerPos.y = 0.0f;
		m_playerPos = _playerPos;
		Math::Vector3 _pos = owner->m_pos;
		_pos.y = 0.0f;
		Math::Vector3 _moveDir = _playerPos - _pos;
		float dist = _moveDir.Length();
		_moveDir.Normalize();
		_dir = _moveDir;

		return;
	}

	if (owner->m_pos.y <= 0.0f)
	{
		AttackHit(owner);
		KdEffekseerManager::GetInstance().Play("Enemy/Golem/smash.efkefc", owner->m_pos, Math::Vector3{ 60.0f,15.0f,60.0f }, 1.0f, false);
		owner->m_flow = EnemyBase::Flow::ExitType;
		return;
	}


	Math::Vector3 _pos = owner->m_pos;
	Math::Vector3 _moveDir = m_playerPos - _pos;
	float dist = _moveDir.Length();
	_moveDir.Normalize();
	_dir = _moveDir;
	_pos.y = 0.0f;

	if (_pos != m_playerPos)owner->SetMove(_dir, 10.0f);
}

void Golem::Attack2::Exit(Golem* owner)
{
	if (owner->m_anime != "Attack2ToIdol")
	{
		owner->SetAnime("Attack2ToIdol", false, 1.5f);
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

void Golem::Attack2::AttackHit(Golem* owner)
{
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = owner->m_pos;
	sphereInfo.m_sphere.Radius = 60.0f;
	sphereInfo.m_type = KdCollider::Type::TypeDamage;

	// デバッグ
	//Math::Color _color = { 1,0,0,1 };
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, _color);

	if (owner->m_Target.lock()->Intersects(sphereInfo, nullptr))
	{
		owner->m_Target.lock()->Damage(5, owner->shared_from_this());
	}
}
//=================================================================================================


// Attack3=========================================================================================
void Golem::Attack3::Enter(Golem* owner)
{
	if (owner->m_anime != "IdolToAttack3")
	{
		owner->SetAnime("IdolToAttack3", false, 1.0f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		KdEffekseerManager::GetInstance().Play("Enemy/BloodLance.efkefc", owner->GetAttackStartPointMat().Translation(), 2.0f, 2.0f, false);
		owner->m_flow = owner->Flow::UpdateType;
		return;
	}
}

void Golem::Attack3::Update(Golem* owner)
{
	if (owner->m_anime != "Attack3")
	{
		owner->SetAnime("Attack3", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		AttackHit(owner);
		KdEffekseerManager::GetInstance().Play("Enemy/Golem/smash.efkefc", owner->m_pos, Math::Vector3{ 50.0f,15.0f,50.0f }, 1.0f, false);
		owner->m_flow = EnemyBase::Flow::ExitType;
		return;
	}
}

void Golem::Attack3::Exit(Golem* owner)
{
	if (owner->m_anime != "Attack3ToIdol")
	{
		owner->SetAnime("Attack3ToIdol", false, 1.5f);
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

	Math::Vector3 dir = owner->m_Target.lock()->GetPos() - owner->m_pos;
	dir.y = 0.0f;
	dir.Normalize();
	owner->Rotate(dir, 2.5f);
}

void Golem::Attack3::AttackHit(Golem* owner)
{
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = owner->GetQuakePoint().Translation();
	sphereInfo.m_sphere.Radius = 60.0f;
	sphereInfo.m_type = KdCollider::Type::TypeDamage;

	// デバッグ
	//Math::Color _color = { 1,0,0,1 };
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, _color);

	if (owner->m_Target.lock()->Intersects(sphereInfo, nullptr))
	{
		owner->m_Target.lock()->Damage(5, owner->shared_from_this());
	}
}
//=================================================================================================

void Golem::Crushing::Enter(Golem* owner)
{
}

void Golem::Crushing::Update(Golem* owner)
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

void Golem::Crushing::Exit(Golem* owner)
{
}