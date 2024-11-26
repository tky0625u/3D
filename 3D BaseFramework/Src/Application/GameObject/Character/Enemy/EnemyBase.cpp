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
}

void EnemyBase::CrushingAction()
{
	CharacterBase::CrushingAction();

	if (m_dissolve >= 1.0f)
	{
		m_isExpired = true;
	}
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

	KdCollider::SphereInfo sphereInfo;
	Math::Matrix _mat = owner->m_model->FindWorkNode("spine")->m_worldTransform * (Math::Matrix::CreateTranslation(owner->m_mWorld.Translation()));
	sphereInfo.m_sphere.Center = _mat.Translation();
	sphereInfo.m_sphere.Radius = owner->m_AtkRange;
	sphereInfo.m_type = KdCollider::TypeBump;

	std::list<KdCollider::CollisionResult> _retSphereList;
	for (auto& enemy : SceneManager::Instance().GetEnemyList())
	{
		if (enemy->GetID() == owner->m_id)continue;
		enemy->Intersects(sphereInfo, &_retSphereList);
	}

	bool _IsHit = false;
	Math::Vector3 _HitDir = Math::Vector3::Zero;
	float _maxOverLap = 0.0f;

	for (auto& enemy : _retSphereList)
	{
		if (_maxOverLap < enemy.m_overlapDistance)
		{
			_maxOverLap = enemy.m_overlapDistance;
			_HitDir = enemy.m_hitDir;
			_IsHit = true;
		}
	}

	if (_IsHit)
	{
		_HitDir.y = 0.0f;
		_HitDir.Normalize();
		_moveDir = (_moveDir + _HitDir) / 2.0f;
		_moveDir.Normalize();
	}

	if (dist >= owner->m_AtkRange)owner->SetMove(_moveDir);
	owner->Rotate(_moveDir);
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