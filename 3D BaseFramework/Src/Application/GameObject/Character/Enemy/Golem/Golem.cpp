#include "Golem.h"

// シーンマネジャ
#include"../../../../Scene/SceneManager.h"
// オブジェクトマネジャ
#include"../../../ObjectManager.h"
// 敵基底
#include"../EnemyBase.h"
// 弾
#include"Bullet/Bullet.h"
// プレイヤー
#include"../../Player/Player.h"
// ゲームカメラ
#include"../../../Camera/GameCamera/GameCamera.h"

void Golem::Action()
{
	// 次の行動が決まっていたら
	if (m_NextState != nullptr)
	{
		// 行動
		m_state = m_NextState;
		// 行動タイプ
		m_actionType = m_NextActionType;
		// 切り替えが終わったらリセット
		m_NextState.reset();
	}
	// 行動更新
	m_state->StateUpdate(shared_from_this());
}

void Golem::PostUpdate()
{
	if (m_camera.lock()->GetShakeFlg())return;

	EnemyBase::PostUpdate();

	// 攻撃範囲表示
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

	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_model, KdCollider::TypeDamage | KdCollider::TypeBump | KdCollider::TypeSight);
}

// 行動切り替え==========================================================================================
// 攻撃
void Golem::AttackChange()
{
	Math::Vector3 _playerPos = m_Target.lock()->GetPos();
	float _dist = (_playerPos - m_pos).Length();

	if (_dist > m_Attack2Dist && m_AttackType != AttackKind::JumpType)
	{
		// ジャンプ攻撃
		std::shared_ptr<Attack2> _attack2 = std::make_shared<Attack2>();
		m_NextState = _attack2;
		m_NextActionType = m_NextActionType = EnemyBase::Action::AttackType;
		m_flow = EnemyBase::Flow::EnterType;
		m_AttackType = AttackKind::JumpType;
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

	if (fabs(ang) <= m_Attack1Angle && m_AttackType != AttackKind::BulletType)
	{
		// 弾攻撃 
		std::shared_ptr<Attack1> _attack1 = std::make_shared<Attack1>();
		m_NextState = _attack1;
		m_NextActionType = m_NextActionType = EnemyBase::Action::AttackType;
		m_flow = EnemyBase::Flow::EnterType;
		m_AttackType = AttackKind::BulletType;
		return;
	}
	else
	{
		// 叩きつけ攻撃
		std::shared_ptr<Attack3> _attack3 = std::make_shared<Attack3>();
		m_NextState = _attack3;
		m_NextActionType = m_NextActionType = EnemyBase::Action::AttackType;
		m_flow = EnemyBase::Flow::EnterType;
		m_AttackType = AttackKind::QuakeType;
		return;
	}

}

// 消滅
void Golem::CrushingChange()
{
	m_NextState = std::make_shared<Crushing>();
	m_NextActionType = EnemyBase::Action::CrushingType;
	m_flow = Flow::EnterType;
}
//=================================================================================================

void Golem::BumpCheck()
{
	// スフィア判定
	KdCollider::SphereInfo sphereInfo;
	Math::Matrix _mat = m_model->FindWorkNode("spine")->m_worldTransform * (Math::Matrix::CreateTranslation(m_mWorld.Translation()));
	// 中心
	sphereInfo.m_sphere.Center = _mat.Translation();
	// 半径
	sphereInfo.m_sphere.Radius = m_HitSphereSize;
	// 対象
	sphereInfo.m_type = KdCollider::TypeBump;

	//デバッグ用
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, Math::Color{ 0,1,1,1 });

	// 当たり判定リスト
	std::list<KdCollider::CollisionResult>retSphereList;
	// 当たったオブジェクト格納リスト
	std::vector<std::shared_ptr<KdGameObject>>SphereList;
	for (auto& ret : SceneManager::Instance().GetObjList()) // キャラクター以外のオブジェクト
	{
		// 弾なら当たり判定をしない
		if (ret->GetName() == "Bullet")continue;

		if (ret->Intersects(sphereInfo, &retSphereList))SphereList.push_back(ret);
	}
	for (auto& ret : SceneManager::Instance().GetEnemyList()) // 敵
	{
		if (ret->IsExpired())continue;     // 消滅フラグが立っていたら当たり判定をしない
		if (m_id == ret->GetID())continue; // 自分自身なら当たり判定をしない
		if (ret->Intersects(sphereInfo, &retSphereList))SphereList.push_back(ret);
	}
	if (SceneManager::Instance().GetPlayer()->Intersects(sphereInfo, &retSphereList)) // プレイヤー
	{
		SphereList.push_back(SceneManager::Instance().GetPlayer());
	}

	// 当たった方向
	Math::Vector3 HitDir = Math::Vector3::Zero;
	// はみだし距離
	float maxOverLap = 0.0f;
	// 当たり判定フラグ
	bool HitFlg = false;
	// 当たったオブジェクト
	std::shared_ptr<KdGameObject> HitObj = nullptr;
	// リスト番号
	int listNum = 0;
	for (auto& sphere : retSphereList)
	{
		// はみ出し距離が長かったら更新
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
		// Y軸は考慮しない
		HitDir.y = 0.0f;
		// 正規化
		HitDir.Normalize();
		// キャラクター以外のオブジェクトなら自身が動く
		if (HitObj->GetObjType() == KdGameObject::ObjType::oObject)
		{
			m_pos += maxOverLap * HitDir;
		}
		// キャラクターならそのキャラクターを動かす
		else
		{
			Math::Vector3 _pos = HitObj->GetPos();
			HitDir *= -1.0f;
			_pos += maxOverLap * HitDir;
			HitObj->SetPos(_pos);
		}
	}
}

// セッター========================================================================================
// 弾攻撃の射程角度
void Golem::SetAttack1Angle(float _angle)
{
	m_Attack1Angle = _angle;
}

// ジャンプ攻撃の最短距離
void Golem::SetAttack2Dist(float _dist)
{
	m_Attack2Dist = _dist;
}
//=================================================================================================

// ゲッター========================================================================================
// 弾発射位置
const Math::Vector3& Golem::GetBulletPoint() const
{
	return (m_model->FindWorkNode("BulletPoint")->m_worldTransform * m_mWorld).Translation();
}
// 地震位置
const Math::Vector3& Golem::GetQuakePoint() const
{
	return (m_model->FindWorkNode("QuakePoint")->m_worldTransform * m_mWorld).Translation();
}
// 弾攻撃の射程角度
const float& Golem::GetAttack1Angle() const
{
	return m_Attack1Angle;
}
// ジャンプ攻撃の最短距離
const float& Golem::GetAttack2Dist() const
{
	return m_Attack2Dist;
}
//=================================================================================================

// Appeal==========================================================================================
void Golem::Appeal::Enter(std::shared_ptr<EnemyBase> owner)
{
	EnemyBase::Appeal::Enter(owner);
}

void Golem::Appeal::Update(std::shared_ptr<EnemyBase> owner)
{
	EnemyBase::Appeal::Update(owner);

	if (m_ActionFPS == 80)
	{
		// SE
		KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Enemy/Golem/ドラゴンが火を吐く.wav", 0.5f, false);
	}

	// FPS加算
	m_ActionFPS++;
}

void Golem::Appeal::Exit(std::shared_ptr<EnemyBase> owner)
{
	EnemyBase::Appeal::Exit(owner);
}
//=================================================================================================


// Attack1=========================================================================================
void Golem::Attack1::Enter(std::shared_ptr<EnemyBase> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("IdolToAttack1"))
	{
		owner->SetAnime("IdolToAttack1", false, 1.0f);
		return;
	}

	// アニメーションが終了したら次のフローへ
	if (owner->GetIsAnimator())
	{
		owner->SetFlow(EnemyBase::Flow::UpdateType);
		return;
	}
}

void Golem::Attack1::Update(std::shared_ptr<EnemyBase> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("Attack1"))
	{
		owner->SetAnime("Attack1", false, 1.0f);
		// 弾生成
		m_bullet = owner->GetObjManager().lock()->SetBulletParam();
		return;
	}

	// 弾が無くなったら次のフローへ
	if (m_bullet.expired())
	{
		owner->SetFlow(EnemyBase::Flow::ExitType);
		return;
	}

	// 弾発射
	if (m_ActionFPS == 10)
	{
		// エフェクト
		KdEffekseerManager::GetInstance().Play("Enemy/AttackSignal/BloodLance.efkefc", owner->GetAttackStartPointMat().Translation(), 2.0f, 2.0f, false);
		// SE
		KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Enemy/maou_se_battle05 (mp3cut.net).wav", 0.1f, false);
		// 方向
		Math::Matrix RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->GetAngle().y));
		Math::Vector3 _dir = Math::Vector3::TransformNormal(owner->GetForward(), RotY);
		m_bullet.lock()->SetDir(_dir);
	}

	// 弾が最大サイズではなかった場合
	if (!m_bullet.expired() && m_bullet.lock()->GetSize() < m_bullet.lock()->GetMaxSize())
	{
		// 発射位置に固定
		m_bullet.lock()->SetPos(owner->GetObjManager().lock()->GetGolem().lock()->GetBulletPoint());

		// プレイヤーの方向に向く
		Math::Vector3 _dir = owner->GetTarget().lock()->GetPos() - owner->GetPos();
		_dir.y = 0.0f;
		_dir.Normalize();
		owner->Rotate(_dir, 1.0f);
	}
	else if (!m_bullet.expired() && m_bullet.lock()->GetSize() >= m_bullet.lock()->GetMaxSize())
	{
		// FPS加算
		m_ActionFPS++;
	}
}

void Golem::Attack1::Exit(std::shared_ptr<EnemyBase> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("Attack1ToIdol"))
	{
		owner->SetAnime("Attack1ToIdol", false, 1.0f);
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
//=================================================================================================


// Attack2=========================================================================================
void Golem::Attack2::Enter(std::shared_ptr<EnemyBase> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("IdolToAttack2"))
	{
		owner->SetAnime("IdolToAttack2", false, 1.0f);
		return;
	}

	// アニメーションが終了したら次のフローへ
	if (owner->GetIsAnimator())
	{
		owner->SetFlow(EnemyBase::Flow::UpdateType);
		return;
	}
}

void Golem::Attack2::Update(std::shared_ptr<EnemyBase> owner)
{
	// 方向
	static Math::Vector3 _dir = Math::Vector3::Zero;
	// プレイヤーの距離
	static float _playerDist = 0.0f;

	if (!owner->IsAnimCheck("Attack2"))
	{
		// アニメーション変更
		owner->SetAnime("Attack2", false, 1.0f);
		// ジャンプ力追加
		owner->SetJumpPow(5.0f);

		// プレイヤーの距離と方向計算
		std::shared_ptr<Player> _player = owner->GetTarget().lock();
		Math::Vector3 _playerPos = _player->GetPos();
		m_playerPos = _playerPos; // プレイヤー座標を記録
		Math::Vector3 _pos = owner->GetPos();
		Math::Vector3 _moveDir = _playerPos - _pos;
		_playerDist = Math::Vector3{ _moveDir.x,0.0f,_moveDir.z }.Length();
		_moveDir.Normalize();
		_dir = _moveDir;

		// 攻撃範囲出現
		owner->SetColorLightFlg(true);
		// 当たり判定用スフィアサイズ拡大※プレイヤーが埋まるのを防止
		owner->SetHitSphereSize(10.0f);
		return;
	}

	// 地面についたら
	if (owner->GetGroundFlg())
	{
		// 攻撃判定
		AttackHit(owner);
		// 攻撃範囲削除
		owner->SetColorLightFlg(false);
		KdEffekseerManager::GetInstance().Play("Enemy/Golem/Attack2/smash.efkefc", owner->GetPos(), Math::Vector3{60.0f,15.0f,60.0f}, 1.0f, false);
		// SE
		KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Enemy/Golem/全力で踏み込む.wav", 0.1f, false);
		// 次のフローへ
		owner->SetFlow(EnemyBase::Flow::ExitType);
		return;
	}

	// プレイヤーの方向計算
	Math::Vector3 _pos = owner->GetPos();
	Math::Vector3 _moveDir = m_playerPos - _pos;
	float dist = Math::Vector3{ _moveDir.x,0.0f,_moveDir.z }.Length();
	m_AttackSphereRange = 60.0f * ((_playerDist - dist) / _playerDist); // 近くにつれて攻撃範囲を拡大
	_moveDir.Normalize(); // 正規化
	_dir = _moveDir;

	// 移動
	if (_pos != m_playerPos)owner->SetMove(_dir, 10.0f);

	// 攻撃範囲
	KdShaderManager::Instance().WriteCBColor(m_playerPos, m_AttackSphereRange,Math::Color{1.0f,0.0f,0.0f,1.0f});
}

void Golem::Attack2::Exit(std::shared_ptr<EnemyBase> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("Attack2ToIdol"))
	{
		owner->SetAnime("Attack2ToIdol", false, 1.0f);
		return;
	}

	// アニメーションが終了したら待機状態へ
	if (owner->GetIsAnimator())
	{
		// 当たり判定用スフィアサイズを戻す
		owner->SetHitSphereSize(2.0f);

		// Idol
		owner->IdolChange();
		return;
	}
}

void Golem::Attack2::AttackHit(std::shared_ptr<EnemyBase> owner)
{
	// スフィア判定
	KdCollider::SphereInfo sphereInfo;
	// 中心
	sphereInfo.m_sphere.Center = owner->GetPos();
	// 半径
	sphereInfo.m_sphere.Radius = 60.0f;
	// 対象
	sphereInfo.m_type = KdCollider::Type::TypeDamage;

	// デバッグ
	//Math::Color _color = { 1,0,0,1 };
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, _color);

	// 攻撃判定
	if (owner->GetTarget().lock()->Intersects(sphereInfo, nullptr))
	{
		// ダメージ
		owner->GetTarget().lock()->Damage(owner->GetParam().Atk, owner->shared_from_this());
	}
}
//=================================================================================================


// Attack3=========================================================================================
void Golem::Attack3::Enter(std::shared_ptr<EnemyBase> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("IdolToAttack3"))
	{
		owner->SetAnime("IdolToAttack3", false, 1.0f);
		
		return;
	}

	// アニメーションが終了したら次のフローへ
	if (owner->GetIsAnimator())
	{
		owner->SetFlow(owner->Flow::UpdateType);
		return;
	}

	
}

void Golem::Attack3::Update(std::shared_ptr<EnemyBase> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("Attack3"))
	{
		owner->SetAnime("Attack3", false, 1.0f);
		return;
	}

	// アニメーションが終了したら待機状態へ
	if (owner->GetIsAnimator())
	{
		// Idol
		owner->IdolChange();
		return;
	}

	if (161 >= m_ActionFPS)
	{
		// 攻撃範囲
		owner->SetColorLightFlg(true);
	}
	else if (162 == m_ActionFPS)
	{
		// エフェクト
		KdEffekseerManager::GetInstance().Play("Enemy/AttackSignal/BloodLance.efkefc", owner->GetAttackStartPointMat().Translation(), 2.0f, 2.0f, false);
		// SE
		KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Enemy/maou_se_battle05 (mp3cut.net).wav", 0.1f, false);
	}
	else if (180 == m_ActionFPS)
	{
		// 攻撃判定
		AttackHit(owner);
		// 攻撃範囲OFF
		owner->SetColorLightFlg(false);
		KdEffekseerManager::GetInstance().Play("Enemy/Golem/Attack3/smash.efkefc", owner->GetPos(), Math::Vector3{ 57.0f,15.0f,57.0f }, 1.0f, false);
		// SE
		KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Enemy/Golem/全力で踏み込む.wav", 0.1f, false);
	}
	else
	{
		// プレイヤーの方向に向く
		Math::Vector3 dir = owner->GetTarget().lock()->GetPos() - owner->GetPos();
		dir.y = 0.0f;
		// 正規化
		dir.Normalize();
		// 回転
		owner->Rotate(dir, 2.5f);
	}

	// 攻撃範囲
	m_AttackSphereRange = 50.0f * (m_ActionFPS / 180.0f);
	KdShaderManager::Instance().WriteCBColor(owner->GetPos(), m_AttackSphereRange,Math::Color{1.0f,0.0f,0.0f,1.0f});
	
	// FPS加算
	m_ActionFPS++;
}

void Golem::Attack3::Exit(std::shared_ptr<EnemyBase> owner)
{

}

void Golem::Attack3::AttackHit(std::shared_ptr<EnemyBase> owner)
{
	// スフィア判定
	KdCollider::SphereInfo sphereInfo;
	// 中心
	sphereInfo.m_sphere.Center = owner->GetObjManager().lock()->GetGolem().lock()->GetQuakePoint();
	// 半径
	sphereInfo.m_sphere.Radius = 50.0f;
	// 対象
	sphereInfo.m_type = KdCollider::Type::TypeDamage;

	// デバッグ
	//Math::Color _color = { 1,0,0,1 };
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, _color);

	// 攻撃判定
	if (owner->GetTarget().lock()->Intersects(sphereInfo, nullptr))
	{
		// ダメージ
		owner->GetTarget().lock()->Damage(owner->GetParam().Atk, owner->shared_from_this());
	}
}
//=================================================================================================


// Crushing========================================================================================
void Golem::Crushing::Enter(std::shared_ptr<EnemyBase> owner)
{
	KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Enemy/Golem/建物が少し崩れる2.wav", 0.5f, false);
	EnemyBase::Crushing::Enter(owner);
}

void Golem::Crushing::Update(std::shared_ptr<EnemyBase> owner)
{
	EnemyBase::Crushing::Update(owner);
}

void Golem::Crushing::Exit(std::shared_ptr<EnemyBase> owner)
{
	EnemyBase::Crushing::Exit(owner);
}
//=================================================================================================