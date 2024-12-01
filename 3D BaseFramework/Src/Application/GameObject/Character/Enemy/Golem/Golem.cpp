#include "Golem.h"
#include"../../../../Scene/SceneManager.h"
#include"../EnemyBase.h"
#include"../../Player/Player.h"
#include"../../../Camera/GameCamera/GameCamera.h"
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

void Golem::PostUpdate()
{
	if (m_camera.lock()->GetState()->GetShakeFlg())return;

	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = m_pos;
	float LitleUP = 1.0f;
	rayInfo.m_pos.y += LitleUP;
	rayInfo.m_dir = Math::Vector3::Down;
	rayInfo.m_range = m_gravity + LitleUP;
	rayInfo.m_type = KdCollider::TypeGround;

	//デバッグ用
	//Math::Color color = { 1,1,1,1 };
	//m_pDebugWire->AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range, color);

	std::list<KdCollider::CollisionResult> retRayList;
	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		ret->Intersects(rayInfo, &retRayList);
	}

	float _maxOverLap = 0.0f;
	Math::Vector3 _hitPos = Math::Vector3::Zero;
	bool _hitFlg = false;

	for (auto& ray : retRayList)
	{
		if (_maxOverLap < ray.m_overlapDistance)
		{
			_maxOverLap = ray.m_overlapDistance;
			_hitPos = ray.m_hitPos;
			_hitFlg = true;
		}
	}

	m_groundFlg = false;
	if (_hitFlg)
	{
		m_pos = _hitPos;
		m_dir.y = 0.0f;
		m_gravity = 0.0f;
		m_param.JumpPow = 0.0f;
		m_groundFlg = true;
	}

	KdCollider::SphereInfo sphereInfo;
	Math::Matrix _mat = m_model->FindWorkNode("spine")->m_worldTransform * (Math::Matrix::CreateTranslation(m_mWorld.Translation()));
	sphereInfo.m_sphere.Center = _mat.Translation();
	sphereInfo.m_sphere.Radius = m_HitSphereSize;
	sphereInfo.m_type = KdCollider::TypeBump;

	//デバッグ用
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, Math::Color{ 0,1,1,1 });

	std::list<KdCollider::CollisionResult>retSphereList;
	std::vector<std::shared_ptr<KdGameObject>>SphereList;
	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		if (ret->GetName() == "Bullet")continue;

		if (ret->Intersects(sphereInfo, &retSphereList))SphereList.push_back(ret);
	}
	for (auto& ret : SceneManager::Instance().GetEnemyList())
	{
		if (ret->IsExpired())return;
		if (m_id == ret->GetID())continue;
		if (ret->Intersects(sphereInfo, &retSphereList))SphereList.push_back(ret);
	}
	if (m_id != SceneManager::Instance().GetPlayer()->GetID())
	{
		if (SceneManager::Instance().GetPlayer()->Intersects(sphereInfo, &retSphereList))
		{
			SphereList.push_back(SceneManager::Instance().GetPlayer());
		}
	}

	Math::Vector3 HitDir = Math::Vector3::Zero;
	float maxOverLap = 0.0f;
	bool HitFlg = false;
	std::shared_ptr<KdGameObject> HitObj = nullptr;
	int listNum = 0;
	for (auto& sphere : retSphereList)
	{
		if (maxOverLap < sphere.m_overlapDistance)
		{
			maxOverLap = sphere.m_overlapDistance;
			HitDir = sphere.m_hitDir;
			HitObj = SphereList[listNum];
			HitFlg = true;
		}
		listNum++;
	}

	if (HitFlg == true)
	{
		HitDir.y = 0.0f;
		HitDir.Normalize();
		if (HitObj->GetObjType() == KdGameObject::ObjType::oObject)
		{
			m_pos += maxOverLap * HitDir;
		}
		else
		{
			Math::Vector3 _pos = HitObj->GetPos();
			HitDir *= -1.0f;
			_pos += maxOverLap * HitDir;
			HitObj->SetPos(_pos);
		}
	}

	if (m_camera.lock()->GetState()->GetShakeFlg())return;

	//アニメーションの更新
	if (m_anime != m_beforeAnime)
	{
		m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);
		m_beforeAnime = m_anime;
	}
	float _slow = 1.0f;
	_slow = m_ObjectManager.lock()->GetSlow();
	m_animator->AdvanceTime(m_model->WorkNodes(), m_animeSpeed * _slow);
	m_model->CalcNodeMatrices();

	if (m_inviTime > 0)
	{
		m_inviTime--;
	}

	// デバッグ
	if (GetAsyncKeyState('P') & 0x8000)
	{
		m_param.Hp = 0;
		std::shared_ptr<Crushing> _crush = std::make_shared<Crushing>();
		m_NextState = _crush;
		m_NextActionType = CharacterBase::Action::CrushingType;
		m_flow = Flow::UpdateType;
		return;
	}
	KdShaderManager::Instance().WriteCBColorEnable(m_ColorLightFlg);
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
		owner->SetAnime("IdolToAttack1", false, 1.0f);
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
		owner->SetAnime("Attack1", false, 1.0f);
		m_bullet = owner->GetObjManager().lock()->SetBulletParam();
		return;
	}

	if (m_bullet.expired())
	{
		owner->SetFlow(EnemyBase::Flow::ExitType);
		return;
	}

	if (m_ActionFPS == 10)
	{
		KdEffekseerManager::GetInstance().Play("Enemy/BloodLance.efkefc", owner->GetAttackStartPointMat().Translation(), 2.0f, 2.0f, false);
		Math::Matrix RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->GetAngle().y));
		Math::Vector3 _dir = Math::Vector3::TransformNormal(owner->GetForward(), RotY);
		m_bullet.lock()->SetDir(_dir);
	}

	if (!m_bullet.expired() && m_bullet.lock()->GetSize() < m_bullet.lock()->GetMaxSize())
	{
		m_bullet.lock()->SetPos(owner->GetObjManager().lock()->GetGolem().lock()->GetBulletPoint().Translation());

		Math::Vector3 _dir = owner->GetTarget().lock()->GetPos() - owner->GetPos();
		_dir.y = 0.0f;
		_dir.Normalize();
		owner->Rotate(_dir, 1.0f);
	}
	else if (!m_bullet.expired() && m_bullet.lock()->GetSize() >= m_bullet.lock()->GetMaxSize())
	{
		m_ActionFPS++;
	}
}

void Golem::Attack1::Exit(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("Attack1ToIdol"))
	{
		owner->SetAnime("Attack1ToIdol", false, 1.0f);
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
		owner->SetAnime("IdolToAttack2", false, 1.0f);
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
		owner->SetAnime("Attack2", false, 1.0f);
		owner->SetJumpPow(5.0f);

		std::shared_ptr<Player> _player = owner->GetTarget().lock();
		Math::Vector3 _playerPos = _player->GetPos();
		m_playerPos = _playerPos;
		Math::Vector3 _pos = owner->GetPos();
		Math::Vector3 _moveDir = _playerPos - _pos;
		float dist = _moveDir.Length();
		_moveDir.Normalize();
		_dir = _moveDir;
		owner->SetColorLightFlg(true);
		owner->SetHitSphereSize(10.0f);
		return;
	}

	if (owner->GetGroundFlg())
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

	if (_pos != m_playerPos)owner->SetMove(_dir, 10.0f);

	KdShaderManager::Instance().WriteCBColor(m_playerPos, 60.0f);
}

void Golem::Attack2::Exit(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("Attack2ToIdol"))
	{
		owner->SetAnime("Attack2ToIdol", false, 1.0f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->SetHitSphereSize(2.0f);

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
		owner->GetTarget().lock()->Damage(owner->GetParam().Atk, owner->shared_from_this());
	}
}
//=================================================================================================


// Attack3=========================================================================================
void Golem::Attack3::Enter(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("IdolToAttack3"))
	{
		owner->SetAnime("IdolToAttack3", false, 1.0f);
		
		return;
	}

	if (owner->GetIsAnimator())
	{

		owner->SetFlow(owner->Flow::UpdateType);
		return;
	}

	
}

void Golem::Attack3::Update(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("Attack3"))
	{
		owner->SetAnime("Attack3", false, 1.0f);
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

	if (161 >= m_ActionFPS)
	{
		owner->SetColorLightFlg(true);
	}
	else if (162 == m_ActionFPS)
	{
		KdEffekseerManager::GetInstance().Play("Enemy/BloodLance.efkefc", owner->GetAttackStartPointMat().Translation(), 2.0f, 2.0f, false);
	}
	else if (180 == m_ActionFPS)
	{
		AttackHit(owner);
		owner->SetColorLightFlg(false);
		KdEffekseerManager::GetInstance().Play("Enemy/Golem/smash.efkefc", owner->GetPos(), Math::Vector3{ 57.0f,15.0f,57.0f }, 1.0f, false);
	}
	else
	{
		Math::Vector3 dir = owner->GetTarget().lock()->GetPos() - owner->GetPos();
		dir.y = 0.0f;
		dir.Normalize();
		owner->Rotate(dir, 2.5f);
	}


	m_ActionFPS++;
	KdShaderManager::Instance().WriteCBColor(owner->GetPos(), 50.0f);
}

void Golem::Attack3::Exit(std::shared_ptr<EnemyBase> owner)
{

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
		owner->GetTarget().lock()->Damage(owner->GetParam().Atk, owner->shared_from_this());
	}
}
//=================================================================================================
