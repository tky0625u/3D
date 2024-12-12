#include "GameCamera.h"
#include"../../../Scene/SceneManager.h"
#include"../../ObjectManager.h"
#include"../../StageManager.h"
#include"../../Character/Player/Player.h"
#include"../../Stage/MagicPolygon/MagicPolygon.h"
#include"../../Character/Enemy/EnemyBase.h"

void GameCamera::Update()
{
	// デバッグ
	static bool key = false;
	if (GetAsyncKeyState('C') & 0x8000)
	{
		if (!key)
		{
			if (showFlg)
			{
				showFlg = false;
			}
			else
			{
				showFlg = true;
			}
			key = true;
		}
	}
	else
	{
		key = false;
	}

	// デバッグ
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

		if (!(GetAsyncKeyState(VK_LSHIFT) & 0x8000))
		{
			//UpdateRotateByMouse();
			m_spCamera->SetProjectionMatrix(m_ViewAngList[m_CameraType]);
		}

	}


	ShowCursor(showFlg);
}

void GameCamera::PostUpdate()
{
	// デバッグ
	if (showFlg)return;
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
	if (m_ObjectManager.lock()->GetSlowFlg())
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

void GameCamera::SetDegAngList(Math::Vector3 _player, Math::Vector3 _fixed, Math::Vector3 _clear)
{
	m_DegAngList.push_back(_player);
	m_DegAngList.push_back(_fixed);
	m_DegAngList.push_back(_clear);
}

void GameCamera::SetPosList(Math::Vector3 _player, Math::Vector3 _fixed, Math::Vector3 _clear)
{
	m_PosList.push_back(_player);
	m_PosList.push_back(_fixed);
	m_PosList.push_back(_clear);
}

void GameCamera::SetViewAngList(float _player, float _fixed, float _clear)
{
	m_ViewAngList.push_back(_player);
	m_ViewAngList.push_back(_fixed);
	m_ViewAngList.push_back(_clear);
}

// Player==========================================================================================
void GameCamera::PlayerCamera::Enter(std::shared_ptr<GameCamera> owner)
{
	owner->m_CameraType = GameCamera::CameraType::PlayerType;
	owner->m_flow       = GameCamera::Flow::UpdateType;
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

	owner->UpdateRotateByMouse();


	Math::Matrix _rot   = owner->GetRotationMatrix();
	Math::Matrix _trans = Math::Matrix::CreateTranslation(owner->GetNowPos());
	
	if (m_shakeFlg)Shake(owner,_trans);
	owner->m_mWorld = _trans * _rot * _targetMat;
}

void GameCamera::PlayerCamera::Exit(std::shared_ptr<GameCamera> owner)
{
}

void GameCamera::PlayerCamera::ChangeState(std::shared_ptr<GameCamera> owner)
{

}

void GameCamera::PlayerCamera::Shake(std::shared_ptr<GameCamera> owner,Math::Matrix& _trans)
{
	Math::Vector3 _pos   = owner->GetNowPos();
	static float  _angle = 0.0f;

	_angle += 100.0f;
	if (_angle > 360.0f)_angle -= 360.0f;
	_pos.y += m_move * (cos(DirectX::XMConvertToRadians(_angle)));
	_trans  = Math::Matrix::CreateTranslation(_pos);
	m_shakeTime--;
	if (m_shakeTime <= 0.0f)
	{
		m_shakeTime = 10.0f;
		m_move = 0.1f;
		m_shakeFlg  = false;
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
	const std::shared_ptr<MagicPolygon>	_spTarget = owner->GetFixedTarget();
	if (_spTarget)
	{
		_targetMat = Math::Matrix::CreateTranslation(_spTarget->GetMatrix().Translation());
	}

	Math::Matrix _rot   = owner->GetRotationMatrix();
	Math::Matrix _trans = Math::Matrix::CreateTranslation(owner->GetNowPos());

	owner->m_mWorld = _trans * _rot * _targetMat;

	ChangeState(owner);
}

void GameCamera::FixedCamera::Exit(std::shared_ptr<GameCamera> owner)
{
}

void GameCamera::FixedCamera::ChangeState(std::shared_ptr<GameCamera> owner)
{
	if (owner->m_ObjectManager.lock()->GetTeleportFlg())
	{
		std::shared_ptr<PlayerCamera> _player = std::make_shared<PlayerCamera>();
		owner->m_NextState = _player;
		owner->m_flow = GameCamera::Flow::EnterType;
		return;
	}
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
	Math::Vector3 _angle = owner->GetNowDegAng();
	_angle.y += 0.05f;
	if (_angle.y >= 360.0f)_angle.y -= 360.0f;
	owner->SetDegAng(_angle);

	// ターゲットの行列(有効な場合利用する)
	Math::Matrix				  _targetMat = Math::Matrix::Identity;
	const std::shared_ptr<Player> _spTarget = owner->m_wpTarget.lock();
	if (_spTarget)
	{
		Math::Matrix _rotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_spTarget->GetAngle().y + 180.0f));
		Math::Matrix _trans = Math::Matrix::CreateTranslation(_spTarget->GetPos());
		_targetMat = _rotY * _trans;
	}

	Math::Matrix _rot = owner->GetRotationMatrix();
	Math::Matrix _trans = Math::Matrix::CreateTranslation(owner->GetNowPos());

	owner->m_mWorld = _trans * _rot * _targetMat;
}

void GameCamera::ClearCamera::Exit(std::shared_ptr<GameCamera> owner)
{
}

void GameCamera::ClearCamera::ChangeState(std::shared_ptr<GameCamera> owner)
{
}
//=================================================================================================