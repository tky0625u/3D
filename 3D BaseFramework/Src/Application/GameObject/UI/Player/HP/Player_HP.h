#pragma once

// UI基底
#include"../../UIBase.h"

// プレイヤー
class Player;

class Player_HP :public UIBase
{
public:
	Player_HP()                {};
	~Player_HP()      override {};

	void Update()     override;
	void DrawSprite() override;
	void Init()       override;

	// セッター===============================================================================
	void SetTraget    (std::shared_ptr<Player> _target) { m_target = _target; }         // ターゲット（プレイヤー）
	void SetDownTime  (int _downTime)                   { m_DownTime = _downTime; }     // 減少ゲージ減少開始時間
	void SetDownChange(float _downChange)               { m_DownChange = _downChange; } // 減少ゲージ変化量
	//========================================================================================

	// ゲッター===============================================================================
	const int&   GetDownTime()const   { return m_DownTime; }   // 減少ゲージ減少開始時間
	const float& GetDownChange()const { return m_DownChange; } // 減少ゲージ変化量
	//========================================================================================

private:

	// HP列挙型
	enum HP
	{
		box,  // HPボックス
		down, // 減少ゲージ
		hp,   // HPゲージ
		num   // 数
	};

	std::weak_ptr<Player>      m_target;             // ターゲット（プレイヤー）
	std::shared_ptr<KdTexture> m_pTex[HP::num];      // テクスチャ
	Math::Rectangle            m_rect[HP::num];      // 切り取り範囲
	int                        m_MaxHP       = 0;    // 最大HP
	int                        m_beforeHP    = 0;    // 前回のHP
	int                        m_DownTime    = 0;    // 減少ゲージ減少開始時間
	int                        m_NowDownTime = 0;    // 現在の減少ゲージ減少開始時間
	float                      m_DownChange  = 0.0f; // 減少ゲージ変化量
	float                      m_MaxWidth    = 0.0f; // 最大幅
};