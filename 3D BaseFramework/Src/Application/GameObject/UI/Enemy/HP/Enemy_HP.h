#pragma once

// UI基底
#include"../../UIBase.h"

// ゲームカメラ
class GameCamera;
// 敵基底
class EnemyBase;

class Enemy_HP :public UIBase,public std::enable_shared_from_this<Enemy_HP>
{
public:
	Enemy_HP() {}
	~Enemy_HP()       override {};

	void Update()     override;
	void DrawSprite() override;
	void Init()       override;

	// セッター==========================================================================
	void SetTarget        (std::shared_ptr<EnemyBase> _target)  { m_target = _target; }             // ターゲット
	void SetCamera        (std::shared_ptr<GameCamera> _camera) { m_camera = _camera; }             // カメラ
	void SetDownTime      (int _downTime)                       { m_DownTime = _downTime; }         // 減少ゲージ減少開始時間
	void SetDownChange    (float _downChange)                   { m_DownChange = _downChange; }     // 減少ゲージ変化量
	void SetPosXCorrection(float _correction)                   { m_posXCorrection = _correction; } // X軸補正
	//===================================================================================

	// ゲッター==========================================================================
	const int&   GetDownTime()const       { return m_DownTime; }       // 減少ゲージ減少開始時間
	const float& GetDownChange()const     { return m_DownChange; }     // 減少ゲージ変化量
	const float& GetPosXCorrection()const { return m_posXCorrection; } // X軸補正
	//===================================================================================

private:

	// HP列挙型
	enum HP
	{
		box,  // HPボックス
		down, // 減少ゲージ
		hp,   // HPゲージ
		num   // 数
	};

	std::weak_ptr<GameCamera>  m_camera;                // ゲームカメラ
	std::weak_ptr<EnemyBase>   m_target;                // ターゲット
	std::shared_ptr<KdTexture> m_pTex[HP::num];         // テクスチャ
	Math::Rectangle            m_rect[HP::num];         // 切り取り範囲
	int                        m_MaxHP       = 0;       // 最大HP
	int                        m_beforeHP    = 0;       // 前回のHP
	int                        m_DownTime    = 0;       // 減少ゲージ減少開始時間
	int                        m_NowDownTime = 0;       // 現在の減少ゲージ減少開始時間
	float                      m_DownChange  = 0.0f;    // 減少ゲージ変化量
	float                      m_MaxWidth    = 0.0f;    // 最大幅
	float                      m_alpha       = 1.0f;    // アルファ値
	float                      m_posXCorrection = 0.0f; // X軸補正

private:

	//////////////////////
	///ステートパターン///
	//////////////////////

	// ステート基底
	class StateBase
	{
	public:
		StateBase() {};
		virtual ~StateBase() {};

		virtual void Update(std::shared_ptr<Enemy_HP> owner) {};
	};

	// 通常
	class Normal :public StateBase
	{
	public:
		Normal() {};
		~Normal()override {};

		void Update(std::shared_ptr<Enemy_HP> owner)override;

	private:
	};

	// ボス
	class Boss :public StateBase
	{
	public:
		Boss() {};
		~Boss()override {};

		void Update(std::shared_ptr<Enemy_HP> owner)override;

	private:
	};

	std::shared_ptr<StateBase> m_state = nullptr;
};