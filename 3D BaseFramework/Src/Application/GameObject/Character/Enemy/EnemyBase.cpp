#include "EnemyBase.h"
#include"../../ObjectManager.h"
#include"../../../Scene/SceneManager.h"
#include"../Player/Player.h"
#include"../../Camera/GameCamera/GameCamera.h"

void EnemyBase::PostUpdate()
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

	if (m_actionType != Action::AppealType)
	{
		KdCollider::SphereInfo sphereInfo;
		Math::Matrix _mat = m_model->FindWorkNode("spine")->m_worldTransform * (Math::Matrix::CreateTranslation(m_mWorld.Translation()));
		sphereInfo.m_sphere.Center = _mat.Translation();
		sphereInfo.m_sphere.Radius = m_HitSphereSize;
		sphereInfo.m_type = KdCollider::TypeBump;

		//デバッグ用
		//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, Math::Color{ 0,1,1,1 });

		std::list<KdCollider::CollisionResult>retSphereList;
		for (auto& ret : SceneManager::Instance().GetObjList())
		{
			ret->Intersects(sphereInfo, &retSphereList);
		}
		for (auto& ret : SceneManager::Instance().GetEnemyList())
		{
			if (ret->IsExpired())return;
			if (m_id == ret->GetID())continue;
			ret->Intersects(sphereInfo, &retSphereList);
		}
		if (m_id != SceneManager::Instance().GetPlayer()->GetID())SceneManager::Instance().GetPlayer()->Intersects(sphereInfo, &retSphereList);

		Math::Vector3 HitDir = Math::Vector3::Zero;
		float maxOverLap = 0.0f;
		bool HitFlg = false;

		for (auto& sphere : retSphereList)
		{
			if (maxOverLap < sphere.m_overlapDistance)
			{
				maxOverLap = sphere.m_overlapDistance;
				HitDir = sphere.m_hitDir;
				HitFlg = true;
			}
		}

		if (HitFlg == true)
		{
			HitDir.y = 0.0f;
			HitDir.Normalize();
			m_pos += maxOverLap * HitDir;
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

void EnemyBase::Action()
{
	if (m_NextState)
	{
		m_state = m_NextState;
		m_actionType = m_NextActionType;
		m_NextState.reset();
	}
	m_state->StateUpdate(shared_from_this());
}

void EnemyBase::Init()
{
	CharacterBase::Init();

	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Trans;

	m_ObjType = ObjType::oEnemy;

	srand(timeGetTime());
}

void EnemyBase::CrushingAction()
{
	CharacterBase::CrushingAction();

	if (m_dissolve >= 1.0f)
	{
		m_isExpired = true;
	}
}

Math::Matrix EnemyBase::GettoTargetRotateYMatrix(std::weak_ptr<Player> _target)
{
	Math::Matrix _nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	Math::Vector3 _nowVec = Math::Vector3::TransformNormal(m_forward, _nowRot);
	_nowVec.Normalize();

	Math::Vector3 _dir = _target.lock()->GetPos() - m_pos;
	_dir.y = 0.0f;
	_dir.Normalize();

	float d = _nowVec.Dot(_dir);
	d = std::clamp(d, -1.0f, 1.0f);

	float _ang = DirectX::XMConvertToDegrees(acos(d));

	Math::Vector3 c = _dir.Cross(_nowVec);
	if (c.y > 0)_ang = 360.0f - _ang;

	float _playerToAng = m_angle.y + _ang;
	if (_playerToAng >= 360.0f)_playerToAng -= 360.0f;
	else if (_playerToAng < 0.0f)_playerToAng += 360.0f;

	return Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_playerToAng));
}

void EnemyBase::StateBase::Damage(std::shared_ptr<EnemyBase> owner, int _damage)
{
	owner->m_param.Hp -= _damage;
	if (owner->m_param.Hp <= 0)
	{
		owner->m_param.Hp = 0;
		owner->m_ColorLightFlg = false;
		// Crushing
		std::shared_ptr<Crushing> _crush = std::make_shared<Crushing>();
		owner->m_NextState = _crush;
		owner->m_NextActionType = EnemyBase::Action::CrushingType;
		owner->m_flow = EnemyBase::Flow::UpdateType;
		return;
	}

	// Hit
	owner->HitChange();
}

void EnemyBase::Appeal::Enter(std::shared_ptr<EnemyBase> owner)
{
}

void EnemyBase::Appeal::Update(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("Appeal"))
	{
		owner->SetAnime("Appeal", false, 1.0f);
		m_handle = KdEffekseerManager::GetInstance().Play("Enemy/Appeal/MagicDrak.efkefc", owner->m_pos, owner->GetAppealEffectSize(), 1.0f, false).lock()->GetHandle();
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

void EnemyBase::Appeal::Exit(std::shared_ptr<EnemyBase> owner)
{
}


void EnemyBase::Idol::Enter(std::shared_ptr<EnemyBase> owner)
{
}

void EnemyBase::Idol::Update(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("Idol"))
	{
		owner->SetAnime("Idol", true, 1.0f);
		return;
	}
}

void EnemyBase::Idol::Exit(std::shared_ptr<EnemyBase> owner)
{
}


void EnemyBase::Run::Enter(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("IdolToRun"))
	{
		owner->SetAnime("IdolToRun", false, 1.5f);

		int r = rand() % 2;
		if (r == 0)
		{
			m_leaveDir = { 1.0f,0.0f,0.0f };
		}
		else
		{
			m_leaveDir = { -1.0f,0.0f,0.0f };
		}

		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->m_flow = EnemyBase::Flow::UpdateType;
		return;
	}

	Move(owner);
}		
		
void EnemyBase::Run::Update(std::shared_ptr<EnemyBase> owner)
{		
	if (!owner->IsAnimCheck("Run"))
	{
		owner->SetAnime("Run", true, 0.7f);
		return;
	}

	Move(owner);
}		
		
void EnemyBase::Run::Exit(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("RunToIdol"))
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

void EnemyBase::Run::Move(std::shared_ptr<EnemyBase> owner)
{
	std::shared_ptr<Player> _player = owner->m_Target.lock();
	Math::Vector3 _playerPos = _player->GetPos();
	Math::Vector3 _pos = owner->m_pos;
	Math::Vector3 _moveDir = _playerPos - _pos;
	float dist = _moveDir.Length();
	_moveDir.Normalize();
	m_moveDir = _moveDir;

	if (dist > owner->m_param.AtkRange)
	{
		if (owner->m_leaveEnemy.expired())EnemyCheck(owner);
		if (!owner->m_leaveEnemy.expired())Leave(owner);

		owner->SetMove(m_moveDir);
	}
	owner->Rotate(m_moveDir);
}

bool EnemyBase::Run::PlayerDistCheck(std::shared_ptr<EnemyBase> owner, std::weak_ptr<EnemyBase> leave)
{
	std::shared_ptr<Player> _player = owner->GetTarget().lock();
	Math::Vector3 _playerPos = _player->GetPos();
	Math::Vector3 _pos = owner->GetPos();
	float _dist = (_playerPos - _pos).Length();

	Math::Vector3 _leavePos = leave.lock()->GetPos();
	float _leavedist = (_playerPos - _leavePos).Length();

	if (_dist <= _leavedist)return false;

	return true;
}

void EnemyBase::Run::EnemyCheck(std::shared_ptr<EnemyBase> owner)
{
	KdCollider::SphereInfo sphereInfo;
	Math::Matrix _mat = owner->m_model->FindWorkNode("spine")->m_worldTransform * (Math::Matrix::CreateTranslation(owner->m_mWorld.Translation()));
	sphereInfo.m_sphere.Center = _mat.Translation();
	sphereInfo.m_sphere.Radius = 3.0f;
	sphereInfo.m_type = KdCollider::TypeBump;

	std::list<KdCollider::CollisionResult> _retSphereList;
	std::vector<std::weak_ptr<EnemyBase>>  _HitEnemyList;
	for (auto& enemy : SceneManager::Instance().GetEnemyList())
	{
		if (enemy->GetID() == owner->m_id)continue;
		if (!enemy->GetLeave())continue;
		if (enemy->Intersects(sphereInfo, &_retSphereList))_HitEnemyList.push_back(enemy);
	}

	bool _IsHit = false;
	float _maxOverLap = 0.0f;
	std::weak_ptr<EnemyBase> _HitEnemy;
	int   _listNum = 0;

	for (auto& enemy : _retSphereList)
	{
		if (_maxOverLap < enemy.m_overlapDistance &&
			PlayerDistCheck(owner, _HitEnemyList[_listNum]))
		{
			_maxOverLap = enemy.m_overlapDistance;
			_HitEnemy = _HitEnemyList[_listNum];
			_IsHit = true;
		}
		_listNum++;
	}

	if (_IsHit)
	{
		owner->m_leaveEnemy = _HitEnemy;
	}
}

void EnemyBase::Run::Leave(std::shared_ptr<EnemyBase> owner)
{
	if (owner->m_leaveEnemy.expired())return;
	if (owner->m_leaveEnemy.lock()->GetParam().Hp <= 0)return;

	Math::Vector3 _pos = owner->m_pos;
	_pos.y = 0.0f;
	Math::Vector3 _leavePos = owner->m_leaveEnemy.lock()->GetPos();
	_leavePos.y = 0.0f;
	float         _dist = (_leavePos - _pos).Length();

	if (_dist > 6.0f)
	{
		owner->m_leaveEnemy.reset();
		return;
	}

	Math::Vector3 _dir = Math::Vector3::Zero;
	_dir = m_leaveDir.TransformNormal(m_leaveDir, owner->GettoTargetRotateYMatrix(owner->GetTarget()));
	_dir.Normalize();
	m_moveDir = _dir;
	m_moveDir.Normalize();
}


void EnemyBase::Hit::Enter(std::shared_ptr<EnemyBase> owner)
{
}

void EnemyBase::Hit::Update(std::shared_ptr<EnemyBase> owner)
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
		owner->m_NextActionType = EnemyBase::Action::IdolType;
		owner->m_flow = EnemyBase::Flow::UpdateType;
		return;
	}
}				
				
void EnemyBase::Hit::Exit(std::shared_ptr<EnemyBase> owner)
{
}


void EnemyBase::Crushing::Enter(std::shared_ptr<EnemyBase> owner)
{				
}				
				
void EnemyBase::Crushing::Update(std::shared_ptr<EnemyBase> owner)
{			
	if (!owner->IsAnimCheck("Death"))
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
				
void EnemyBase::Crushing::Exit(std::shared_ptr<EnemyBase> owner)
{
}