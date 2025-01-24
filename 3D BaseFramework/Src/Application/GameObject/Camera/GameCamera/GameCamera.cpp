#include "GameCamera.h"

// シーンマネジャ
#include"../../../Scene/SceneManager.h"
// オブジェクトマネジャ
#include"../../ObjectManager.h"
// プレイヤー
#include"../../Character/Player/Player.h"
// 敵基底
#include"../../Character/Enemy/EnemyBase.h"
// 魔法陣
#include"../../Stage/MagicPolygon/MagicPolygon.h"

void GameCamera::Update()
{
	// デバッグ====================================================================================
	static bool key = false;
	if (GetAsyncKeyState('C') & 0x8000)
	{
		if (!key)
		{
			if (cursorMoveFlg)
			{
				cursorMoveFlg = false;
			}
			else
			{
				cursorMoveFlg = true;
			}
			key = true;
		}
	}
	else
	{
		key = false;
	}

	if (SceneManager::Instance().m_stop)
	{
		Math::Vector3 dir = Math::Vector3::Zero;
		if (GetAsyncKeyState('W') & 0x8000)dir.z = 1.0f;
		if (GetAsyncKeyState('A') & 0x8000)dir.x = -1.0f;
		if (GetAsyncKeyState('S') & 0x8000)dir.z = -1.0f;
		if (GetAsyncKeyState('D') & 0x8000)dir.x = 1.0f;
		if (GetAsyncKeyState('E') & 0x8000)dir.y = 1.0f;
		if (GetAsyncKeyState('Q') & 0x8000)dir.y = -1.0f;
		dir.Normalize();
		Math::Matrix cameraRotYMat = GetRotationYMatrix();
		dir = Math::Vector3::TransformNormal(dir, cameraRotYMat);
		dir.Normalize();

		m_PosList[m_CameraType] += dir * 1.0f;

	}

	//=============================================================================================
}

void GameCamera::PostUpdate()
{
	if (!m_spCamera) { return; }

	// ステート切替
	if (m_NextState)
	{
		m_state = m_NextState;
		m_NextState.reset();
	}

	m_state->StateUpdate(shared_from_this()); // ステート更新

	// パリィ時
	// 視野角の拡縮
	if (SceneManager::Instance().GetSlowFlg())
	{
		if (m_FocusBackRange != 10.0f)m_FocusBackRange = 10.0f;

		if (m_ViewAngList[m_CameraType] > 45)
		{
			m_ViewAngList[m_CameraType] -= 1.5f;
		}
		else
		{
			m_ViewAngList[m_CameraType] = 45;
		}
	}
	else
	{
		if (m_FocusBackRange != 2000.0f)m_FocusBackRange = 2000.0f;

		if (m_ViewAngList[m_CameraType] < 60)
		{
			m_ViewAngList[m_CameraType] += 1.5f;
		}
		else
		{
			m_ViewAngList[m_CameraType] = 60;
		}
	}

	HitCheck(); //当たり判定

	m_spCamera->SetFocus(8, 5, m_FocusBackRange);

	m_spCamera->SetCameraMatrix(m_mWorld);
	m_spCamera->SetProjectionMatrix(m_ViewAngList[m_CameraType]);
	
	if (cursorMoveFlg)return;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);
}

void GameCamera::Init()
{
	CameraBase::Init();

	// ステート設定
	std::shared_ptr<PlayerCamera> _player = std::make_shared<PlayerCamera>();
	m_state = _player;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

}

void GameCamera::UpdateRotateByMouse()
{
	// デバッグ
	if (cursorMoveFlg)return;

	// マウスでカメラを回転させる処理
	POINT _nowPos;
	GetCursorPos(&_nowPos);

	POINT _mouseMove{};
	_mouseMove.x = _nowPos.x - m_FixMousePos.x;
	_mouseMove.y = _nowPos.y - m_FixMousePos.y;

	// 実際にカメラを回転させる処理(0.15はただの補正値)
	m_DegAngList[m_CameraType].x += _mouseMove.y * 0.15f;
	m_DegAngList[m_CameraType].y += _mouseMove.x * 0.15f;

	// 回転制御
	m_DegAngList[m_CameraType].x = std::clamp(m_DegAngList[m_CameraType].x, -40.f, 45.f);
}

void GameCamera::HitCheck()
{
	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = m_mWorld.Translation();
	Math::Vector3 _dir = m_wpTarget.lock()->GetCameraPointMat().Translation() - m_mWorld.Translation();
	_dir.Normalize();
	rayInfo.m_dir = _dir;
	rayInfo.m_range = (m_wpTarget.lock()->GetCameraPointMat().Translation() - m_mWorld.Translation()).Length();
	rayInfo.m_type = KdCollider::TypeBump | KdCollider::TypeGround;

	//デバッグ用
	//Math::Color color = { 1,1,1,1 };
	//m_pDebugWire->AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range, color);

	// 当たり判定
	std::list<KdCollider::CollisionResult> retRayList;
	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		ret->Intersects(rayInfo, &retRayList);
	}

	float _maxOverLap = -0.1f;
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

	if (_hitFlg)
	{
		Math::Vector3 _distPos = _hitPos;
		_distPos += _dir * 0.4f; //当たった位置より少し前に設定　※カメラを埋まらなくするため
		m_mWorld.Translation(_distPos);
	}
}

// ステート更新
void GameCamera::StateBase::StateUpdate(std::shared_ptr<GameCamera> owner)
{
	switch (owner->m_flow)
	{
	case Flow::EnterType:
		Enter(owner);
		break;
	case Flow::UpdateType:
		Update(owner);
		break;
	case Flow::ExitType:
		Exit(owner);
		break;
	default:
		break;
	}
}

// Player==========================================================================================
void GameCamera::PlayerCamera::Enter(std::shared_ptr<GameCamera> owner)
{
	owner->m_CameraType = GameCamera::CameraType::PlayerType;
	owner->m_flow       = GameCamera::Flow::UpdateType;

	ChangeState(owner);
}

void GameCamera::PlayerCamera::Update(std::shared_ptr<GameCamera> owner)
{
	// ターゲットの行列(有効な場合利用する)
	Math::Matrix								_targetMat = Math::Matrix::Identity;
	const std::shared_ptr<Player>	_spTarget = owner->m_wpTarget.lock();
	if (_spTarget)
	{
		_targetMat = Math::Matrix::CreateTranslation(_spTarget->GetPos());
	}

	owner->UpdateRotateByMouse(); // マウスでの視点移動

	// 行列
	Math::Matrix _rot   = owner->GetRotationMatrix();                          // 回転
	Math::Matrix _trans = Math::Matrix::CreateTranslation(owner->GetNowPos()); // 座標
	
	// 画面振動
	if (owner->m_shakeFlg)Shake(owner,_trans);
	
	// 行列合成
	owner->m_mWorld = _trans * _rot * _targetMat;

	ChangeState(owner);
}

void GameCamera::PlayerCamera::Exit(std::shared_ptr<GameCamera> owner)
{
}

void GameCamera::PlayerCamera::ChangeState(std::shared_ptr<GameCamera> owner)
{
	if (!owner->m_BossTarget.expired() && owner->m_BossTarget.lock()->GetActionType() == EnemyBase::Action::AppealType)owner->BossChange();
}

void GameCamera::PlayerCamera::Shake(std::shared_ptr<GameCamera> owner,Math::Matrix& _trans)
{
	Math::Vector3 _pos   = owner->GetNowPos(); // カメラ位置
	static float  _angle = 0.0f;

	_angle += owner->m_ChangeShakeAngle;
	if (_angle > 360.0f)_angle -= 360.0f;
	_pos.y += owner->m_move * (sin(DirectX::XMConvertToRadians(_angle))); // sinカーブでのY座標を上下に動かす
	_trans  = Math::Matrix::CreateTranslation(_pos);
	owner->m_shakeTime--;
	if (owner->m_shakeTime <= 0.0f) // 制限時間が経てば終了
	{
		owner->m_shakeTime = owner->m_DefaultShakeTime;
		owner->m_move      = owner->m_DefaultMove;
		owner->m_shakeFlg  = false;
	}
}
//=================================================================================================


// Fixed===========================================================================================
void GameCamera::FixedCamera::Enter(std::shared_ptr<GameCamera> owner)
{
	owner->m_CameraType = GameCamera::CameraType::FixedType;
	owner->m_flow = GameCamera::Flow::UpdateType;
}

void GameCamera::FixedCamera::Update(std::shared_ptr<GameCamera> owner)
{
	Math::Matrix								_targetMat = Math::Matrix::Identity;
	
	// 魔法陣
	const std::shared_ptr<MagicPolygon>	_spTarget = owner->GetFixedTarget().lock();
	if (_spTarget)
	{
		// 座標行列
		_targetMat = Math::Matrix::CreateTranslation(_spTarget->GetMatrix().Translation());
	}

	// 行列
	Math::Matrix _rot   = owner->GetRotationMatrix();                          // 回転
	Math::Matrix _trans = Math::Matrix::CreateTranslation(owner->GetNowPos()); // 座標
	// 合成
	owner->m_mWorld = _trans * _rot * _targetMat;

	ChangeState(owner);
}

void GameCamera::FixedCamera::Exit(std::shared_ptr<GameCamera> owner)
{
}

void GameCamera::FixedCamera::ChangeState(std::shared_ptr<GameCamera> owner)
{
	// テレポートの準備が整ったら
	if (owner->m_ObjectManager.lock()->GetTeleportFlg())
	{
		// プレイヤー
		std::shared_ptr<PlayerCamera> _player = std::make_shared<PlayerCamera>();
		owner->m_NextState = _player;
		owner->m_flow = GameCamera::Flow::EnterType;
		return;
	}
}
//=================================================================================================


// Boss============================================================================================
void GameCamera::BossCamera::Enter(std::shared_ptr<GameCamera> owner)
{
	owner->m_CameraType = GameCamera::CameraType::BossType;
	owner->m_flow = GameCamera::Flow::UpdateType;
}

void GameCamera::BossCamera::Update(std::shared_ptr<GameCamera> owner)
{
	// ボス
	Math::Matrix _bossMat = Math::Matrix::Identity;
	if (!owner->m_BossTarget.expired())
	{
		// ボス行列
		_bossMat = owner->m_BossTarget.lock()->GetMatrix();
	}

	// 回転
	Math::Matrix _rot = owner->GetRotationMatrix();
	// 座標
	Math::Matrix _trans = Math::Matrix::CreateTranslation(owner->GetNowPos());

	// 行列合成
	owner->m_mWorld = _trans * _rot * _bossMat;

	ChangeState(owner);
}

void GameCamera::BossCamera::Exit(std::shared_ptr<GameCamera> owner)
{
}

void GameCamera::BossCamera::ChangeState(std::shared_ptr<GameCamera> owner)
{
	if (owner->m_BossTarget.lock()->GetActionType() != EnemyBase::Action::AppealType)owner->PlayerChange();
}
//=================================================================================================


// Clear===========================================================================================
void GameCamera::ClearCamera::Enter(std::shared_ptr<GameCamera> owner)
{
	owner->m_CameraType = GameCamera::CameraType::ClearType;
	owner->m_flow = GameCamera::Flow::UpdateType;
}

void GameCamera::ClearCamera::Update(std::shared_ptr<GameCamera> owner)
{
	// カメラ回転
	Math::Vector3 _angle = owner->GetNowDegAng();
	_angle.y += owner->m_ChangeClearAngle;
	if (_angle.y >= 360.0f)_angle.y -= 360.0f;
	owner->SetDegAng(_angle);

	// プレイヤーの行列(有効な場合利用する)
	Math::Matrix				  _targetMat = Math::Matrix::Identity;
	const std::shared_ptr<Player> _spTarget = owner->m_wpTarget.lock();
	if (_spTarget)
	{            
		// 行列
		                                                                              // プレイヤーの正面
		Math::Matrix _rotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_spTarget->GetAngle().y + 180.0f)); // 回転
		Math::Matrix _trans = Math::Matrix::CreateTranslation(_spTarget->GetPos());                                        // プレイヤー座標
		_targetMat = _rotY * _trans; // プレイヤー行列
	}

	Math::Matrix _rot = owner->GetRotationMatrix();                            // 回転
	Math::Matrix _trans = Math::Matrix::CreateTranslation(owner->GetNowPos()); // 座標

	// 行列合成
	owner->m_mWorld = _trans * _rot * _targetMat;
}

void GameCamera::ClearCamera::Exit(std::shared_ptr<GameCamera> owner)
{
}

void GameCamera::ClearCamera::ChangeState(std::shared_ptr<GameCamera> owner)
{
}
//=================================================================================================

// セッター=================================================================================================================
// ワールド行列
void GameCamera::SetMatrix(Math::Matrix _mWorld)
{
	m_mWorld = _mWorld;
}

// 角度
void GameCamera::SetDegAng(Math::Vector3 _deg)
{
	m_DegAngList[m_CameraType] = _deg;
}

// 座標
void GameCamera::SetPos(Math::Vector3 _pos)
{
	m_PosList[m_CameraType] = _pos;
}

// 視野角
void GameCamera::SetViewAng(float _viewAng)
{
	m_ViewAngList[m_CameraType] = _viewAng;
}

// 角度リスト
void GameCamera::SetDegAngList(Math::Vector3 _player, Math::Vector3 _fixed, Math::Vector3 _boss, Math::Vector3 _clear)
{
	m_DegAngList.push_back(_player);
	m_DegAngList.push_back(_fixed);
	m_DegAngList.push_back(_boss);
	m_DegAngList.push_back(_clear);
}

// 座標リスト
void GameCamera::SetPosList(Math::Vector3 _player, Math::Vector3 _fixed, Math::Vector3 _boss, Math::Vector3 _clear)
{
	m_PosList.push_back(_player);
	m_PosList.push_back(_fixed);
	m_PosList.push_back(_boss);
	m_PosList.push_back(_clear);
}

// 視野角リスト
void GameCamera::SetViewAngList(float _player, float _fixed, float _boss, float _clear)
{
	m_ViewAngList.push_back(_player);
	m_ViewAngList.push_back(_fixed);
	m_ViewAngList.push_back(_boss);
	m_ViewAngList.push_back(_clear);
}

// テレポート解放時のターゲット
void GameCamera::SetFixedTarget(std::shared_ptr<MagicPolygon> _Obj)
{
	m_FixedTarget = _Obj;
}

// ボス
void GameCamera::SetBossTarget(std::shared_ptr<EnemyBase> _boss)
{
	m_BossTarget = _boss;
}

// カメラタイプ
void GameCamera::SetCameraType(UINT _cameraType)
{
	m_CameraType = _cameraType;
}

// 振動フラグ
void GameCamera::SetShakeFlg(bool _shakeFlg)
{
	m_shakeFlg = _shakeFlg;
}

// クリア時角度変化量
void GameCamera::SetChangeClearAngle(float _angle)
{
	m_ChangeClearAngle = _angle;
}

// 振動変化量
void GameCamera::SetChangeShakeAngle(float _angle)
{
	m_ChangeShakeAngle = _angle;
}

// 初期振動移動量
void GameCamera::SetDefaultShakeMove(float _move)
{
	m_DefaultMove = _move;
	m_move = m_DefaultMove;
}

// 振動移動量
void GameCamera::SetShakeMove(float _move)
{
	m_move = _move;
}

// 初期振動時間
void GameCamera::SetDefaultShakeTime(int _time)
{
	m_DefaultShakeTime = _time;
	m_shakeTime = m_DefaultShakeTime;
}
//==========================================================================================================================

// ゲッター=================================================================================================================
// 現在のモードの座標
const Math::Vector3& GameCamera::GetNowPos() const
{
	return m_PosList[m_CameraType];
}

// 現在のモードの角度
const Math::Vector3& GameCamera::GetNowDegAng() const
{
	return m_DegAngList[m_CameraType];
}

// 現在のモードの視野角
const float& GameCamera::GetNowViewAng() const
{
	return m_ViewAngList[m_CameraType];
}

// テレポート開放時のターゲット
const std::weak_ptr<MagicPolygon>& GameCamera::GetFixedTarget() const
{
	return m_FixedTarget;
}

// ボス
const std::weak_ptr<EnemyBase>& GameCamera::GetBossTarget() const
{
	return m_BossTarget;
}

// カメラターゲット
const std::weak_ptr<Player>& GameCamera::GetwpTarget() const
{
	return m_wpTarget;
}

// クリア時回転角度
const float& GameCamera::GetChangeClearAngle() const
{
	return m_ChangeClearAngle;
}

// 振動時変化量
const float& GameCamera::GetChangeShakeAngle() const
{
	return m_ChangeShakeAngle;
}

// 初期振動移動量
const float& GameCamera::GetDefaultMove() const
{
	return m_DefaultMove;
}

// 初期振動時間
const int& GameCamera::GetDefaultShakeTime() const
{
	return m_DefaultShakeTime;
}

// 振動フラグ
const bool GameCamera::GetShakeFlg() const
{
	return m_shakeFlg;
}

// カメラタイプ
const UINT& GameCamera::GetCameraType() const
{
	return m_CameraType;
}

// 回転行列
const Math::Matrix GameCamera::GetRotationMatrix() const
{
	return Math::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_DegAngList[m_CameraType].y),
		DirectX::XMConvertToRadians(m_DegAngList[m_CameraType].x),
		DirectX::XMConvertToRadians(m_DegAngList[m_CameraType].z));
}

// 回転行列X
const Math::Matrix GameCamera::GetRotationXMatrix() const
{
	return Math::Matrix::CreateRotationX(
		DirectX::XMConvertToRadians(m_DegAngList[m_CameraType].x));
}

// 回転行列Y
const Math::Matrix GameCamera::GetRotationYMatrix() const
{
	return Math::Matrix::CreateRotationY(
		DirectX::XMConvertToRadians(m_DegAngList[m_CameraType].y));
}

// 現在のステート
const std::shared_ptr<GameCamera::StateBase>& GameCamera::GetState() const
{
	return m_state;
}
//==========================================================================================================================

// ステート切り替え=========================================================================================================
// プレイヤー
void GameCamera::PlayerChange()
{
	std::shared_ptr<PlayerCamera> _player = std::make_shared<PlayerCamera>();
	m_NextState = _player;
	m_flow = GameCamera::Flow::EnterType;
	return;
}

// テレポート開放
void GameCamera::FixedChange()
{
	std::shared_ptr<FixedCamera> _fixed = std::make_shared<FixedCamera>();
	m_NextState = _fixed;
	m_flow = GameCamera::Flow::EnterType;
	return;
}

// ボス
void GameCamera::BossChange()
{
	std::shared_ptr<BossCamera> _boss = std::make_shared<BossCamera>();
	m_NextState = _boss;
	m_flow = GameCamera::Flow::EnterType;
	return;
}

// クリア
void GameCamera::ClearChange()
{
	std::shared_ptr<ClearCamera> _clear = std::make_shared<ClearCamera>();
	m_NextState = _clear;
	m_flow = GameCamera::Flow::EnterType;
	return;
}
//==========================================================================================================================