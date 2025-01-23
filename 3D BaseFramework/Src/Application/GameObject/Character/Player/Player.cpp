#include "Player.h"

// シーンマネジャ
#include"../../../Scene/SceneManager.h"
// ステージマネジャ
#include"../../StageManager.h"
// オブジェクトマネジャ
#include"../../ObjectManager.h"
// ゲームカメラ
#include"../../Camera/GameCamera/GameCamera.h"
// 敵基底
#include"../Enemy/EnemyBase.h"
// 弾基底
#include"../BulletBase/BulletBase.h"
// 剣
#include"../../Weapon/Sword/Sword.h"
// 盾
#include"../../Weapon/Shield/Shield.h"
// 魔法陣
#include"../../Stage/MagicPolygon/MagicPolygon.h"

void Player::Action()
{
	// 次の行動が決まっていたら
	if (m_NextState != nullptr)
	{
		// 切り替え
		m_state = m_NextState;
		m_actionType = m_NextActionType;
		m_NextState.reset(); // 切り替えが完了したらリセット
	}
	m_state->StateUpdate(shared_from_this()); // ステート更新
}

void Player::PostUpdate()
{
	// テレポートが開放していたら
	if (m_ObjectManager.lock()->GetTeleportFlg())
	{
		NextStageCheck(); // テレポート位置判定
	}

	CharacterBase::PostUpdate();

	// スタミナ回復開始時間が経っていたらスタミナ回復
	if (m_NowStaminaRecoveryTime > 0)m_NowStaminaRecoveryTime--;
	else if (m_param.Sm < m_MaxStamina) { StaminaRecovery(); }
}

void Player::Init()
{
	CharacterBase::Init();
	m_MaxStamina = m_param.Sm;

	m_model->SetModelData("Asset/Models/Character/Player/Player.gltf");
	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	// 待機
	std::shared_ptr<Idol> idol = std::make_shared<Idol>();
	m_state = idol;
	m_actionType = Action::IdolType;
	m_flow = KdGameObject::Flow::UpdateType;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Player", m_model, KdCollider::TypeBump | KdCollider::TypeDamage | KdCollider::TypeEvent);

	m_ObjType = ObjType::oPlayer;
}

void Player::NextStageCheck()
{
	if (SceneManager::Instance().GetEnemyList().size() > 0)return; // ←敵が居たら
	if (m_camera.lock()->GetCameraType() != GameCamera::CameraType::PlayerType)return; // カメラの状態がプレイヤーじゃなかったら
	if (m_actionType == Player::Action::TeleportType)return; // プレイヤーがテレポート開始していたら

	// レイ判定
	KdCollider::RayInfo rayInfo;

	// 開始位置
	rayInfo.m_pos = m_pos;
	float LitleUP = 1.0f;
	rayInfo.m_pos.y += LitleUP; // 埋まらないために少し上げる
	
	// 方向
	rayInfo.m_dir = Math::Vector3::Down;
	
	// 長さ
	rayInfo.m_range = m_gravity + LitleUP;
	
	// 対象
	rayInfo.m_type = KdCollider::TypeEvent;

	//デバッグ用
	//Math::Color color = { 1,1,1,1 };
	//m_pDebugWire->AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range, color);

	// 当たり判定
	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		std::list<KdCollider::CollisionResult> _list;
		if (ret->Intersects(rayInfo, &_list))
		{
			// テレポート切り替え
			TeleportChange();
			return;
		}
	}
}

// スタミナ回復
void Player::StaminaRecovery()
{
	if (m_NowStaminaRecoveryTime > 0)return;
	m_param.Sm++;
	if (m_param.Sm >= m_MaxStamina)m_param.Sm = m_MaxStamina;
}

// 行動切り替え====================================================================================
// 待機
void Player::IdolChange()
{
	m_sword.lock()->ClearTraject(); // 剣の軌跡を削除

	std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
	m_NextState = _idol;
	m_NextActionType = Action::IdolType;
	m_flow = KdGameObject::Flow::UpdateType;
}

// テレポート
void Player::TeleportChange()
{
	m_sword.lock()->ClearTraject(); // 剣の軌跡を削除

	std::shared_ptr<Teleport> _teleport = std::make_shared<Teleport>();
	m_NextState = _teleport;
	m_NextActionType = Action::TeleportType;
	m_flow = KdGameObject::Flow::EnterType;
}
//=================================================================================================

// ダメージ========================================================================================
// 直接攻撃
void Player::Damage(int _damage, std::shared_ptr<EnemyBase> _enemy)
{
	m_state->Damage(shared_from_this(), _damage, _enemy);
}

// 遠距離攻撃
void Player::Damage(int _damage, std::shared_ptr<BulletBase> _bullet)
{
	m_state->Damage(shared_from_this(), _damage, _bullet);
}
//=================================================================================================

// セッター========================================================================================
// ステージマネジャ
void Player::SetStageManager(std::shared_ptr<StageManager> _stage)
{
	m_StageManager = _stage;
}

// 盾
void Player::SetShield(std::shared_ptr<Shield> _shield)
{
	m_shield = _shield;
}

// カウンターの攻撃範囲
void Player::SetCounterRadius(float _radius)
{
	m_CounterRadius = _radius;
}

// パリィした敵のID
void Player::SetParryID(UINT _id)
{
	m_ParryID = _id;
}

// パリィ可能時間
void Player::SetParryTime(int _time)
{
	m_ParryTime = _time;
}

// スタミナ回復開始時間
void Player::SetStaminaRecoveryTime(int _time)
{
	m_StaminaRecoveryTime = _time;
}

// 回避した時のスタミナの減少量
void Player::SetRollStamina(int _rollStamina)
{
	m_RollStamina = _rollStamina;
}

// 次のステート
void Player::SetNextState(std::shared_ptr<StateBase> _next, UINT _action)
{
	// ステート
	m_NextState = _next;
	// 行動タイプ
	m_NextActionType = _action;
}
//=================================================================================================

// ゲッター========================================================================================
// 盾
const std::weak_ptr<Shield>& Player::GetShield() const
{
	return m_shield;
}

// オブジェクトマネジャ
const std::weak_ptr<ObjectManager>& Player::GetObjectManager() const
{
	return m_ObjectManager;
}

// 敵に対する攻撃の高さノード
const Math::Matrix& Player::GetEnemyAttackPointMat() const
{
	return (m_model->FindWorkNode("EnemyAttackPoint")->m_worldTransform) * m_mWorld;
}

// カメラ判定用ノード
const Math::Matrix& Player::GetCameraPointMat() const
{
	return (m_model->FindWorkNode("CameraPoint")->m_worldTransform) * m_mWorld;
}

// カウンターの攻撃範囲
const float& Player::GetCounterRadius() const
{
	return m_CounterRadius;
}

// パリィした敵のID
const UINT& Player::GetParryID() const
{
	return m_ParryID;
}

// 行動タイプ
const UINT& Player::GetActionType() const
{
	return m_actionType;
}

// 最大スタミナ
const int& Player::GetParryTime() const
{
	return m_ParryTime;
}

// 最大スタミナ
const int& Player::GetMaxStamina() const
{
	return m_MaxStamina;
}

// スタミナ回復開始時間
const int& Player::GetStaminaRecoveryTime() const
{
	return m_StaminaRecoveryTime;
}

// 回避した時のスタミナ減少量
const int& Player::GetRollStamina() const
{
	return m_RollStamina;
}
//=================================================================================================

void Player::StateBase::StateUpdate(std::shared_ptr<Player> owner)
{
	// ステート更新
	switch (owner->m_flow)
	{
	case KdGameObject::Flow::EnterType:
		Enter(owner);
		break;
	case KdGameObject::Flow::UpdateType:
		Update(owner);
		break;
	case KdGameObject::Flow::ExitType:
		Exit(owner);
		break;
	}

	if (owner->m_camera.lock()->GetCameraType() != GameCamera::CameraType::PlayerType)return;

	// 行動キー確認
	owner->KeyCheck();

	// 行動キーによってステートの切替
	ChangeState(owner);

	// 長押し防止のため前フレームの行動キーを記録しておく
	owner->m_BeforeKeyType = owner->m_keyType;
}

void Player::StateBase::Damage(std::shared_ptr<Player> owner, int _damage, std::shared_ptr<EnemyBase> _enemy)
{
	// HP減少
	if (owner->m_param.Hp <= _damage)owner->m_param.Hp = 0;
	else { owner->m_param.Hp -= _damage; } // ダメージ
	
	// 死亡
	if (owner->m_param.Hp <= 0)
	{
		// Crushing
		std::shared_ptr<Crushing> _crush = std::make_shared<Crushing>();
		owner->m_NextState = _crush;
		owner->m_NextActionType = Player::Action::CrushingType;
		owner->m_flow = KdGameObject::Flow::UpdateType;
		return;
	}
	
	// Hit
	std::shared_ptr<Hit> _hit = std::make_shared<Hit>();
	owner->m_NextState = _hit;
	owner->m_NextActionType = Player::Action::HitType;
	owner->m_flow = KdGameObject::Flow::UpdateType;
	return;
}

void Player::StateBase::Damage(std::shared_ptr<Player> owner, int _damage, std::shared_ptr<BulletBase> _bullet)
{
	// 弾はオブジェクトに当たったら破壊
	_bullet->SetCrush(true);

	// HP減少
	owner->m_param.Hp -= _damage;
	
	// 死亡
	if (owner->m_param.Hp <= 0)
	{
		// Crushing
		std::shared_ptr<Crushing> _crush = std::make_shared<Crushing>();
		owner->m_NextState = _crush;
		owner->m_NextActionType = Player::Action::CrushingType;
		owner->m_flow = KdGameObject::Flow::UpdateType;
		return;
	}

	// Hit
	std::shared_ptr<Hit> _hit = std::make_shared<Hit>();
	owner->m_NextState = _hit;
	owner->m_NextActionType = Player::Action::HitType;
	owner->m_flow = KdGameObject::Flow::UpdateType;
	return;
}

void Player::StateBase::AttackHit(std::shared_ptr<Player> owner)
{
	// スフィア判定
	std::vector<KdCollider::SphereInfo> sphereInfoList;
	KdCollider::SphereInfo sphereInfo;

	// 座標
	// 剣先
	sphereInfo.m_sphere.Center = owner->GetSword().lock()->GetModelTop().Translation();
	sphereInfoList.push_back(sphereInfo);

	// 剣の中心
	sphereInfo.m_sphere.Center = owner->GetSword().lock()->GetModelCenter().Translation();
	sphereInfoList.push_back(sphereInfo);

	// 剣の鍔（つば）
	sphereInfo.m_sphere.Center = owner->GetSword().lock()->GetModelBottom().Translation();
	sphereInfoList.push_back(sphereInfo);

	for (int i = 0; i < sphereInfoList.size(); ++i)
	{
		// 半径
		sphereInfoList[i].m_sphere.Radius = owner->m_sword.lock()->GetAttackSphereSize();
		
		// 対象
		sphereInfoList[i].m_type = KdCollider::TypeDamage;
	}

	std::list<KdCollider::CollisionResult> retSphereList; //当たり判定リスト
	std::shared_ptr<EnemyBase> hitEnemy; // 当たった敵

	// 剣全体
	for (auto& sword:sphereInfoList)
	{
		// 敵
		for (auto& enemy : SceneManager::Instance().GetEnemyList())
		{
			if (owner->m_ParryID != 0 && enemy->GetID() != owner->m_ParryID)continue; // パリィした場合IDがパリィした敵と一致しなかった場合
			if (enemy->GetParam().Hp <= 0 || enemy->GetActionType() == EnemyBase::Action::AppealType || enemy->GetinviTime() > 0)continue; // 敵が攻撃を受けない状態の場合
			if (!enemy->Intersects(sword, &retSphereList))continue; // 当たらなかった場合
			
			// ヒット処理
			
			hitEnemy = enemy;
			// カメラ振動
			owner->m_camera.lock()->SetShakeFlg(true);
			// ダメージ
			hitEnemy->Damage(owner->m_param.Atk);
			// 無敵時間付与
			hitEnemy->SetInviTime(owner->m_inviAddTime);
			// エフェクト
			KdEffekseerManager::GetInstance().Play("Enemy/Hit/hit_eff.efkefc", retSphereList.begin()->m_hitPos, 1.0f, 0.8f, false);
			// 効果音
			KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/刀で斬る2.WAV", 0.05f, false);
			// 処理が終わったら削除
			retSphereList.erase(retSphereList.begin());
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
	else if (m_keyType & Player::KeyType::MoveKey) // キーから離したら
	{
		m_keyType ^= Player::KeyType::MoveKey;
	}

	//攻撃
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_keyType |= Player::KeyType::AttackKey;
	}
	else if (m_keyType & Player::KeyType::AttackKey) // キーから離したら
	{
		m_keyType ^= Player::KeyType::AttackKey;
	}

	//防御
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_keyType |= Player::KeyType::GuardKey;
	}
	else if (m_keyType & Player::KeyType::GuardKey) // キーから離したら
	{
		m_keyType ^= Player::KeyType::GuardKey;
	}

	//回避
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_keyType |= Player::KeyType::RollKey;
	}
	else if (m_keyType & Player::KeyType::RollKey) // キーから離したら
	{
		m_keyType ^= Player::KeyType::RollKey;
	}
}

// Idol============================================================================================
void Player::Idol::Enter(std::shared_ptr<Player> owner)
{
}

void Player::Idol::Update(std::shared_ptr<Player> owner)
{
	// アニメーション変更
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
	// 回避
	if (owner->m_keyType & Player::KeyType::RollKey && !(owner->m_BeforeKeyType & Player::KeyType::RollKey))
	{
		// スタミナが無かったらリターン
		if (owner->m_param.Sm <= 0)return;

		std::shared_ptr<Roll> _roll = std::make_shared<Roll>();
		owner->m_NextState = _roll;
		owner->m_NextActionType = Player::Action::RollType;
		owner->m_flow = KdGameObject::Flow::EnterType;
		return;
	}
	// 移動
	else if (owner->m_keyType & Player::KeyType::MoveKey)
	{
		std::shared_ptr<Run> _run = std::make_shared<Run>();
		owner->m_NextState = _run;
		owner->m_NextActionType = Player::Action::RunType;
		owner->m_flow = KdGameObject::Flow::EnterType;
		return;
	}
	// 攻撃
	else if (owner->m_keyType & Player::KeyType::AttackKey && !(owner->m_BeforeKeyType & Player::KeyType::AttackKey))
	{
		std::shared_ptr<Attack> _attack = std::make_shared<Attack>();
		owner->m_NextState = _attack;
		owner->m_NextActionType = Player::Action::AttackType;
		owner->m_flow = KdGameObject::Flow::EnterType;
		return;
	}
	// 防御
	else if (owner->m_keyType & Player::KeyType::GuardKey)
	{
		std::shared_ptr<Guard> _guard = std::make_shared<Guard>();
		owner->m_NextState = _guard;
		owner->m_NextActionType = Player::Action::GuardType;
		owner->m_flow = KdGameObject::Flow::EnterType;
		return;
	}

}
//=================================================================================================


// Run=============================================================================================
void Player::Run::Enter(std::shared_ptr<Player> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("IdolToRun"))
	{
		owner->SetAnime("IdolToRun", false, 3.0f);
		return;
	}

	// アニメーションが終わったら次のフローへ
	if (owner->GetIsAnimator())
	{
		owner->m_flow = Player::Flow::UpdateType;
		return;
	}

	// 移動キーから離したら終了処理に移る
	if (!(owner->m_keyType & Player::KeyType::MoveKey))
	{
		owner->m_flow = Flow::ExitType;
		return;
	}

	// 移動処理
	Move(owner);
}

void Player::Run::Update(std::shared_ptr<Player> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("Run"))
	{
		owner->SetAnime("Run", true, 1.5f);
		return;
	}

	// 移動キーから離したら終了処理に移る
	if (!(owner->m_keyType & Player::KeyType::MoveKey))
	{
		owner->m_flow = Player::Flow::ExitType;
		return;
	}

	// 移動処理
	Move(owner);
}

void Player::Run::Exit(std::shared_ptr<Player> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("RunToIdol"))
	{
		owner->SetAnime("RunToIdol", false, 3.0f);
		return;
	}

	// アニメーションが終了したら待機状態へ
	if (owner->GetIsAnimator())
	{
		// Idol
		owner->IdolChange();
		return;
	}

	// 移動キーを押したら最初のフローへ
	if (owner->m_keyType & Player::MoveKey)
	{
		owner->m_flow = Player::Flow::EnterType;
		return;
	}
}

void Player::Run::Move(std::shared_ptr<Player> owner)
{
	// 方向
	Math::Vector3 dir = Math::Vector3::Zero;
	if (GetAsyncKeyState('W') & 0x8000) // 前
	{
		dir.z = 1.0f;
	}
	if (GetAsyncKeyState('S') & 0x8000) // 後
	{
		dir.z = -1.0f;
	}
	if (GetAsyncKeyState('A') & 0x8000) // 左
	{
		dir.x = -1.0f;
	}
	if (GetAsyncKeyState('D') & 0x8000) // 右
	{
		dir.x = 1.0f;
	}
	if (dir == Math::Vector3::Zero)return;
	owner->CameraTransform(dir); // カメラの方向によって方向修正
	owner->Rotate(dir);          // 回転
	owner->SetMove(dir);         // 移動
}

void Player::Run::ChangeState(std::shared_ptr<Player> owner)
{
	// 攻撃
	if (owner->m_keyType & Player::KeyType::AttackKey && !(owner->m_BeforeKeyType & Player::KeyType::AttackKey))
	{
		std::shared_ptr<Attack> _attack = std::make_shared<Attack>();
		owner->m_NextState = _attack;
		owner->m_NextActionType = Player::Action::AttackType;
		owner->m_flow = KdGameObject::Flow::EnterType;
		return;
	}
	// 防御
	else if (owner->m_keyType & Player::KeyType::GuardKey)
	{
		std::shared_ptr<Guard> _guard = std::make_shared<Guard>();
		owner->m_NextState = _guard;
		owner->m_NextActionType = Player::Action::GuardType;
		owner->m_flow = KdGameObject::Flow::EnterType;
		return;
	}
	// 回避
	else if (owner->m_keyType & Player::KeyType::RollKey && !(owner->m_BeforeKeyType & Player::KeyType::RollKey))
	{
		// スタミナが無かったらリターン
		if (owner->m_param.Sm <= 0)return;

		std::shared_ptr<Roll> _roll = std::make_shared<Roll>();
		owner->m_NextState = _roll;
		owner->m_NextActionType = Player::Action::RollType;
		owner->m_flow = KdGameObject::Flow::EnterType;
		return;
	}
}
//=================================================================================================


// Attack==========================================================================================
void Player::Attack::Enter(std::shared_ptr<Player> owner)
{
	// 攻撃方向確認
	AttackDirCheck(owner);
	
	if (owner->GetSword().expired())return;
	
	// 回転
	owner->Rotate(m_AttackDir, 360.0f);

	// 攻撃フレーム切り替え
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

	owner->GetSword().lock()->MakeTraject(); // 剣線作成
	owner->m_flow = Player::Flow::UpdateType;
}

void Player::Attack::Update(std::shared_ptr<Player> owner)
{
	// アニメーション変更
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

	// 各攻撃の処理
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

	// アニメーションが終了したら待機状態
	if (owner->GetIsAnimator())
	{
		// Idol
		owner->IdolChange();
		return;
	}

	// FPS加算
	m_ActionFPS++;
}

void Player::Attack::Exit(std::shared_ptr<Player> owner)
{

}

void Player::Attack::Attack1(std::shared_ptr<Player> owner)
{
	// 移動
	owner->SetMove(m_AttackDir, 0.5f);

	// 剣線削除
	if (m_ActionFPS > 30 && owner->GetSword().lock()->GetTrajectPolygon())
	{
		owner->GetSword().lock()->ClearTraject();
		return;
	}

	if (m_ActionFPS < 15 || m_ActionFPS > 30)return;
	// 攻撃判定
	AttackHit(owner);

	if (owner->GetSword().expired())return;
	// 剣線追加
	owner->GetSword().lock()->SetTrajectMat();
}

void Player::Attack::Attack2(std::shared_ptr<Player> owner)
{
	// 移動
	owner->SetMove(m_AttackDir, 1.0f);

	// 剣線削除
	if (m_ActionFPS > 25 && owner->GetSword().lock()->GetTrajectPolygon())
	{
		owner->GetSword().lock()->ClearTraject();
		return;
	}

	if (m_ActionFPS < 10 || m_ActionFPS > 25)return;
	// 攻撃判定
	AttackHit(owner);

	if (owner->GetSword().expired())return;
	// 剣線追加
	owner->GetSword().lock()->SetTrajectMat();
}

void Player::Attack::Attack3(std::shared_ptr<Player> owner)
{
	// 移動
	owner->SetMove(m_AttackDir, 1.2f);

	// 剣線削除
	if (m_ActionFPS > 36 && owner->GetSword().lock()->GetTrajectPolygon())
	{
		owner->GetSword().lock()->ClearTraject();
		return;
	}

	if (m_ActionFPS < 12 || m_ActionFPS > 36)return;
	// 攻撃判定
	AttackHit(owner);

	if (owner->GetSword().expired())return;
	// 剣線追加
	owner->GetSword().lock()->SetTrajectMat();
}

void Player::Attack::AttackDirCheck(std::shared_ptr<Player> owner)
{
	// 方向
	m_AttackDir = Math::Vector3::Zero;

	if (GetAsyncKeyState('W') & 0x8000) //前
	{
		m_AttackDir.z = 1.0f;
	}
	if (GetAsyncKeyState('S') & 0x8000) //後
	{
		m_AttackDir.z = -1.0f;
	}
	if (GetAsyncKeyState('A') & 0x8000) //左
	{
		m_AttackDir.x = -1.0f;
	}
	if (GetAsyncKeyState('D') & 0x8000) //右
	{
		m_AttackDir.x = 1.0f;
	}

	// どのキーも押していなかったら現在向いている方向or近くの敵の方向を向く
	if (m_AttackDir == Math::Vector3::Zero)
	{
		if (SceneManager::Instance().GetEnemyList().size() > 0)
		{
			float _enemyDist = 0.0f; // 敵の距離
			for (auto& enemy : SceneManager::Instance().GetEnemyList())
			{
				float _dist = (enemy->GetPos() - owner->GetPos()).Length(); // 距離計算
				if (m_AttackDir != Math::Vector3::Zero && _dist > _enemyDist)continue; 
				// 攻撃方向が決まっていないor距離が前の敵よりも近かったら
				
				// 距離更新
				_enemyDist = _dist;
				// 方向更新
				m_AttackDir = enemy->GetPos() - owner->GetPos();
				m_AttackDir.y = 0.0f;
				m_AttackDir.Normalize();
			}
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
			cameraRotYMat = owner->m_camera.lock()->GetRotationYMatrix(); // カメラの回転行列
		}
		m_AttackDir = m_AttackDir.TransformNormal(m_AttackDir, cameraRotYMat); // カメラ方向によって攻撃方向を修正
	}

	m_AttackDir.Normalize(); //正規化
}

void Player::Attack::ChangeState(std::shared_ptr<Player> owner)
{
	// 切り替え可能ではなかったらリターン
	if (m_ActionFPS < m_ChangeTime)return;

	// 攻撃
	if (owner->m_keyType & Player::KeyType::AttackKey && !(owner->m_BeforeKeyType & Player::KeyType::AttackKey))
	{
		// 剣線削除
		owner->GetSword().lock()->ClearTraject();

		// 攻撃段階加算
		m_atkNum++;
		// 最初に戻る
		owner->m_flow = Player::Flow::EnterType;
		// 最大段階まできたら最初の段階に戻る
		if (m_atkNum > AttackNUM)m_atkNum = 1;
		// FPSリセット
		m_ActionFPS = 0;
	}
	// 防御
	else if (owner->m_keyType & Player::KeyType::GuardKey)
	{
		// 剣線削除
		owner->GetSword().lock()->ClearTraject();

		std::shared_ptr<Guard> _guard = std::make_shared<Guard>();
		owner->m_NextState = _guard;
		owner->m_NextActionType = Player::Action::GuardType;
		owner->m_flow = KdGameObject::Flow::EnterType;
		return;
	}
	// 回避
	else if (owner->m_keyType & Player::KeyType::RollKey && !(owner->m_BeforeKeyType & Player::KeyType::RollKey))
	{
		// スタミナが無かったらリターン
		if (owner->m_param.Sm <= 0)return;
		// 剣線削除
		owner->GetSword().lock()->ClearTraject();

		std::shared_ptr<Roll> _roll = std::make_shared<Roll>();
		owner->m_NextState = _roll;
		owner->m_NextActionType = Player::Action::RollType;
		owner->m_flow = KdGameObject::Flow::EnterType;
		return;
	}
}

void Player::Attack::Damage(std::shared_ptr<Player> owner, int _damage, std::shared_ptr<EnemyBase> _enemy)
{
	// 剣線削除
	owner->GetSword().lock()->ClearTraject();
	StateBase::Damage(owner, _damage, _enemy);
}

void Player::Attack::Damage(std::shared_ptr<Player> owner, int _damage, std::shared_ptr<BulletBase> _bullet)
{
	// 剣線削除
	owner->GetSword().lock()->ClearTraject();
	StateBase::Damage(owner, _damage, _bullet);
}
//=================================================================================================


// Counter==========================================================================================
void Player::Counter::Enter(std::shared_ptr<Player> owner)
{
	// カウンターの対象を検知
	for (auto& enemy : SceneManager::Instance().GetEnemyList())
	{
		if (enemy->GetID() != owner->m_ParryID)continue; // 対象のIDと違うなら次へ
		m_CounterEnemy = enemy;
	}

	if (owner->GetSword().expired() == false)
	{
		owner->GetSword().lock()->MakeTraject(); // 剣線作成
	}
	owner->m_flow = Player::Flow::UpdateType; // 次のフローへ
	return;
}

void Player::Counter::Update(std::shared_ptr<Player> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("Counter"))
	{
		owner->SetAnime("Counter", false, 1.5f);
		return;
	}

	// アニメーションが終了したら
	if (owner->GetIsAnimator())
	{
		owner->m_ParryID = 0; // 対象IDをリセット

		// Idol
		owner->IdolChange();
		return;
	}

	// 剣線削除
	if (m_ActionFPS > 70 && owner->GetSword().lock()->GetTrajectPolygon())
	{
		owner->GetSword().lock()->ClearTraject();
	}

	// エフェクト
	if (m_ActionFPS == 44)
	{
		KdEffekseerManager::GetInstance().Play("Player/Counter/CounterImpact/CounterImpact.efkefc", owner->m_pos, 2.0f, 1.0f, false);
	}

	if (44 <= m_ActionFPS && m_ActionFPS <= 70)
	{
		// 攻撃判定
		AttackHit(owner);
		// 剣線追加
		owner->GetSword().lock()->SetTrajectMat();
	}

	// 敵の近くに移動
	CounterMove(owner);

	// FPS加算
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
	// 敵が攻撃を受けない状態ならリターン
	if (m_CounterEnemy.lock()->GetParam().Hp <= 0 || m_CounterEnemy.lock()->GetinviTime() > 0)return;

	// スフィア判定
	std::vector<KdCollider::SphereInfo> sphereInfoList;
	KdCollider::SphereInfo sphereInfo;

	// 座標
	// 剣先
	sphereInfo.m_sphere.Center = owner->GetSword().lock()->GetModelTop().Translation();
	sphereInfoList.push_back(sphereInfo);
	// 剣の中心
	sphereInfo.m_sphere.Center = owner->GetSword().lock()->GetModelCenter().Translation();
	sphereInfoList.push_back(sphereInfo);
	// 剣の鍔（つば）
	sphereInfo.m_sphere.Center = owner->GetSword().lock()->GetModelBottom().Translation();
	sphereInfoList.push_back(sphereInfo);

	for (unsigned int i = 0; i < sphereInfoList.size(); ++i)
	{
		// 半径
		sphereInfoList[i].m_sphere.Radius = owner->GetSword().lock()->GetAttackSphereSize();
		// 対象
		sphereInfoList[i].m_type = KdCollider::TypeDamage;
	}

	std::list<KdCollider::CollisionResult> retSphereList;

	for (unsigned int i = 0; i < sphereInfoList.size(); ++i)
	{
		// 攻撃判定
		if (!m_CounterEnemy.lock()->Intersects(sphereInfoList[i], &retSphereList))continue;
		
		// カメラ振動
		owner->m_camera.lock()->SetShakeMove(0.5f);
		owner->m_camera.lock()->SetShakeFlg(true);
		// ダメージ
		m_CounterEnemy.lock()->Damage(owner->m_param.Atk * 5);
		// 無敵時間付与
		m_CounterEnemy.lock()->SetInviTime(owner->m_inviAddTime);
		// エフェクト
		m_handle = KdEffekseerManager::GetInstance().Play("Player/Counter/CounterHit/CounterHit.efkefc", retSphereList.begin()->m_hitPos, 5.0f, 0.8f, false).lock()->GetHandle();
		// エフェクトのプレイヤーの正面に向ける
		KdEffekseerManager::GetInstance().SetRotation(m_handle, Math::Vector3{ 0.0f,1.0f,0.0f }, DirectX::XMConvertToRadians(owner->GetAngle().y));
		// 効果音
		KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/刀で斬る2.WAV", 0.05f, false);
		return;;
	}
}

void Player::Counter::CounterMove(std::shared_ptr<Player> owner)
{
	// 対象の方向
	Math::Vector3 _dir = m_CounterEnemy.lock()->GetPos() - owner->m_pos;
	_dir.Normalize(); // 正規化
	owner->Rotate(_dir); // 回転

	// スフィア判定
	KdCollider::SphereInfo sphereInfo;
	// 座標
	sphereInfo.m_sphere.Center = owner->m_pos;
	// 半径
	sphereInfo.m_sphere.Radius = owner->m_CounterRadius;
	// 対象
	sphereInfo.m_type = KdCollider::TypeBump;

	// 判定
	if (m_CounterEnemy.lock()->Intersects(sphereInfo, nullptr))return;

	// 近くに居なかったら移動
	owner->SetMove(_dir,10.0f);
}
//=================================================================================================


// Roll============================================================================================
void Player::Roll::Enter(std::shared_ptr<Player> owner)
{
	// 方向
	Math::Vector3 dir = Math::Vector3::Zero;
	if (GetAsyncKeyState('W') & 0x8000) // 前
	{
		dir.z = 1.0f;
	}
	if (GetAsyncKeyState('S') & 0x8000) // 後
	{
		dir.z = -1.0f;
	}
	if (GetAsyncKeyState('A') & 0x8000) // 左
	{
		dir.x = -1.0f;
	}
	if (GetAsyncKeyState('D') & 0x8000) // 右
	{
		dir.x = 1.0f;
	}
	if (dir != Math::Vector3::Zero)
	{
		owner->CameraTransform(dir); // カメラの方向によって方向を修正
		owner->Rotate(dir, 360.0f);  // 回転
	}

	// スタミナ減少
	unsigned int _stamina = owner->m_RollStamina;
	if (owner->m_param.Sm <= _stamina)_stamina = owner->m_param.Sm;
	owner->m_param.Sm -= _stamina;
	// スタミナ回復可能時間増加
	owner->m_NowStaminaRecoveryTime = owner->m_StaminaRecoveryTime;
	// 次のフローへ
	owner->m_flow = Player::Flow::UpdateType;
}

void Player::Roll::Update(std::shared_ptr<Player> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("Roll"))
	{
		owner->SetAnime("Roll", false, 1.0f);
		// エフェクト
		KdEffekseerManager::GetInstance().Play("Player/Smoke/Smoke.efkefc", owner->m_pos, 0.5f, 1.0f, false);
		return;
	}

	// アニメーションが終了したら待機状態
	if (owner->GetIsAnimator())
	{
		// Idol
		owner->IdolChange();
		return;
	}

	if (m_ActionFPS < 32)RollMove(owner); // 移動

	// FPS加算
	m_ActionFPS++;
}

void Player::Roll::Exit(std::shared_ptr<Player> owner)
{

}

void Player::Roll::RollMove(std::shared_ptr<Player> owner)
{
	// 方向
	Math::Vector3 dir = Math::Vector3::Zero;

	//今の方向
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->m_angle.y));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(owner->m_forward, nowRot);
	dir = nowVec;
	dir.Normalize(); // 正規化

	// 移動
	owner->SetMove(dir, 2.0f);
}

void Player::Roll::ChangeState(std::shared_ptr<Player> owner)
{
}
//=================================================================================================


// Guard===========================================================================================
void Player::Guard::Enter(std::shared_ptr<Player> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("IdolToGuard"))
	{
		owner->SetAnime("IdolToGuard", false, 1.0f);
		return;
	}

	// アニメーションが終了したら次のフローへ
	if (owner->GetIsAnimator())
	{
		owner->m_flow = Player::Flow::UpdateType;
		return;
	}

	// 防御キーを離したら終了処理へ
	if (!(owner->m_keyType & Player::KeyType::GuardKey))
	{
		owner->m_flow = Player::Flow::ExitType;
		return;
	}

	// 経過時間加算
	m_guardTime++;
}

void Player::Guard::Update(std::shared_ptr<Player> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("Guard"))
	{
		owner->SetAnime("Guard", true, 1.0f);
		return;
	}

	// 防御キーを離したら終了処理へ
	if (!(owner->m_keyType & Player::KeyType::GuardKey))
	{
		owner->m_flow = Player::Flow::ExitType;
		return;
	}

	// 経過時間加算
	m_guardTime++;
}

void Player::Guard::Exit(std::shared_ptr<Player> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("GuardToIdol"))
	{
		owner->SetAnime("GuardToIdol", false, 1.0f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		owner->IdolChange();
		return;
	}

	// 防御キーを押したら最初へ戻る
	if (owner->m_keyType & Player::KeyType::GuardKey)
	{
		owner->m_flow = Player::Flow::EnterType;
		return;
	}
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
	// 攻撃してきた敵の方向
	Math::Vector3 toVec = _enemy->GetPos() - owner->m_pos;
	toVec.y = 0.0f;    // Y軸は考慮しない
	toVec.Normalize(); // 正規化
	GuardRotate(owner, toVec); // 回転

	if (m_guardTime <= owner->m_ParryTime)
	{
		// スローにする
		if(!owner->m_ObjectManager.lock()->GetSlowFlg())owner->m_ObjectManager.lock()->SlowChange();
		
		// Parry
		std::shared_ptr<Parry> _parry = std::make_shared<Parry>();
		owner->m_NextState = _parry;
		owner->m_NextActionType = Player::Action::ParryType;
		owner->m_flow = KdGameObject::Flow::UpdateType;
		// IDを記録
		owner->m_ParryID = _enemy->GetID();
		// 敵はよろめく
		_enemy->StumbleChange();
	}
	else
	{
		// スタミナが無かったらダメージを受ける
		if (owner->m_param.Sm <= 0)
		{
			StateBase::Damage(owner, _damage, _enemy);
		}
		else
		{
			// スタミナ減少
			unsigned int _stamina = _damage * owner->m_GuardStaminaCorrection;
			if (owner->m_param.Sm <= _stamina)_stamina = owner->m_param.Sm;
			owner->m_param.Sm -= _stamina;
			// スタミナ可能時間加算
			owner->m_NowStaminaRecoveryTime = owner->m_StaminaRecoveryTime;

			// GuardReaction
			std::shared_ptr<GuardReaction> _reaction = std::make_shared<GuardReaction>();
			owner->m_NextState = _reaction;
			owner->m_NextActionType = Player::Action::GuardReactionType;
			owner->m_flow = KdGameObject::Flow::UpdateType;
		}
	}
}

void Player::Guard::Damage(std::shared_ptr<Player> owner, int _damage, std::shared_ptr<BulletBase> _bullet)
{
	if (m_guardTime <= owner->m_ParryTime)
	{
		// 攻撃してきた敵の方向
		Math::Vector3 toVec = _bullet->GetOwner().lock()->GetPos() - owner->m_pos;
		toVec.y = 0.0f; // Y軸は考慮しない
		toVec.Normalize(); // 正規化
		GuardRotate(owner, toVec); // 回転

		// スローにする
		if (!owner->m_ObjectManager.lock()->GetSlowFlg())owner->m_ObjectManager.lock()->SlowChange();

		// Parry
		std::shared_ptr<Parry> _parry = std::make_shared<Parry>();
		owner->m_NextState = _parry;
		owner->m_NextActionType = Player::Action::ParryType;
		owner->m_flow = KdGameObject::Flow::UpdateType;

		// 跳ね返し
		// 飛ばしてきた敵の方向
		Math::Matrix RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->m_angle.y));
		Math::Vector3 _dir = Math::Vector3::TransformNormal(owner->m_forward, RotY);
		_bullet->SetDir(_dir); // 方向
		_bullet->SetOwner(owner); // 弾の持ち主をプレイヤーに
	}
	else
	{
		// 弾が飛んできた方向を向く
		Math::Vector3 _bulletDir = _bullet->GetDir();
		_bulletDir *= -1.0f; // 弾の移動方向の逆の方向
		_bulletDir.Normalize(); // 正規化
		GuardRotate(owner, _bulletDir); // 回転

		// スタミナが無かったらダメージを受ける
		if (owner->m_param.Sm <= 0)
		{
			StateBase::Damage(owner, _damage, _bullet);
		}
		else
		{
			// スタミナ減少
			unsigned int _stamina = _damage * owner->m_GuardStaminaCorrection;
			if (owner->m_param.Sm <= _stamina)_stamina = owner->m_param.Sm;
			owner->m_param.Sm -= _stamina;
			// スタミナ可能時間加算
			owner->m_NowStaminaRecoveryTime = owner->m_StaminaRecoveryTime;

			// GuardReaction
			std::shared_ptr<GuardReaction> _reaction = std::make_shared<GuardReaction>();
			owner->m_NextState = _reaction;
			owner->m_NextActionType = Player::Action::GuardReactionType;
			owner->m_flow = KdGameObject::Flow::UpdateType;

			// 弾破壊
			_bullet->SetCrush(true);
		}
	}
}

// ガード時間
void Player::Guard::SetGuardTime(int _time)
{
	m_guardTime = _time;
}
//=================================================================================================


// GuardReaction===================================================================================
void Player::GuardReaction::Enter(std::shared_ptr<Player> owner)
{
}

void Player::GuardReaction::Update(std::shared_ptr<Player> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("GuardReaction"))
	{
		owner->SetAnime("GuardReaction", false, 1.0f);
		// エフェクト
		m_handle=KdEffekseerManager::GetInstance().Play("Player/Guard/Spark.efkefc",owner->GetShieldMat().Translation(), 1.0f, 1.0f, false).lock()->GetHandle();
		// プレイヤーが向いている方向へ回転
		KdEffekseerManager::GetInstance().SetRotation(m_handle, Math::Vector3{ 0.0f,1.0f,0.0f }, DirectX::XMConvertToRadians((owner->GetAngle().y) + 180.0f));
		// 効果音
		KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/ロボットを殴る1.WAV", 0.05f, false);
		return;
	}

	// アニメーションが終了したら防御へ
	if (owner->GetIsAnimator())
	{
		std::shared_ptr<Guard> _guard = std::make_shared<Guard>();
		_guard->SetGuardTime(owner->GetParryTime() + 1); // パリィ可能時間の１フレーム先に設定　※パリィ防止
		owner->m_NextState = _guard;
		owner->m_NextActionType = Player::Action::GuardType;
		owner->m_flow = KdGameObject::Flow::UpdateType;
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
	// アニメーション変更
	if (!owner->IsAnimCheck("Parrying"))
	{
		owner->SetAnime("Parrying", false, 1.0f);
		// エフェクト
		KdEffekseerManager::GetInstance().Play("Player/Parry/hit_hanmado_0409.efkefc", owner->GetShield().lock()->GetParryPoint().Translation(), 1.0f, 0.5f, false);
		// 効果音
		KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/maou_se_magic_ice05.WAV", 0.1f, false);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// IDをリセット
		owner->m_ParryID = 0;

		// Idol
		owner->IdolChange();

		// スローを解除
		if (owner->m_ObjectManager.lock()->GetSlowFlg())owner->m_ObjectManager.lock()->SlowChange();
		return;
	}

	// FPS加算
	m_ActionFPS++;
}

void Player::Parry::Exit(std::shared_ptr<Player> owner)
{

}

void Player::Parry::ChangeState(std::shared_ptr<Player> owner)
{
	if (m_ActionFPS < 30)return;

	// 攻撃orカウンター
	if (owner->m_keyType & Player::KeyType::AttackKey && !(owner->m_BeforeKeyType & Player::KeyType::AttackKey))
	{
		// IDが記録されていたらカウンター
		if (owner->m_ParryID != 0)
		{
			// カウンター
			std::shared_ptr<Counter> _counter = std::make_shared<Counter>();
			owner->m_NextState = _counter;
			owner->m_NextActionType = Player::Action::CounterType;
			owner->m_flow = KdGameObject::Flow::EnterType;
		}
		else 
		{
			// 攻撃
			std::shared_ptr<Attack> _attack = std::make_shared<Attack>();
			owner->m_NextState = _attack;
			owner->m_NextActionType = Player::Action::AttackType;
			owner->m_flow = KdGameObject::Flow::EnterType;
		}

		// スロー解除
		if(owner->m_ObjectManager.lock()->GetSlowFlg())owner->m_ObjectManager.lock()->SlowChange();
		return;
	}

	// 移動
	if (owner->m_keyType & Player::KeyType::MoveKey && !(owner->m_BeforeKeyType & Player::KeyType::MoveKey))
	{
		// IDリセット
		owner->m_ParryID = 0;

		std::shared_ptr<Run> _run = std::make_shared<Run>();
		owner->m_NextState = _run;
		owner->m_NextActionType = Player::Action::RunType;
		owner->m_flow = KdGameObject::Flow::EnterType;
		// スロー解除
		if (owner->m_ObjectManager.lock()->GetSlowFlg())owner->m_ObjectManager.lock()->SlowChange();
		return;
	}
	else if (owner->m_keyType & Player::KeyType::GuardKey)
	{
		// IDリセット
		owner->m_ParryID = 0;

		std::shared_ptr<Guard> _guard = std::make_shared<Guard>();
		owner->m_NextState = _guard;
		owner->m_NextActionType = Player::Action::GuardType;
		owner->m_flow = KdGameObject::Flow::EnterType;
		// スロー解除
		if (owner->m_ObjectManager.lock()->GetSlowFlg())owner->m_ObjectManager.lock()->SlowChange();
		return;
	}
	else if (owner->m_keyType & Player::KeyType::RollKey && !(owner->m_BeforeKeyType & Player::KeyType::RollKey))
	{
		if (owner->m_param.Sm <= 0)return;
		// IDリセット
		owner->m_ParryID = 0;

		std::shared_ptr<Roll> _roll = std::make_shared<Roll>();
		owner->m_NextState = _roll;
		owner->m_NextActionType = Player::Action::RollType;
		owner->m_flow = KdGameObject::Flow::EnterType;
		// スロー解除
		if (owner->m_ObjectManager.lock()->GetSlowFlg())owner->m_ObjectManager.lock()->SlowChange();
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
	// アニメーション変更
	if (!owner->IsAnimCheck("Hit"))
	{
		owner->SetAnime("Hit", false, 1.5f);
		return;
	}

	// アニメーションが終了したら待機状態へ
	if (owner->GetIsAnimator())
	{
		// Idol
		owner->IdolChange();
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
	// アニメーション変更
	if (!owner->IsAnimCheck("Death"))
	{
		owner->SetAnime("Death", false, 1.0f);
		return;
	}

	// ディゾルブが最大になったら消滅
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
	// 魔法陣の中心へ移動
	// プレイヤー位置
	Math::Vector3 _playerPos = owner->GetPos();
	_playerPos.y = 0.0f;
	// 魔法陣位置
	Math::Vector3 _magicPolygonPos = owner->m_ObjectManager.lock()->GetMagicPolygon().lock()->GetPos();
	_magicPolygonPos.y = 0.0f;
	
	// 中心へ来たら次のフローへ
	if (_playerPos.x >= _magicPolygonPos.x - 0.5f && _playerPos.x <= _magicPolygonPos.x + 0.5f && 
		_playerPos.z >= _magicPolygonPos.z - 0.5f && _playerPos.z <= _magicPolygonPos.z + 0.5f)
	{
		owner->m_flow = Player::Flow::UpdateType;
	}

	// 移動アニメーション
	if (!owner->IsAnimCheck("Run"))owner->SetAnime("Run", true, 1.0f);

	// 方向
	Math::Vector3 _dir = _magicPolygonPos - _playerPos;
	_dir.y = 0.0f;
	float _dist = _dir.Length();
	_dir.Normalize(); // 正規化

	float _speed = owner->GetParam().Sp;
	if (_dist < _speed)_speed = _dist; // 距離が素早さより小さければ距離を素早さにする
	owner->SetMove(_dir, _speed); // 移動
	owner->Rotate(_dir); // 回転
}

void Player::Teleport::Update(std::shared_ptr<Player> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("Teleport"))
	{
		owner->SetAnime("Teleport", true, 1.0f);
		return;
	}

	// テレポートしたかのようにプレイヤーの姿を消す
	if (m_ActionFPS == 38)
	{
		// ディゾルブを最大値へ
		owner->m_dissolve = 1.0f;
		// エフェクト
		m_handle = KdEffekseerManager::GetInstance().Play("Player/Teleport/LightEnd.efkefc", owner->m_pos, owner->m_size, 1.0f, false).lock()->GetHandle();
		// SE
		KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/ワープ.wav", 0.1f, false);
	}

	// テレポートのエフェクトが終了したらフェードアウト
	if (m_ActionFPS >= 38 && !KdEffekseerManager::GetInstance().IsPlaying(m_handle))
	{
		SceneManager::Instance().BlackAlphaChange(0.01f, true);
		// 次のフローへ
		owner->m_flow = Player::Flow::ExitType;
	}

	// FPS加算
	m_ActionFPS++;
}

void Player::Teleport::Exit(std::shared_ptr<Player> owner)
{
	if (owner->m_ObjectManager.lock()->GetTeleportFlg())return;
	// テレポートが終わったら

	// アニメーション変更
	if (!owner->IsAnimCheck("TeleportToIdol"))
	{
		owner->SetAnime("TeleportToIdol", false, 1.0f);
		// ディゾルブを戻す
		owner->m_dissolve = 0.0f;
		// エフェクト
		KdEffekseerManager::GetInstance().Play("Player/Teleport/LightEnd.efkefc", owner->m_pos, owner->m_size, 1.0f, false);
		// SE
		KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/ワープ.wav", 0.1f, false);
		return;
	}

	// アニメーションが終了したら待機状態へ
	if (owner->GetIsAnimator())
	{
		// Idol
		owner->IdolChange();
		return;
	}
}

void Player::Teleport::ChangeState(std::shared_ptr<Player> owner)
{
}
//=================================================================================================