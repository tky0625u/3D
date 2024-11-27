#include "EnemyBase.h"
#include"../../ObjectManager.h"
#include"../../../Scene/SceneManager.h"
#include"../Player/Player.h"
#include"../../Camera/GameCamera/GameCamera.h"

void EnemyBase::PostUpdate()
{
	if (m_camera.lock()->GetState()->GetShakeFlg())return;

	CharacterBase::PostUpdate();

	if (m_inviTime > 0)
	{
		m_inviTime--;
	}
}

void EnemyBase::DrawOutLine()
{
	KdShaderManager::Instance().m_StandardShader.SetEnableOutLineDraw(m_LockONFlg);
	if(m_LockONFlg)KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
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
		m_handle = KdEffekseerManager::GetInstance().Play("Enemy/MagicDrak.efkefc", owner->m_pos, owner->GetAppealEffectSize(), 1.0f, false).lock()->GetHandle();
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

	if (dist > owner->m_AtkRange)
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
		m_leaveDir = Math::Vector3::Zero;
		return;
	}

	Math::Vector3 _dir = Math::Vector3::Zero;
	if (m_leaveDir == Math::Vector3::Zero)
	{
		int r = rand() % 2;
		if (r == 0)
		{
			m_leaveDir = { 1.0f,0.0f,0.0f };
		}
		else
		{
			m_leaveDir = { -1.0f,0.0f,0.0f };
		}
	}

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