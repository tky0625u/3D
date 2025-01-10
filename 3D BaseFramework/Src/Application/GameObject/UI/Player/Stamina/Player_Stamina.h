#pragma once

// UI基底
#include"../../UIBase.h"

// プレイヤー
class Player;

class Player_Stamina :public UIBase
{
public:
	Player_Stamina()           {};
	~Player_Stamina() override {};

	void Update()     override;
	void DrawSprite() override;
	void Init()       override;

	// セッター==========================================================================
	void SetTraget(std::shared_ptr<Player> _target) { m_target = _target; } // ターゲット（プレイヤー）
	//===================================================================================

private:

	// スタミナ列挙型
	enum Stamina
	{
		box,     // スタミナボックス
		stamina, // スタミナゲージ
		num      // 数
	};

	std::weak_ptr<Player>      m_target;               // ターゲット（プレイヤー）
	std::shared_ptr<KdTexture> m_pTex[Stamina::num];   // テクスチャ
	Math::Rectangle            m_rect[Stamina::num];   // 切り取り範囲
	int                        m_MaxStamina    = 0;    // 最大スタミナ
	int                        m_beforeStamina = 0;    // 前回のスタミナ
	float                      m_MaxWidth      = 0.0f; // 最大幅
};