#include "Player_HP.h"

// プレイヤー
#include"../../../Character/Player/Player.h"

void Player_HP::Update()
{
	// ターゲットが消滅していたら早期リターン
	if (m_target.expired())return;

	// HP減少
	if (m_target.lock()->GetParam().Hp != m_beforeHP) // 前回のHPと現在のHPが違う場合
	{//               最大幅      最大HP               現在のHP
		float hp = (m_MaxWidth / m_MaxHP) * m_target.lock()->GetParam().Hp; // 幅計算
		m_rect[HP::hp] = { 0,0,long(hp),64 }; // 切り取り範囲
		m_beforeHP = m_target.lock()->GetParam().Hp; // 前回のHPを更新
		m_NowDownTime = m_DownTime;
	}

	if (m_NowDownTime > 0)m_NowDownTime--;
	else
	{
		m_NowDownTime = 0;
		if (m_rect[HP::down].width != m_rect[HP::hp].width)m_rect[HP::down].width-=m_DownChange; //現在のHPまで徐々に減少
	}
}

void Player_HP::DrawSprite()
{
	for (int h = 0; h < HP::num; ++h)
	{
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex[h], (int)m_pos.x, (int)m_pos.y, (int)m_rect[h].width * m_size, (int)m_rect[h].height * m_size, &m_rect[h], &m_color, m_pivot);
	}
}

void Player_HP::Init()
{
	m_pivot = { 0.0f,0.5f };
	m_MaxWidth = 1024;
	m_color = { 1,1,1,1 };
	if (m_target.expired())return;
	m_MaxHP = m_target.lock()->GetParam().Hp;
	m_beforeHP = m_MaxHP;

	//HPバー
	m_rect[HP::hp] = { 0,0,long(m_MaxWidth),64 };
	m_pTex[HP::hp] = std::make_shared<KdTexture>();
	m_pTex[HP::hp]->Load("Asset/Textures/UI/Player/HP/Player_Hp_line.png");

	//HPボックス
	m_rect[HP::box] = { 0,0,long(m_MaxWidth),64 };
	m_pTex[HP::box] = std::make_shared<KdTexture>();
	m_pTex[HP::box]->Load("Asset/Textures/UI/Player/HP/frame.png");

	//HP減少ゲージ
	m_rect[HP::down] = { 0,0,long(m_MaxWidth),64 };
	m_pTex[HP::down] = std::make_shared<KdTexture>();
	m_pTex[HP::down]->Load("Asset/Textures/UI/Player/HP/Player_Hp_Downline.png");
}

// セッター========================================================================================
// ターゲット
void Player_HP::SetTraget(std::shared_ptr<Player> _target)
{
	m_target = _target;
}

// 減少ゲージ減少開始時間
void Player_HP::SetDownTime(int _downTime)
{
	m_DownTime = _downTime;
}

// 減少ゲージ変化量
void Player_HP::SetDownChange(float _downChange)
{
	m_DownChange = _downChange;
}
//=================================================================================================

// ゲッター========================================================================================
// 減少ゲージ減少開始時間
const int& Player_HP::GetDownTime() const
{
	return m_DownTime;
}

// 減少ゲージ変化量
const float& Player_HP::GetDownChange() const
{
	return m_DownChange;
}
//=================================================================================================