#include "Player.h"

void Player::Update()
{
	//移動
	Math::Vector3 dir = Math::Vector3::Zero; //ベクトルリセット
	if (GetAsyncKeyState('W') & 0x8000)dir.z =  1.0f;
	if (GetAsyncKeyState('S') & 0x8000)dir.z = -1.0f;
	if (GetAsyncKeyState('A') & 0x8000)dir.x = -1.0f;
	if (GetAsyncKeyState('D') & 0x8000)dir.x =  1.0f;
	dir.Normalize();       //正規化
	m_pos += m_move * dir; //座標更新

	//ワールド行列更新
	CharacterBase::Update();
}

void Player::Init()
{
	CharacterBase::Init();
	m_move = 0.1f;
}
