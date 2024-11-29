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
	m_state->StateUpdate(shared_from_this());
}

void Golem::Init()
{
	EnemyBase::Init();

	m_model->SetModelData("Asset/Models/Character/Enemy/Golem/golem.gltf");

	std::shared_ptr<Appeal> appeal = std::make_shared<Appeal>();
	m_state = appeal;
	m_actionType = EnemyBase::Action::AppealType;
	m_flow = EnemyBase::Flow::UpdateType;

	m_AppealEffectSize = 5.0f;

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

// Attack1=========================================================================================
void Golem::Attack1::Enter(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("IdolToAttack1"))
	{
		owner->SetAnime("IdolToAttack1", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->SetFlow(EnemyBase::Flow::UpdateType);
		return;
	}
}

void Golem::Attack1::Update(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("Attack1"))
	{
		owner->SetAnime("Attack1", false, 1.5f);
		m_bullet = owner->GetObjManager().lock()->SetBulletParam();
		return;
	}

	if (!m_bullet.expired() && m_bullet.lock()->GetSize() < m_bullet.lock()->GetMaxSize())
	{
		m_bullet.lock()->SetPos(owner->GetObjManager().lock()->GetGolem().lock()->GetBulletPoint().Translation());

		Math::Vector3 _dir = owner->GetTarget().lock()->GetPos() - owner->GetPos();
		_dir.y = 0.0f;
		_dir.Normalize();
		owner->Rotate(_dir, 1.0f);
	}
	else if (!m_bullet.expired() && m_bullet.lock()->GetSize() >= m_bullet.lock()->GetMaxSize() && m_bullet.lock()->GetDir() == Math::Vector3::Zero)
	{
		KdEffekseerManager::GetInstance().Play("Enemy/BloodLance.efkefc", owner->GetAttackStartPointMat().Translation(), 2.0f, 2.0f, false);
		Math::Matrix RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->GetAngle().y));
		Math::Vector3 _dir = Math::Vector3::TransformNormal(owner->GetForward(), RotY);
		m_bullet.lock()->SetDir(_dir);
	}

	if (m_bullet.expired())
	{
		owner->SetFlow(EnemyBase::Flow::ExitType);
		return;
	}
}

void Golem::Attack1::Exit(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("Attack1ToIdol"))
	{
		owner->SetAnime("Attack1ToIdol", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->SetNextAction(_idol, EnemyBase::Action::IdolType);
		owner->SetFlow(EnemyBase::Flow::UpdateType);
		return;
	}
}
//=================================================================================================


// Attack2=========================================================================================
void Golem::Attack2::Enter(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("IdolToAttack2"))
	{
		owner->SetAnime("IdolToAttack2", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->SetFlow(EnemyBase::Flow::UpdateType);
		return;
	}
}

void Golem::Attack2::Update(std::shared_ptr<EnemyBase> owner)
{
	static Math::Vector3 _dir = Math::Vector3::Zero;
	if (!owner->IsAnimCheck("Attack2"))
	{
		owner->SetAnime("Attack2", false, 1.5f);
		owner->SetJumpPow(5.0f);

		std::shared_ptr<Player> _player = owner->GetTarget().lock();
		Math::Vector3 _playerPos = _player->GetPos();
		_playerPos.y = 0.0f;
		m_playerPos = _playerPos;
		Math::Vector3 _pos = owner->GetPos();
		_pos.y = 0.0f;
		Math::Vector3 _moveDir = _playerPos - _pos;
		float dist = _moveDir.Length();
		_moveDir.Normalize();
		_dir = _moveDir;
		owner->SetColorLightFlg(true);
		return;
	}

	if (owner->GetPos().y <= 0.0f)
	{
		AttackHit(owner);
		owner->SetColorLightFlg(false);
		KdEffekseerManager::GetInstance().Play("Enemy/Golem/smash.efkefc", owner->GetPos(), Math::Vector3{60.0f,15.0f,60.0f}, 1.0f, false);
		owner->SetFlow(EnemyBase::Flow::ExitType);
		return;
	}


	Math::Vector3 _pos = owner->GetPos();
	Math::Vector3 _moveDir = m_playerPos - _pos;
	float dist = _moveDir.Length();
	_moveDir.Normalize();
	_dir = _moveDir;
	_pos.y = 0.0f;

	if (_pos != m_playerPos)owner->SetMove(_dir, 10.0f);

	KdShaderManager::Instance().WriteCBColor(m_playerPos, 60.0f);
}

void Golem::Attack2::Exit(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("Attack2ToIdol"))
	{
		owner->SetAnime("Attack2ToIdol", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->SetNextAction(_idol, EnemyBase::Action::IdolType);
		owner->SetFlow(EnemyBase::Flow::UpdateType);
		return;
	}
}

void Golem::Attack2::AttackHit(std::shared_ptr<EnemyBase> owner)
{
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = owner->GetPos();
	sphereInfo.m_sphere.Radius = 60.0f;
	sphereInfo.m_type = KdCollider::Type::TypeDamage;

	// デバッグ
	//Math::Color _color = { 1,0,0,1 };
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, _color);

	if (owner->GetTarget().lock()->Intersects(sphereInfo, nullptr))
	{
		owner->GetTarget().lock()->Damage(5, owner->shared_from_this());
	}
}
//=================================================================================================


// Attack3=========================================================================================
void Golem::Attack3::Enter(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("IdolToAttack3"))
	{
		owner->SetAnime("IdolToAttack3", false, 1.0f);
		owner->SetColorLightFlg(true);
		return;
	}

	if (owner->GetIsAnimator())
	{
		KdEffekseerManager::GetInstance().Play("Enemy/BloodLance.efkefc", owner->GetAttackStartPointMat().Translation(), 2.0f, 2.0f, false);
		owner->SetFlow(owner->Flow::UpdateType);
		return;
	}

	KdShaderManager::Instance().WriteCBColor(owner->GetPos(), 50.0f);
}

void Golem::Attack3::Update(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("Attack3"))
	{
		owner->SetAnime("Attack3", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		AttackHit(owner);
		owner->SetColorLightFlg(false);
		KdEffekseerManager::GetInstance().Play("Enemy/Golem/smash.efkefc", owner->GetPos(), Math::Vector3{50.0f,15.0f,50.0f}, 1.0f, false);
		owner->SetFlow(EnemyBase::Flow::ExitType);
		return;
	}

	KdShaderManager::Instance().WriteCBColor(owner->GetPos(), 50.0f);
}

void Golem::Attack3::Exit(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("Attack3ToIdol"))
	{
		owner->SetAnime("Attack3ToIdol", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->SetNextAction(_idol, EnemyBase::Action::IdolType);
		owner->SetFlow(EnemyBase::Flow::UpdateType);
		return;
	}

	Math::Vector3 dir = owner->GetTarget().lock()->GetPos() - owner->GetPos();
	dir.y = 0.0f;
	dir.Normalize();
	owner->Rotate(dir, 2.5f);
}

void Golem::Attack3::AttackHit(std::shared_ptr<EnemyBase> owner)
{
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = owner->GetObjManager().lock()->GetGolem().lock()->GetQuakePoint().Translation();
	sphereInfo.m_sphere.Radius = 50.0f;
	sphereInfo.m_type = KdCollider::Type::TypeDamage;

	// デバッグ
	//Math::Color _color = { 1,0,0,1 };
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, _color);

	if (owner->GetTarget().lock()->Intersects(sphereInfo, nullptr))
	{
		owner->GetTarget().lock()->Damage(5, owner->shared_from_this());
	}
}
//=================================================================================================
