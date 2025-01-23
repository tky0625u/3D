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
	void SetMatrix(Math::Matrix _mWorld);
	void SetDegAng(Math::Vector3 _deg);
	void SetPos(Math::Vector3 _pos)override;
	void SetViewAng(float _viewAng);
	void SetDegAngList       (Math::Vector3 _player, Math::Vector3 _fixed, Math::Vector3 _boss, Math::Vector3 _clear);
	void SetPosList          (Math::Vector3 _player, Math::Vector3 _fixed, Math::Vector3 _boss, Math::Vector3 _clear);
	void SetViewAngList      (float _player, float _fixed, float _boss, float _clear);
	void SetFixedTarget(std::shared_ptr<MagicPolygon> _Obj);
	void SetBossTarget(std::shared_ptr<EnemyBase> _boss);
	void SetCameraType(UINT _cameraType);
	void SetStageManager(std::shared_ptr<StageManager> _stageManager);
	void SetShakeFlg(bool _shakeFlg);
	void SetChangeClearAngle(float _angle);
	void SetChangeShakeAngle(float _angle);
	void SetDefaultShakeMove(float _move);
	void SetShakeMove(float _move);
	void SetDefaultShakeTime(int _time);
	//============================================================================================================

	// ゲッター===================================================================================================
	const Math::Vector3& GetNowPos()const;
	const Math::Vector3& GetNowDegAng()const;
	const float& GetNowViewAng()const;
	const std::weak_ptr<MagicPolygon>& GetFixedTarget()const;
	const std::weak_ptr<EnemyBase>& GetBossTarget()const;
	const std::weak_ptr<Player>& GetwpTarget()const;
	const float& GetChangeClearAngle()const;
	const float& GetChangeShakeAngle()const;
	const float& GetDefaultMove()const;
	const int& GetDefaultShakeTime()const;
	const bool GetShakeFlg()const;
	const UINT& GetCameraType()const;
	const Math::Matrix GetRotationMatrix()const override;
	const Math::Matrix GetRotationXMatrix() const override;
	const Math::Matrix GetRotationYMatrix() const override;
	//============================================================================================================

	// ステート切り替え===========================================================================================
	void PlayerChange(); // プレイヤー
	void FixedChange();  // テレポート開放
	void BossChange();   // ボス
	void ClearChange();  // クリア
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
	int                            m_DefaultShakeTime = 10;                 // 初期カメラ振動時間
	int                            m_shakeTime        = m_DefaultShakeTime; // カメラ振動時間
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

		void StateUpdate(std::shared_ptr<GameCamera> owner);

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
	const std::shared_ptr<StateBase>& GetState()const;
};