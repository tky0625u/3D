#pragma once
#include"../CameraBase.h"

// ステージマネージャ
class StageManager;
// 敵基底
class EnemyBase;
// 魔法陣
class MagicPolygon;

class GameCamera :public CameraBase,public std::enable_shared_from_this<GameCamera>
{
public:
	GameCamera() {};
	~GameCamera()override {};

	// 現在のステートタイプ
	enum CameraType
	{
		PlayerType, // プレイヤー
		FixedType,  // テレポート解放
		BossType,   // ボス登場
		ClearType   // ゲームクリア 
	};

	void Update()    override;
	void PostUpdate()override;
	void Init()      override;

	void UpdateRotateByMouse()override; // マウスカーソルでカメラを動かす
	void HitCheck(); //壁や地面の当たり判定

	// セッター===================================================================================================
	void SetMatrix           (Math::Matrix _mWorld)                        { m_mWorld = _mWorld; }
	void SetDegAng           (Math::Vector3 _deg)                          { m_DegAngList[m_CameraType] = _deg; }
	void SetPos              (Math::Vector3 _pos)override                  { m_PosList[m_CameraType] = _pos; }
	void SetViewAng          (float _viewAng)                              { m_ViewAngList[m_CameraType] = _viewAng; }
	void SetDegAngList       (Math::Vector3 _player, Math::Vector3 _fixed, Math::Vector3 _boss, Math::Vector3 _clear);
	void SetPosList          (Math::Vector3 _player, Math::Vector3 _fixed, Math::Vector3 _boss, Math::Vector3 _clear);
	void SetViewAngList      (float _player, float _fixed, float _boss, float _clear);
	void SetFixedTarget      (std::shared_ptr<MagicPolygon> _Obj)          { m_FixedTarget = _Obj; }
	void SetBossTarget       (std::shared_ptr<EnemyBase> _boss)            { m_BossTarget = _boss; }
	void SetCameraType       (UINT _cameraType)                            { m_CameraType = _cameraType; }
	void SetStageManager     (std::shared_ptr<StageManager> _stageManager) { m_stageManager = _stageManager; }
	void SetShakeFlg         (bool _shakeFlg)                              { m_shakeFlg = _shakeFlg; }
	void SetChangeClearAngle (float _angle)                                { m_ChangeClearAngle = _angle; }
	void SetChangeShakeAngle (float _angle)                                { m_ChangeShakeAngle = _angle; }
	void SetDefaultShakeMove (float _move)                                 
	{ 
		m_DefaultMove = _move;
		m_move = m_DefaultMove;
	}
	void SetShakeMove        (float _move)                                 { m_move = _move; }
	void SetDefaultShakeTime (int _time)                                   
	{
		m_DefaultShakeTime = _time;
		m_shakeTime = m_DefaultShakeTime;
	}
	//============================================================================================================

	// ゲッター===================================================================================================
	const Math::Vector3&                 GetNowPos()           const { return m_PosList[m_CameraType]; }
	const Math::Vector3&                 GetNowDegAng()        const { return m_DegAngList[m_CameraType]; }
	const std::weak_ptr<MagicPolygon>&   GetFixedTarget()      const { return m_FixedTarget; }
	const std::weak_ptr<EnemyBase>&      GetBossTarget()       const { return m_BossTarget; }
	const std::weak_ptr<Player>&         GetwpTarget()         const { return m_wpTarget; }
	const float&                         GetNowViewAng()       const { return m_ViewAngList[m_CameraType]; }
	const float&                         GetChangeClearAngle() const { return m_ChangeClearAngle; }
	const float&                         GetChangeShakeAngle() const { return m_ChangeShakeAngle; }
	const float&                         GetDefaultMove()      const { return m_DefaultMove; }
	const int&                           GetDefaultShakeTime() const { return m_DefaultShakeTime; }
	const bool                           GetShakeFlg()         const { return m_shakeFlg; }
	const UINT&                          GetCameraType()       const { return m_CameraType; }
	const Math::Matrix                   GetRotationMatrix()   const override
	{
		return Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_DegAngList[m_CameraType].y),
			DirectX::XMConvertToRadians(m_DegAngList[m_CameraType].x),
			DirectX::XMConvertToRadians(m_DegAngList[m_CameraType].z));
	}
	const Math::Matrix GetRotationXMatrix() const override
	{
		return Math::Matrix::CreateRotationX(
			DirectX::XMConvertToRadians(m_DegAngList[m_CameraType].x));
	}

	const Math::Matrix GetRotationYMatrix() const override
	{
		return Math::Matrix::CreateRotationY(
			DirectX::XMConvertToRadians(m_DegAngList[m_CameraType].y));
	}
	//============================================================================================================

	// ステート切り替え===========================================================================================
	void PlayerChange() // プレイヤー
	{
		std::shared_ptr<PlayerCamera> _player = std::make_shared<PlayerCamera>();
		m_NextState = _player;
		m_flow = GameCamera::Flow::EnterType;
		return;
	}
	void FixedChange() // テレポート開放
	{
		std::shared_ptr<FixedCamera> _fixed = std::make_shared<FixedCamera>();
		m_NextState = _fixed;
		m_flow = GameCamera::Flow::EnterType;
		return;
	}
	void BossChange()
	{
		std::shared_ptr<BossCamera> _boss = std::make_shared<BossCamera>();
		m_NextState = _boss;
		m_flow = GameCamera::Flow::EnterType;
		return;
	}
	void ClearChange() // クリア
	{
		std::shared_ptr<ClearCamera> _clear = std::make_shared<ClearCamera>();
		m_NextState = _clear;
		m_flow = GameCamera::Flow::EnterType;
		return;
	}
	//============================================================================================================
private:
	std::vector<Math::Vector3>     m_PosList;                               // それぞれのステートの位置
	std::vector<Math::Vector3>     m_DegAngList;                            // それぞれのステートの角度
	std::vector<float>             m_ViewAngList;                           // それぞれのステートの視野角
	std::weak_ptr<MagicPolygon>    m_FixedTarget;                           // テレポート解放時のターゲット
	std::weak_ptr<EnemyBase>       m_BossTarget;                            // ボス
	std::weak_ptr<StageManager>    m_stageManager;                          // ステージマネージャ
	float                          m_ChangeClearAngle = 0.0f;               // クリア時の回転角度変化量
	float                          m_ChangeShakeAngle = 0.0f;               // 振動時のsinカーブ変化量
	float                          m_DefaultMove      = 0.1f;               // 初期移動量
	float                          m_move             = m_DefaultMove;      // 移動量
	int                            m_shakeTime        = m_DefaultShakeTime; // カメラ振動時間
	int                            m_DefaultShakeTime = 10;                 // 初期カメラ振動時間
	bool                           m_shakeFlg         = false;              // カメラ振動フラグ

	// デバッグ
	bool cursorMoveFlg = false; // マウスカーソル可視化フラグ

private:

	// ステートパターン

	// 基底ステート
	class StateBase
	{
	public:
		StateBase()          {};
		virtual ~StateBase() {};

		void StateUpdate(std::shared_ptr<GameCamera> owner)
		{
			switch (owner->m_flow)
			{
			case Flow::EnterType:
				Enter(owner);
				break;
			case Flow::UpdateType:
				Update(owner);
				break;
			case Flow::ExitType:
				Exit(owner);
				break;
			default:
				break;
			}
		}

		virtual void Enter (std::shared_ptr<GameCamera> owner) {};
		virtual void Update(std::shared_ptr<GameCamera> owner) {};
		virtual void Exit  (std::shared_ptr<GameCamera> owner) {};

		virtual void ChangeState(std::shared_ptr<GameCamera> owner) = 0; // ステート切替

	};

	// プレイヤー
	class PlayerCamera:public StateBase
	{
	public:
		PlayerCamera()          {};
		~PlayerCamera()override {};

		void Enter (std::shared_ptr<GameCamera> owner)override;
		void Update(std::shared_ptr<GameCamera> owner)override;
		void Exit  (std::shared_ptr<GameCamera> owner)override;

		void ChangeState(std::shared_ptr<GameCamera> owner)override;

		void Shake (std::shared_ptr<GameCamera> owner, Math::Matrix& _trans);

	private:

	};		  


	// テレポート解放
	class FixedCamera :public StateBase
	{
	public:
		FixedCamera()          {};
		~FixedCamera()override {};

		void Enter(std::shared_ptr<GameCamera> owner)override;
		void Update(std::shared_ptr<GameCamera> owner)override;
		void Exit(std::shared_ptr<GameCamera> owner)override;

		void ChangeState(std::shared_ptr<GameCamera> owner)override;

	private:

	};

	// ボス登場
	class BossCamera :public StateBase
	{
	public:
		BossCamera()          {};
		~BossCamera()override {};

		void Enter (std::shared_ptr<GameCamera> owner)override;
		void Update(std::shared_ptr<GameCamera> owner)override;
		void Exit  (std::shared_ptr<GameCamera> owner)override;

		void ChangeState(std::shared_ptr<GameCamera> owner)override;
	private:
	};

	// ゲームクリア
	class ClearCamera :public StateBase
	{
	public:
		ClearCamera()          {};
		~ClearCamera()override {};

		void Enter(std::shared_ptr<GameCamera> owner)override;
		void Update(std::shared_ptr<GameCamera> owner)override;
		void Exit(std::shared_ptr<GameCamera> owner)override;

		void ChangeState(std::shared_ptr<GameCamera> owner)override;

	private:

	};

	std::shared_ptr<StateBase> m_state      = nullptr;                // 現在のステート
	std::shared_ptr<StateBase> m_NextState  = nullptr;                // 次のステート
	UINT                       m_CameraType = CameraType::PlayerType; // 現在のステート

public:
	const std::shared_ptr<StateBase>& GetState()const { return m_state; }
};