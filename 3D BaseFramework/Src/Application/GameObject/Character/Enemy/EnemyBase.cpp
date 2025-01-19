#include "EnemyBase.h"

// シーンマネジャ 
#include"../../../Scene/SceneManager.h"
// オブジェクトマネジャ
#include"../../ObjectManager.h"
// プレイヤー
#include"../Player/Player.h"
// ゲームカメラ
#include"../../Camera/GameCamera/GameCamera.h"

void EnemyBase::PostUpdate()
{
	if (m_camera.lock()->GetShakeFlg())return;

	CharacterBase::PostUpdate();

	// 無敵時間減少
	if (m_inviTime > 0)
	{
		m_inviTime--;
	}

	// デバッグ 消滅
	if (GetAsyncKeyState('P') & 0x8000)
	{
		if (m_actionType != Action::CrushingType)
		{
			m_param.Hp = 0;
			CrushingChange();
			return;
		}
	}
}

void EnemyBase::Action()
{
	// 次の行動が決まっていたら
	if (m_NextState)
	{
		// ステート
		m_state = m_NextState;
		// 行動
		m_actionType = m_NextActionType;
		// 切り替えが終わったらリセット
		m_NextState.reset();
	}
	// ステート更新
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
}

void EnemyBase::GroundCheck()
{
	// レイ判定
	KdCollider::RayInfo rayInfo;
	// 座標
	rayInfo.m_pos = m_pos;
	float LitleUP = 1.0f;
	rayInfo.m_pos.y += LitleUP; // 埋まらないために少し上げる
	// 方向
	rayInfo.m_dir = Math::Vector3::Down;
	// 長さ
	rayInfo.m_range = m_gravity + LitleUP;
	// 対象
	rayInfo.m_type = KdCollider::TypeGround;

	//デバッグ用
	//Math::Color color = { 1,1,1,1 };
	//m_pDebugWire->AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range, color);

	std::list<KdCollider::CollisionResult> retRayList; // 当たり判定リスト
	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		// 当たり判定
		ret->Intersects(rayInfo, &retRayList);
	}

	// はみ出し距離
	float _maxOverLap = 0.0f;
	// 当たった座標
	Math::Vector3 _hitPos = Math::Vector3::Zero;
	// ヒットフラグ
	bool _hitFlg = false;

	// 対象選出
	for (auto& ray : retRayList)
	{
		// はみ出し距離が長いほうが対象
		if (_maxOverLap < ray.m_overlapDistance)
		{
			// はみ出し距離
			_maxOverLap = ray.m_overlapDistance;
			// 当たった座標
			_hitPos = ray.m_hitPos;
			// ヒットフラグ
			_hitFlg = true;
		}
	}

	m_groundFlg = false; // 地面フラグ

	// 当たったなら
	if (_hitFlg)
	{
		// 座標
		m_pos = _hitPos;
		// 移動方向
		m_dir.y = 0.0f;
		// 重力
		m_gravity = 0.0f;
		// ジャンプ力
		m_param.JumpPow = 0.0f;
		// 地面フラグ
		m_groundFlg = true;
	}
}

void EnemyBase::BumpCheck()
{
	if (m_actionType == Action::AppealType)return; // 出現演出中ならリターン

	// スフィア判定
	KdCollider::SphereInfo sphereInfo;
	// 中心
	Math::Matrix _mat = m_model->FindWorkNode("spine")->m_worldTransform * (Math::Matrix::CreateTranslation(m_mWorld.Translation()));
	sphereInfo.m_sphere.Center = _mat.Translation();
	// 半径
	sphereInfo.m_sphere.Radius = m_HitSphereSize;
	// 対象
	sphereInfo.m_type = KdCollider::TypeBump;

	//デバッグ用
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, Math::Color{ 0,1,1,1 });

	std::list<KdCollider::CollisionResult>retSphereList; // 当たり判定リスト
	for (auto& ret : SceneManager::Instance().GetObjList()) // キャラクター以外のオブジェクト
	{
		ret->Intersects(sphereInfo, &retSphereList);
	}
	for (auto& ret : SceneManager::Instance().GetEnemyList()) // 敵
	{
		if (ret->IsExpired())continue; // 消滅フラグがたっていたら
		if (m_id == ret->GetID())continue; // IDが自分と同じだったら 
		ret->Intersects(sphereInfo, &retSphereList);
	}
	// プレイヤー
	SceneManager::Instance().GetPlayer()->Intersects(sphereInfo, &retSphereList);

	// はみ出し距離
	float maxOverLap = 0.0f;
	// 当たった方向
	Math::Vector3 HitDir = Math::Vector3::Zero;
	// ヒットフラグ
	bool HitFlg = false;

	// 対象選出
	for (auto& sphere : retSphereList)
	{
		// はみ出し距離が長かったら対象
		if (maxOverLap < sphere.m_overlapDistance)
		{
			// はみ出し距離
			maxOverLap = sphere.m_overlapDistance;
			// 当たった方向
			HitDir = sphere.m_hitDir;
			// ヒットフラグ
			HitFlg = true;
		}
	}

	// 当たったら
	if (HitFlg == true)
	{
		// Y軸は考慮しない
		HitDir.y = 0.0f;
		HitDir.Normalize(); // 正規化
		m_pos += maxOverLap * HitDir; // はみ出し距離分当たった方向に移動
	}
}

const Math::Matrix EnemyBase::GettoTargetRotateYMatrix(std::weak_ptr<Player> _target)
{
	// 現在の方向
	Math::Matrix _nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	Math::Vector3 _nowVec = Math::Vector3::TransformNormal(m_forward, _nowRot);
	_nowVec.Normalize(); // 正規化

	// プレイヤーの方向
	Math::Vector3 _dir = _target.lock()->GetPos() - m_pos;
	_dir.y = 0.0f;
	_dir.Normalize();

	// 内積
	float d = _nowVec.Dot(_dir);
	// -1.0～1.0の間に収める
	d = std::clamp(d, -1.0f, 1.0f);

	// 角度計算
	float _ang = DirectX::XMConvertToDegrees(acos(d));

	// 外積
	Math::Vector3 c = _dir.Cross(_nowVec);
	if (c.y > 0)_ang = 360.0f - _ang;

	// 角度
	float _playerToAng = m_angle.y + _ang;
	if (_playerToAng >= 360.0f)_playerToAng -= 360.0f;
	else if (_playerToAng < 0.0f)_playerToAng += 360.0f;

	return Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_playerToAng)); // プレイヤー方向の回転行列
}

void EnemyBase::StateBase::Damage(std::shared_ptr<EnemyBase> owner, int _damage)
{
	owner->m_param.Hp -= _damage; // ダメージ

	// HPがなくなったら消滅
	if (owner->m_param.Hp <= 0)
	{
		owner->m_param.Hp = 0;
		owner->m_ColorLightFlg = false;
		// Crushing
		owner->CrushingChange();
		return;
	}

	// Hit
	owner->HitChange();
}

// Appeal==========================================================================================
void EnemyBase::Appeal::Enter(std::shared_ptr<EnemyBase> owner)
{
}

void EnemyBase::Appeal::Update(std::shared_ptr<EnemyBase> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("Appeal"))
	{
		owner->SetAnime("Appeal", false, 1.0f);
		// エフェクト
		m_handle = KdEffekseerManager::GetInstance().Play("Enemy/Appeal/MagicDrak.efkefc", owner->m_pos, owner->GetAppealEffectSize(), 1.0f, false).lock()->GetHandle();
		return;
	}

	if (owner->GetIsAnimator() && !KdEffekseerManager::GetInstance().IsPlaying(m_handle))
	{
		// Idol
		owner->IdolChange();
		return;
	}
}

void EnemyBase::Appeal::Exit(std::shared_ptr<EnemyBase> owner)
{
}
//=================================================================================================

// Idol============================================================================================
void EnemyBase::Idol::Enter(std::shared_ptr<EnemyBase> owner)
{
}

void EnemyBase::Idol::Update(std::shared_ptr<EnemyBase> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("Idol"))
	{
		owner->SetAnime("Idol", true, 1.0f);
		return;
	}
}

void EnemyBase::Idol::Exit(std::shared_ptr<EnemyBase> owner)
{
}
//=================================================================================================

// Run=============================================================================================
void EnemyBase::Run::Enter(std::shared_ptr<EnemyBase> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("IdolToRun"))
	{
		owner->SetAnime("IdolToRun", false, 1.5f);

		// 避けるときの方向を決めておく
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

	// アニメーションが終了したら次のフローへ
	if (owner->GetIsAnimator())
	{
		owner->m_flow = KdGameObject::Flow::UpdateType;
		return;
	}

	// 移動
	Move(owner);
}		
		
void EnemyBase::Run::Update(std::shared_ptr<EnemyBase> owner)
{		
	// アニメーション変更
	if (!owner->IsAnimCheck("Run"))
	{
		owner->SetAnime("Run", true, 0.7f);
		return;
	}

	// 移動
	Move(owner);
}		
		
void EnemyBase::Run::Exit(std::shared_ptr<EnemyBase> owner)
{
	// ニアメーション変更
	if (!owner->IsAnimCheck("RunToIdol"))
	{
		owner->SetAnime("RunToIdol", false, 1.5f);
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

void EnemyBase::Run::Move(std::shared_ptr<EnemyBase> owner)
{
	// プレイヤー座標
	std::shared_ptr<Player> _player = owner->m_Target.lock();
	Math::Vector3 _playerPos = _player->GetPos();
	// 自分の座標
	Math::Vector3 _pos = owner->m_pos;
	// プレイヤーへの方向
	Math::Vector3 _moveDir = _playerPos - _pos;
	// プレイヤーとの距離
	float dist = _moveDir.Length();
	_moveDir.Normalize(); // 正規化
	m_moveDir = _moveDir;

	// 攻撃範囲外だったら
	if (dist > owner->m_param.AtkRange)
	{
		// 他の敵との距離判定
		if (owner->m_leaveEnemy.expired())EnemyCheck(owner);
		// 近くにいたら避ける
		if (!owner->m_leaveEnemy.expired())Leave(owner);

		// 移動
		owner->SetMove(m_moveDir);
	}
	// 回転
	owner->Rotate(m_moveDir);
}

bool EnemyBase::Run::PlayerDistCheck(std::shared_ptr<EnemyBase> owner, std::weak_ptr<EnemyBase> leave)
{
	// プレイヤー座標
	std::shared_ptr<Player> _player = owner->GetTarget().lock();
	Math::Vector3 _playerPos = _player->GetPos();
	// 自分の座標
	Math::Vector3 _pos = owner->GetPos();
	// プレイヤーとの距離
	float _dist = (_playerPos - _pos).Length();

	// 避ける対象の座標
	Math::Vector3 _leavePos = leave.lock()->GetPos();
	// 避ける対象との距離
	float _leavedist = (_playerPos - _leavePos).Length();

	// プレイヤーとの距離の方が短ければプレイヤーを優先して移動する
	if (_dist <= _leavedist)return false;

	return true;
}

void EnemyBase::Run::EnemyCheck(std::shared_ptr<EnemyBase> owner)
{
	// スフィア判定
	KdCollider::SphereInfo sphereInfo;
	// 中心
	Math::Matrix _mat = owner->m_model->FindWorkNode("spine")->m_worldTransform * (Math::Matrix::CreateTranslation(owner->m_mWorld.Translation()));
	sphereInfo.m_sphere.Center = _mat.Translation();
	// 半径
	sphereInfo.m_sphere.Radius = owner->m_EnemyCheckRadius;
	// 対象
	sphereInfo.m_type = KdCollider::TypeBump;

	std::list<KdCollider::CollisionResult> _retSphereList; // 当たり判定リスト
	std::vector<std::weak_ptr<EnemyBase>>  _HitEnemyList;  // 対象の敵リスト
	for (auto& enemy : SceneManager::Instance().GetEnemyList())
	{
		if (enemy->GetID() == owner->m_id)continue; // 自分と同じIDなら
		if (!enemy->GetLeave())continue;            // この敵が避けている途中なら
		if (enemy->Intersects(sphereInfo, &_retSphereList))_HitEnemyList.push_back(enemy);
	}

	// はみ出し距離
	float _maxOverLap = 0.0f;
	// ヒットフラグ
	bool _IsHit = false;
	// 対象の敵
	std::weak_ptr<EnemyBase> _HitEnemy;
	// リスト番号
	int   _listNum = 0;

	for (auto& enemy : _retSphereList)
	{
		// はみ出し距離が長い&プレイヤーより近ければ対象
		if (_maxOverLap < enemy.m_overlapDistance &&
			PlayerDistCheck(owner, _HitEnemyList[_listNum]))
		{
			// はみだし距離
			_maxOverLap = enemy.m_overlapDistance;
			// 対象
			_HitEnemy = _HitEnemyList[_listNum];
			// ヒットフラグ
			_IsHit = true;
		}
		// リスト番号更新
		_listNum++;
	}

	// 対象が決まったら
	if (_IsHit)
	{
		// 対象として記憶
		owner->m_leaveEnemy = _HitEnemy;
	}
}

void EnemyBase::Run::Leave(std::shared_ptr<EnemyBase> owner)
{
	if (owner->m_leaveEnemy.expired())return; // 対象が消滅したらリターン
	if (owner->m_leaveEnemy.lock()->GetParam().Hp <= 0)return; // 対象のHPが0ならリターン

	// 自分の座標
	Math::Vector3 _pos = owner->m_pos;
	_pos.y = 0.0f; // Y軸は考慮しない
	// 対象の座標
	Math::Vector3 _leavePos = owner->m_leaveEnemy.lock()->GetPos();
	_leavePos.y = 0.0f; // Y軸は考慮しない
	// 対象との距離
	float         _dist = (_leavePos - _pos).Length();

	// 一定距離離れたらリセット
	if (_dist > owner->m_LeaveDist)
	{
		owner->m_leaveEnemy.reset();
		return;
	}

	// 方向
	Math::Vector3 _dir = Math::Vector3::Zero;
	// プレイヤーを中心に回転して避ける
	_dir = m_leaveDir.TransformNormal(m_leaveDir, owner->GettoTargetRotateYMatrix(owner->GetTarget()));
	// 正規化
	_dir.Normalize();
	m_moveDir = _dir;
}
// ================================================================================================

// Hit=============================================================================================
void EnemyBase::Hit::Enter(std::shared_ptr<EnemyBase> owner)
{
}

void EnemyBase::Hit::Update(std::shared_ptr<EnemyBase> owner)
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
				
void EnemyBase::Hit::Exit(std::shared_ptr<EnemyBase> owner)
{
}
// ================================================================================================

// Crushing========================================================================================
void EnemyBase::Crushing::Enter(std::shared_ptr<EnemyBase> owner)
{				
	owner->m_flow = Flow::UpdateType;
	return;
}				
				
void EnemyBase::Crushing::Update(std::shared_ptr<EnemyBase> owner)
{			
	// アニメーション変更
	if (!owner->IsAnimCheck("Death"))
	{
		owner->SetAnime("Death", false, 1.0f);
		return;
	}

	// ディゾルブが最大になったら消滅
	if (owner->m_dissolve >= 1.0f)
	{
		owner->Expired();
		return;
	}

	owner->CrushingAction();
}				
				
void EnemyBase::Crushing::Exit(std::shared_ptr<EnemyBase> owner)
{
}
// ================================================================================================