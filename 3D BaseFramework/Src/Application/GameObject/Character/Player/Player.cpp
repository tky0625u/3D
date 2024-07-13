#include "Player.h"

void Player::Action()
{
	//移動
	Math::Vector3 dir = Math::Vector3::Zero; //ベクトルリセット
	if (GetAsyncKeyState('W') & 0x8000)dir.z =  1.0f;
	if (GetAsyncKeyState('S') & 0x8000)dir.z = -1.0f;
	if (GetAsyncKeyState('A') & 0x8000)dir.x = -1.0f;
	if (GetAsyncKeyState('D') & 0x8000)dir.x =  1.0f;
	dir = dir.TransformNormal(dir, GetRotationYMatrix()); //向いてる方向によってのベクトル変換
	dir.Normalize();       //正規化
	float Move = m_status.SP * m_SpeedCorrection;
	
	//ダッシュ
	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
	{
		Move *= m_DashCorrection;
	}

	m_pos += Move * dir; //座標更新

	//方向転換
	UpdateRotateByMouse();
	m_rot = GetRotationMatrix();
}

void Player::Init()
{
	CharacterBase::Init();
	m_model->Load("Asset/Models/Character/Player/Player.gltf");

	StatusLoad("CSV/Character/Status/Player/Player.csv");

	m_sizeHalf = 1.0f;
	m_DashCorrection = 2.0f;

	// ↓画面中央座標
	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);
	ShowCursor(false);
}

void Player::CrushingAction()
{
	CharacterBase::CrushingAction();
}

void Player::UpdateRotateByMouse()
{
	// マウスでカメラを回転させる処理
	POINT _nowPos;
	GetCursorPos(&_nowPos);

	POINT _mouseMove{};
	_mouseMove.x = _nowPos.x - m_FixMousePos.x;
	_mouseMove.y = _nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// 実際にカメラを回転させる処理(0.15はただの補正値)
	m_DegAng.x += _mouseMove.y * 0.15f;
	m_DegAng.y += _mouseMove.x * 0.15f;

	// 回転制御
	m_DegAng.x = std::clamp(m_DegAng.x, -45.f, 45.f);
}
