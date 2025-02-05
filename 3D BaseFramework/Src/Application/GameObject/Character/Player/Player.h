#pragma once

// キャラクター基底クラス
#include"../CharacterBase.h"

class ObjectManager; // オブジェクトマネジャ
class EnemyBase;     // 敵基底クラス
class BulletBase;    // 弾基底クラス
class Shield;        // 盾


class Player :public CharacterBase, public std::enable_shared_from_this<Player>
{
public:
	// 行動
	enum Action
	{
		IdolType,          // 待機
		AttackType,        // 攻撃
		RunType,           // 移動
		RollType,          // 回避
		GuardType,         // 防御
		GuardReactionType, // 防御反応
		ParryType,         // パリィ
		CounterType,       // カウンター
		HitType,           // 被弾
		CrushingType,      // 消滅
		TeleportType       // テレポート
	};

	Player()                  {}
	~Player()        override {};

	void Action()    override;
	void PostUpdate()override;
	void Init()      override;

	// テレポート位置判定
	void NextStageCheck();
	// スタミナ回復
	void StaminaRecovery();

	// 行動切り替え================================================================================
	// 待機
	void IdolChange();
	// テレポート
	void TeleportChange();
	//=============================================================================================

	// ダメージ===========================================================================================
	void Damage(int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr);   // 直接攻撃による被弾
	void Damage(int _damage = 0, std::shared_ptr<BulletBase> _bullet = nullptr); // 遠距離攻撃による被弾
	//====================================================================================================

	// セッター====================================================================================
	void SetShield             (std::shared_ptr<Shield> _shield);      // 盾
	void SetCounterRadius      (float _radius);                        // カウンターの攻撃範囲
	void SetParryID            (UINT _id);                             // パリィした敵のID
	void SetParryTime          (int _time);                            // パリィ可能時間
	void SetStaminaRecoveryTime(int _time);                            // スタミナ回復開始時間 
	void SetRollStamina        (int _rollStamina);                     // 回避した時のスタミナの減少量
	//=============================================================================================

	// ゲッター====================================================================================
	const std::weak_ptr<Shield>&        GetShield() const;              // 盾
	const std::weak_ptr<ObjectManager>& GetObjectManager() const;       // オブジェクトマネジャ
	const Math::Matrix&                 GetEnemyAttackPointMat() const; // 敵に対する攻撃の高さノード
	const Math::Matrix&                 GetCameraPointMat() const;      // カメラ判定用ノード
	const float&                        GetCounterRadius() const;       // カウンターの攻撃範囲
	const UINT&                         GetParryID() const;             // パリィした敵のID
	const UINT&                         GetActionType() const;          // 行動タイプ
	const int&                          GetParryTime() const;           // パリィ可能時間
	const unsigned int&                 GetMaxStamina() const;          // 最大スタミナ
	const unsigned int&                 GetStaminaRecoveryTime() const; // スタミナ回復開始時間
	const unsigned int&                 GetRollStamina() const;         // 回避した時のスタミナ減少量
	//=============================================================================================

private:
	// 盾
	std::weak_ptr<Shield>        m_shield;
	
	// カウンターの攻撃範囲
	float                        m_CounterRadius          = 0.0f;
													      
	// パリィした敵のID								 	  
	UINT                         m_ParryID                = 0;
	
	// パリィ可能時間
	int                          m_ParryTime              = 0;
													      
	// スタミナ最大値									   
	unsigned int                 m_MaxStamina             = 0;
													      
	// スタミナ回復可能時間							  	  
	unsigned int                 m_StaminaRecoveryTime    = 0;

	// 現在のスタミナ回復可能時間
	int                          m_NowStaminaRecoveryTime = 0;

	// 回避した時のスタミナの減少量
	unsigned int                 m_RollStamina            = 0;

	// ガードした時のスタミナ減少量補正
	int                          m_GuardStaminaCorrection = 10;

private:

	////////////////////////
	/// ステートパターン ///
	////////////////////////

	// ステート基底
	class StateBase
	{
	public:
		StateBase() {};
		virtual ~StateBase() {};

		// ステート更新
		void StateUpdate(std::shared_ptr<Player> owner);

		virtual void Enter (std::shared_ptr<Player> owner) {};
		virtual void Update(std::shared_ptr<Player> owner) {};
		virtual void Exit  (std::shared_ptr<Player> owner) {};

		// ステート切り替え
		virtual void ChangeState(std::shared_ptr<Player> owner) = 0;

		virtual void Damage(std::shared_ptr<Player> owner, unsigned int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr);   // 直接攻撃による被弾
		virtual void Damage(std::shared_ptr<Player> owner, unsigned int _damage = 0, std::shared_ptr<BulletBase> _bullet = nullptr); // 遠距離攻撃による被弾

		// 攻撃判定
		virtual void AttackHit(std::shared_ptr<Player> owner);

	protected:
		// アニメーション経過フレーム
		int m_ActionFPS = 0;
	};

	// 待機
	class Idol :public StateBase
	{
	public:
		Idol() {};
		~Idol()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void ChangeState(std::shared_ptr<Player> owner)override;

	private:

	};

	// 移動
	class Run :public StateBase
	{
	public:
		Run() {};
		~Run()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void Move (std::shared_ptr<Player> owner);
		void ChangeState(std::shared_ptr<Player> owner)override;

	private:

	};

	// 攻撃
	class Attack :public StateBase
	{
	public:
		Attack() {};
		~Attack()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit(std::shared_ptr<Player> owner)override;

		// 攻撃
		void Attack1(std::shared_ptr<Player> owner);
		void Attack2(std::shared_ptr<Player> owner);
		void Attack3(std::shared_ptr<Player> owner);

		// 攻撃方向 
		void AttackDirCheck(std::shared_ptr<Player> owner);
		
		void ChangeState(std::shared_ptr<Player> owner)override;

		void Damage(std::shared_ptr<Player> owner, unsigned int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)  override; // 直接攻撃による被弾
		void Damage(std::shared_ptr<Player> owner, unsigned int _damage = 0, std::shared_ptr<BulletBase> _bullet = nullptr)override; // 遠距離攻撃による被弾

	private:
		// 攻撃方向
		Math::Vector3     m_AttackDir  = Math::Vector3::Zero;
		
		// 攻撃段階
		static const int  AttackNUM    = 3;
		// 現在の攻撃段階
		int               m_atkNum     = 1;
		// 攻撃段階切り替えフレーム
		int               m_ChangeTime = 0;
		
		// エフェクト用ハンドル
		Effekseer::Handle m_handle = 0;
	};

	// カウンター
	class Counter :public StateBase
	{
	public:
		Counter() {};
		~Counter()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void ChangeState(std::shared_ptr<Player> owner)override;

		void AttackHit(std::shared_ptr<Player> owner);
		// 対象の敵の近くに移動
		void CounterMove(std::shared_ptr<Player> owner);

	private:
		// 対象の敵
		std::weak_ptr<EnemyBase> m_CounterEnemy;
		
		// エフェクト用ハンドル
		Effekseer::Handle        m_handle = 0;
	};

	// 回避
	class Roll :public StateBase
	{
	public:
		Roll() {};
		~Roll()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void RollMove (std::shared_ptr<Player> owner); // 回避時移動
		void ChangeState(std::shared_ptr<Player> owner)override;

	private:

	};

	// 防御
	class Guard :public StateBase
	{
	public:
		Guard() {};
		~Guard()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void ChangeState(std::shared_ptr<Player> owner)override;

		void GuardRotate(std::shared_ptr<Player> owner, Math::Vector3 _dir); // 防御した敵の方向を向く
		void Damage(std::shared_ptr<Player> owner, unsigned int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)  override; // 直接攻撃による被弾
		void Damage(std::shared_ptr<Player> owner, unsigned int _damage = 0, std::shared_ptr<BulletBase> _bullet = nullptr)override; // 遠距離攻撃による被弾

		void SetGuardTime(int _time); // ガード時間

	private:
		// 防御してからの経過時間
		int m_guardTime = 0;
	};

	// 防御反応
	class GuardReaction :public StateBase
	{
	public:
		GuardReaction() {};
		~GuardReaction()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void ChangeState(std::shared_ptr<Player> owner)override;

	private:
		// エフェクト用ハンドル
		Effekseer::Handle m_handle = 0;
	};

	// パリィ
	class Parry :public StateBase
	{
	public:
		Parry() {};
		~Parry()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void ChangeState(std::shared_ptr<Player> owner)override;

	private:

	};

	// 被弾
	class Hit :public StateBase
	{
	public:
		Hit() {};
		~Hit()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void ChangeState(std::shared_ptr<Player> owner)override;

	private:

	};

	// 消滅
	class Crushing :public StateBase
	{
	public:
		Crushing() {};
		~Crushing()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void ChangeState(std::shared_ptr<Player> owner)override;

	private:

	};

	// テレポート
	class Teleport :public StateBase
	{
	public:
		Teleport() {};
		~Teleport()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void ChangeState(std::shared_ptr<Player> owner)override;

	private:
		// エフェクト用ハンドル
		Effekseer::Handle m_handle = 0;
	};

	// 行動キー ※長押し防止用
	enum KeyType
	{
		MoveKey   = 1 << 0, // 移動
		AttackKey = 1 << 1, // 攻撃
		GuardKey  = 1 << 2, // 防御
		RollKey   = 1 << 3, // 回避
	};

	// ステート
	std::shared_ptr<StateBase> m_state          = nullptr;
	// 次のステート
	std::shared_ptr<StateBase> m_NextState      = nullptr;
	// 行動タイプ
	UINT                       m_actionType     = Action::IdolType;
	// 次の行動タイプ
	UINT                       m_NextActionType = m_actionType;
	// 行動キー
	UINT                       m_keyType        = 0;
	// 前フレームの行動キー
	UINT                       m_BeforeKeyType  = 0;

	// 次の行動
	void SetNextState(std::shared_ptr<StateBase> _next, UINT _action);

	// キーチェック
	void KeyCheck();
};