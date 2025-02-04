#pragma once
// キャラクター基底クラス
#include"../CharacterBase.h"

class Enemy_HP; // 敵HPUI
class Player;   // プレイヤー

class EnemyBase :public CharacterBase,public std::enable_shared_from_this<EnemyBase>
{
public:
	// 行動
	enum Action
	{
		IdolType,    // 待機
		AttackType,  // 攻撃
		RunType,     // 移動
		StumbleType, // よろけ
		HitType,     // 被弾
		AppealType,  // 出現演出
		CrushingType // 消滅
	};

	EnemyBase()                       {}
	~EnemyBase()             override {};

	void Update()            override;
	void PostUpdate()        override;
	virtual void Action()    override;
	virtual void Init()      override;

	void CrushingAction()    override; // やられ演出
	void GroundCheck()       override; // 地面判定
	virtual void BumpCheck() override; // 横当たり判定

	// 行動切り替え============================================================
	void IdolChange();               // 待機
	void RunChange();                // 移動
	virtual void HitChange();        // 被弾
	virtual void CrushingChange();   // 消滅
	virtual void AttackChange(){};   // 攻撃
	virtual void StumbleChange() {}; // よろけ
	//=========================================================================

	void Damage(unsigned int _damage); // ダメージ計算

	// セッター================================================================
	void SetTarget(std::shared_ptr<Player> _target); // プレイヤー
	void SetEnemyCheckRadius(float _radius);         // 他の敵との距離判定時のスフィアの半径
	void SetLeaveDist(float _dist);                  // 避ける距離
	void SetAppealEffectSize(float _size);           // 出現演出時のエフェクトサイズ
	void SetAttackSphereSize(float _size);           // 攻撃判定のスフィアサイズ
	void SetStumbleMove(float _move);                // よろける時の移動スピード
	void SetFlow(UINT _flow);                        // 現在のステートの実行関数タイプ
	void SetColorLightFlg(bool _flg);                // 攻撃範囲フラグ
	void SetBossFlg(bool _bossFlg);                  // ボスフラグ
	//=========================================================================

	// ゲッター================================================================
	// プレイヤー
	const std::weak_ptr<Player>& GetTarget()const;
	// プレイヤーに向かうための回転行列
	const Math::Matrix           GettoTargetRotateYMatrix(std::weak_ptr<Player> _target);
	// HPノード　この位置にHPが置かれる
	const Math::Matrix           GetHPMat()const;
	// 攻撃前エフェクトノード
	const Math::Matrix           GetAttackStartPointMat()const;
	// 他の敵との距離判定時のスフィアの半径
	const float&                 GetEnemyCheckRadius()const;
	// 避ける距離
	const float&                 GetLeaveDist()const;
	// 出現演出時のエフェクトサイズ
	const float&                 GetAppealEffectSize()const;
	// 攻撃判定のスフィアサイズ
	const float&                 GetAttackSphereSize()const;
	// よろける時の移動スピード
	const float&                 GetStumbleMove()const;
	// 行動
	const UINT&                  GetActionType()const;
	// 地面フラグ
	const bool                   GetGroundFlg()const;
	// 避けて移動する対象
	const bool                   GetLeave()const;
	// ボスフラグ
	const bool                   GetBossFlg()const;
	//=========================================================================

protected:
	// プレイヤー
	std::weak_ptr<Player>              m_Target;
	// エフェクト
	std::weak_ptr<KdEffekseerObject>   m_Effect;
	// 避けて移動する対象
	std::weak_ptr<EnemyBase>           m_leaveEnemy;
	// 他の敵との距離判定時のスフィアの半径
	float                              m_EnemyCheckRadius = 0.0f;
	// 避ける距離
	float                              m_LeaveDist        = 0.0f;
	// 出現演出時のエフェクトサイズ
	float                              m_AppealEffectSize = 1.0f;
	// 攻撃判定のスフィアサイズ
	float                              m_AttackSphereSize = 0.0f;
	// よろける時の移動スピード
	float                              m_StumbleMove      = 0.0f;
	// 攻撃範囲フラグ
	bool                               m_ColorLightFlg    = false;
	// 地面フラグ
	bool                               m_groundFlg        = true;
	// ボスフラグ
	bool                               m_bossFlg          = false;

protected:

	///////////////////////
	/// ステートパターン///
	///////////////////////

	// 基底クラス
	class StateBase
	{
	public:
		StateBase() {};
		virtual ~StateBase() {};

		// ステート更新
		void StateUpdate(std::shared_ptr<EnemyBase> owner);

		virtual void Enter (std::shared_ptr<EnemyBase> owner) {};
		virtual void Update(std::shared_ptr<EnemyBase> owner) {};
		virtual void Exit  (std::shared_ptr<EnemyBase> owner) {};

		// ダメージ計算
		virtual void Damage(std::shared_ptr<EnemyBase> owner, unsigned int _damage);

	protected:
		// アニメーションFPS
		float m_ActionFPS = 0.0f;

	};

	// 出現演出
	class Appeal :public StateBase
	{
	public:
		Appeal() {};
		virtual ~Appeal()override {};

		virtual void Enter (std::shared_ptr<EnemyBase> owner)override;
		virtual void Update(std::shared_ptr<EnemyBase> owner)override;
		virtual void Exit  (std::shared_ptr<EnemyBase> owner)override;

	protected:
		// エフェクト用ハンドル
		Effekseer::Handle m_handle = 0;
	};

	// 待機
	class Idol :public StateBase
	{
	public:
		Idol() {};
		~Idol()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

	protected:

	};

	// 移動
	class Run :public StateBase
	{
	public:
		Run() {};
		~Run()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

		// 移動
		void Move(std::shared_ptr<EnemyBase> owner);
		// プレイヤーとの距離判定
		bool PlayerDistCheck(std::shared_ptr<EnemyBase> owner, std::weak_ptr<EnemyBase> leave);
		// 他の敵との距離判定
		void EnemyCheck(std::shared_ptr<EnemyBase> owner);
		// 避けて移動
		void Leave(std::shared_ptr<EnemyBase> owner);

	protected:
		// 移動方向
		Math::Vector3 m_moveDir  = Math::Vector3::Zero;
		// 避ける方向
		Math::Vector3 m_leaveDir = Math::Vector3::Zero;
	};

	// 被弾
	class Hit :public StateBase
	{
	public:
		Hit() {};
		~Hit()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

	protected:

	};

	// 消滅
	class Crushing :public StateBase
	{
	public:
		Crushing() {};
		virtual ~Crushing()override {};

		virtual void Enter (std::shared_ptr<EnemyBase> owner)override;
		virtual void Update(std::shared_ptr<EnemyBase> owner)override;
		virtual void Exit  (std::shared_ptr<EnemyBase> owner)override;

	protected:

	};

	// 現在のステート
	std::shared_ptr<StateBase> m_state            = nullptr;
	// 次のステート
	std::shared_ptr<StateBase> m_NextState        = nullptr;
	// 現在の行動
	UINT                       m_actionType       = Action::IdolType;
	// 次の行動
	UINT                       m_NextActionType   = m_actionType;

public:
	// セッター=====================================================================
	// 次の行動
	void SetNextAction(std::shared_ptr<StateBase> _action, UINT _actionType);
	//==============================================================================

	// ゲッター=====================================================================
	const UINT& GetNextActionType()const; // 次の行動タイプ
	//==============================================================================
};