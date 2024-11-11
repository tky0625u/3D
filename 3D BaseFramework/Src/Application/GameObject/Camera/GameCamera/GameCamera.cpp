#include "GameCamera.h"
#include"../../../Scene/SceneManager.h"
#include"../../ObjectManager.h"
#include"GameCamera_State.h"
#include"GameCamera_ConText.h"
#include"PlayerCamera/GameCamera_Player.h"
#include"../../Character/Player/Player.h"
#include"../../Character/Action/Player/Player_ConText.h"

void GameCamera::Update()
{
	// デバッグ
	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
	{
		ShowCursor(true);
		return;
	}
	else if(!SceneManager::Instance().m_stop)
	{
		ShowCursor(false);
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
	else if (!m_state.expired())
	{
		if (m_NextState)
		{
			m_conText->SetState(m_NextState);
			m_state = m_NextState;
			m_NextState.reset();
		}
		m_state.lock()->Update();
	}
}

void GameCamera::PostUpdate()
{
	// デバッグ
	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)return;
	if (!m_spCamera) { return; }

	if (!m_state.expired())m_state.lock()->PostUpdate();

	//KdCollider::RayInfo rayInfo;
	//rayInfo.m_pos = m_mWorld.Translation();
	//Math::Vector3 _dir = m_wpTarget.lock()->GetPos() - m_mWorld.Translation();
	//_dir.Normalize();
	//rayInfo.m_dir = _dir;
	//rayInfo.m_range = (m_wpTarget.lock()->GetPos() - m_mWorld.Translation()).Length();
	//rayInfo.m_type = KdCollider::TypeBump | KdCollider::TypeGround;


	////デバッグ用
	//Math::Color color = { 1,1,1,1 };
	//m_pDebugWire->AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range, color);

	if (SceneManager::Instance().GetObjectManager()->GetSlowFlg())
	{
		if (m_FocusBackRange != 10.0f)m_FocusBackRange = 10.0f;

		if (m_ViewAngList[m_CameraType] > 45)
		{
			m_ViewAngList[m_CameraType] -= 2;
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
			m_ViewAngList[m_CameraType] += 2;
		}
		else
		{
			m_ViewAngList[m_CameraType] = 60;
		}
	}

	m_spCamera->SetFocus(8, 5, m_FocusBackRange);

	m_spCamera->SetCameraMatrix(m_mWorld);
	m_spCamera->SetProjectionMatrix(m_ViewAngList[m_CameraType]);
	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);
}

void GameCamera::Init()
{
	CameraBase::Init();

	std::shared_ptr<PlayerCamera> _player = std::make_shared<PlayerCamera>();

	m_conText = std::make_shared<GameCamera_ConText>(_player);
	m_state = m_conText->GetState();
	m_state.lock()->SetTarget(shared_from_this());

	//m_pDebugWire = std::make_unique<KdDebugWireFrame>();

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
	m_DegAngList[m_CameraType].x = std::clamp(m_DegAngList[m_CameraType].x, -20.f, 45.f);
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
