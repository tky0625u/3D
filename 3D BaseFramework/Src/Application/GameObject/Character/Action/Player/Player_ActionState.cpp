#include "Player_ActionState.h"
#include"../../../../Scene/SceneManager.h"
#include"../../Enemy/EnemyBase.h"
#include"../../BulletBase/BulletBase.h"
#include"../Enemy/Enemy_ConText.h"
#include"../../Player/Player.h"
#include"../../../Camera/GameCamera/GameCamera.h"
#include"../../../Camera/GameCamera/GameCamera_ConText.h"
#include"../../../Camera/GameCamera/GameCamera_State.h"
#include"Player_ConText.h"
#include"../../../Weapon/Sword/Sword.h"

#include"Idol/Player_Idol.h"
#include"Run/Player_Run.h"
#include"Attack/Player_Attack.h"
#include"Roll/Player_Roll.h"
#include"Guard/Player_Guard.h"
#include"GuardReaction/Player_GuardReaction.h"
#include"Parry/Player_Parry.h"
#include"Counter/Player_Counter.h"
#include"Hit/Player_Hit.h"
#include"Crushing/Player_Crushing.h"
#include"Teleport/Player_Teleport.h"

void Player_ActionState::AttackDamage()
{
	if (m_target.expired())return;

	std::vector<KdCollider::SphereInfo> sphereInfoList;
	KdCollider::SphereInfo sphereInfo;

	if (m_target.lock()->GetSword().expired() == false)
	{
		sphereInfo.m_sphere.Center = m_target.lock()->GetSword().lock()->GetModelTop().Translation();
		sphereInfoList.push_back(sphereInfo);

		sphereInfo.m_sphere.Center = m_target.lock()->GetSword().lock()->GetModelCenter().Translation();
		sphereInfoList.push_back(sphereInfo);

		sphereInfo.m_sphere.Center = m_target.lock()->GetSword().lock()->GetModelBottom().Translation();
		sphereInfoList.push_back(sphereInfo);
	}
	else
	{
		sphereInfo.m_sphere.Center = m_target.lock()->GetSwordMat().Translation();
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
		if (m_target.lock()->GetParryID() != -1 && sphere->GetID() != m_target.lock()->GetParryID())continue;

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
			m_target.lock()->GetCamera().lock()->GetConText()->GetState()->SetShakeFlg(true);
			hitEnemy->Hit(m_target.lock()->GetParam().Atk);
			hitEnemy->GetConText()->Hit(m_target.lock()->GetParam().Atk);
			hitEnemy->SetInviTime(m_target.lock()->GetinviTime());
			KdEffekseerManager::GetInstance().Play("Enemy/hit_eff.efkefc", ret.m_hitPos, 1.0f, 0.8f, false);
			KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/刀で斬る2.WAV", 0.05f, false);
		}
	}

	if (m_target.lock()->GetParryID() != -1)m_target.lock()->SetParryID(-1);
}

void Player_ActionState::LockON()
{
	if (m_target.lock()->GetConText()->GetLockONFlg() && !m_target.lock()->GetConText()->GetLockONTarget().expired() && m_target.lock()->GetConText()->GetLockONTarget().lock()->GetParam().Hp > 0)
	{
		m_target.lock()->GetConText()->SetLockONFlg(false);
		m_target.lock()->GetConText()->GetLockONTarget().lock()->SetLockONFlg(false);
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
				m_target.lock()->GetCamera().lock()->GetCamera()->ConvertWorldToScreenDetail(SceneManager::Instance().GetEnemyList()[e]->GetPos(), _2dPos);
				HitFlg = true;
				listNum = e;
			}
			else
			{
				Math::Vector3 _tmp2dPos = Math::Vector3::Zero;
				m_target.lock()->GetCamera().lock()->GetCamera()->ConvertWorldToScreenDetail(SceneManager::Instance().GetEnemyList()[e]->GetPos(), _tmp2dPos);
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
		m_target.lock()->GetConText()->SetLockONTarget(SceneManager::Instance().GetEnemyList()[listNum]);
		m_target.lock()->GetConText()->SetLockONFlg(true);
	}
	else
	{
		m_target.lock()->GetConText()->SetLockONFlg(false);
	}
}

void Player_ActionState::Update()
{
	ActionBase::Update();
	if (m_target.lock()->GetStaminaRecoveryTime() <= 0)m_target.lock()->StaminaRecovery();
	
	if (m_target.lock()->GetCamera().lock()->GetCameraType() != GameCamera::CameraType::PlayerType)return;

	KeyCheck();
	if (m_target.lock()->GetConText()->GetLockONFlg())
	{
		if (!m_target.lock()->GetConText()->GetLockONTarget().expired())
		{
			if (m_target.lock()->GetConText()->GetLockONTarget().lock()->GetParam().Hp <= 0)
			{
				m_target.lock()->GetConText()->GetLockONTarget().lock()->SetLockONFlg(false);
				LockON();
			}
		}
	}
	ChangeAction();
	m_target.lock()->GetConText()->SetBeforeKeyType(m_KeyType);
}

void Player_ActionState::KeyCheck()
{
	//移動
	if (GetAsyncKeyState('W') & 0x8000 | GetAsyncKeyState('A') & 0x8000 | GetAsyncKeyState('S') & 0x8000 | GetAsyncKeyState('D') & 0x8000)
	{
		m_KeyType |= Player_ActionConText::KeyType::MoveKey;
	}
	else if (m_KeyType & Player_ActionConText::KeyType::MoveKey)
	{
		m_KeyType ^= Player_ActionConText::KeyType::MoveKey;
	}

	//攻撃
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_KeyType |= Player_ActionConText::KeyType::AttackKey;
	}
	else if (m_KeyType & Player_ActionConText::KeyType::AttackKey)
	{
		m_KeyType ^= Player_ActionConText::KeyType::AttackKey;
	}

	//防御
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_KeyType |= Player_ActionConText::KeyType::GuardKey;
	}
	else if (m_KeyType & Player_ActionConText::KeyType::GuardKey)
	{
		m_KeyType ^= Player_ActionConText::KeyType::GuardKey;
	}

	//回避
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_KeyType |= Player_ActionConText::KeyType::RollKey;
	}
	else if (m_KeyType & Player_ActionConText::KeyType::RollKey)
	{
		m_KeyType ^= Player_ActionConText::KeyType::RollKey;
	}

	//ロックオン
	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
	{
		if(!(m_target.lock()->GetConText()->GetBeforeKeyType() & Player_ActionConText::KeyType::LockONKey))LockON();
		m_KeyType |= Player_ActionConText::KeyType::LockONKey;
	}
	else if (m_KeyType & Player_ActionConText::KeyType::LockONKey)
	{
		m_KeyType ^= Player_ActionConText::KeyType::LockONKey;
	}
}

void Player_ActionState::Idol()
{
	std::shared_ptr<Player_Idol> idol = std::make_shared<Player_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	m_target.lock()->SetNextState(idol,Player::Action::IdolType);
}

void Player_ActionState::Run()
{
	std::shared_ptr<Player_Run> run = std::make_shared<Player_Run>();
	if (m_target.expired())return;
	run->SetTarget(m_target.lock());
	m_target.lock()->SetNextState(run, Player::Action::RunType);
}

void Player_ActionState::Attack()
{
	std::shared_ptr<Player_Attack> attack = std::make_shared<Player_Attack>();
	if (m_target.expired())return;
	attack->SetTarget(m_target.lock());
	attack->AttackDirCheck();
	m_target.lock()->SetNextState(attack, Player::Action::AttackType);
}

void Player_ActionState::Guard()
{
	std::shared_ptr<Player_Guard> guard = std::make_shared<Player_Guard>();
	if (m_target.expired())return;
	guard->SetTarget(m_target.lock());
	guard->SetObjManager(m_target.lock()->GetObjectManager().lock());
	m_target.lock()->SetNextState(guard, Player::Action::GuardType);
}

void Player_ActionState::GuardReaction()
{
	if (m_target.expired())return;
	int sta = m_target.lock()->GetParam().Sm;
	if (sta <= 0)return;
	sta -= 10;
	if (sta <= 0)sta = 0;
	std::shared_ptr<Player_GuardReaction> guardReaction = std::make_shared<Player_GuardReaction>();
	m_target.lock()->SetStamina(sta);
	m_target.lock()->SetStaminaRecoveryTime(60);
	guardReaction->SetTarget(m_target.lock());
	m_target.lock()->SetNextState(guardReaction, Player::Action::GuardReactionType);
}

void Player_ActionState::Parry(std::shared_ptr<EnemyBase> _enemy)
{
	std::shared_ptr<Player_Parry> parry = std::make_shared<Player_Parry>();
	if (m_target.expired())return;
	parry->SetTarget(m_target.lock());
	parry->SetObjManager(m_target.lock()->GetObjectManager().lock());
	m_target.lock()->SetParryID(_enemy->GetID());
	m_target.lock()->SetNextState(parry, Player::Action::ParryType);
	_enemy->GetConText()->Stumble();
}

void Player_ActionState::Parry(std::shared_ptr<BulletBase> _bullet)
{
	std::shared_ptr<Player_Parry> parry = std::make_shared<Player_Parry>();
	if (m_target.expired())return;
	parry->SetTarget(m_target.lock());
	parry->SetObjManager(m_target.lock()->GetObjectManager().lock());
	m_target.lock()->SetNextState(parry, Player::Action::ParryType);
	_bullet->SetDir(_bullet->GetDir() * -1);
}

void Player_ActionState::Counter()
{
	std::shared_ptr<Player_Counter> counter = std::make_shared<Player_Counter>();
	if (m_target.expired())return;
	counter->SetTarget(m_target.lock());
	m_target.lock()->SetNextState(counter, Player::Action::CounterType);
}

void Player_ActionState::Roll()
{
	if (m_target.expired())return;
	int sta = m_target.lock()->GetParam().Sm;
	if (sta <= 0)return;
	sta -= 10;
	if (sta <= 0)sta = 0;
	std::shared_ptr<Player_Roll> roll = std::make_shared<Player_Roll>();
	m_target.lock()->SetStamina(sta);
	m_target.lock()->SetStaminaRecoveryTime(60);
	roll->SetTarget(m_target.lock());
	m_target.lock()->SetNextState(roll, Player::Action::RollType);
}

void Player_ActionState::Hit(int _damage, std::shared_ptr<EnemyBase> _enemy)
{
	std::shared_ptr<Player_Hit> hit = std::make_shared<Player_Hit>();
	if (m_target.expired())return;
	hit->SetTarget(m_target.lock());
	m_target.lock()->Hit(_damage);
	if (m_target.lock()->GetParam().Hp <= 0)
	{
		Crushing();
		return;
	}
	m_target.lock()->SetNextState(hit, Player::Action::HitType);
}

void Player_ActionState::Hit(int _damage, std::shared_ptr<BulletBase> _bullet)
{
	std::shared_ptr<Player_Hit> hit = std::make_shared<Player_Hit>();
	if (m_target.expired())return;
	hit->SetTarget(m_target.lock());
	m_target.lock()->Hit(_damage);
	if (m_target.lock()->GetParam().Hp <= 0)
	{
		Crushing();
		return;
	}
	m_target.lock()->SetNextState(hit, Player::Action::HitType);
}

void Player_ActionState::Crushing()
{
	std::shared_ptr<Player_Crushing> crushing = std::make_shared<Player_Crushing>();
	if (m_target.expired())return;
	crushing->SetTarget(m_target.lock());
	crushing->SetObjManager(m_target.lock()->GetObjectManager().lock());
	m_target.lock()->SetNextState(crushing, Player::Action::CrushingType);
}

void Player_ActionState::Teleport()
{
	std::shared_ptr<Player_Teleport> teleport = std::make_shared<Player_Teleport>();
	if (m_target.expired())return;
	teleport->SetTarget(m_target.lock());
	m_target.lock()->SetNextState(teleport, Player::Action::TeleportType);
}
