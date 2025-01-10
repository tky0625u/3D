#pragma once

// ステージマネジャ
class StageManager;
// ゲームカメラ
class GameCamera;
// 魔法陣の台
class Circle;

class MagicPolygon :public KdGameObject,public std::enable_shared_from_this<MagicPolygon>
{
public:
	MagicPolygon() {};
	~MagicPolygon()override {};

	void Update()override;
	void DrawLit()override;
	void DrawBright()override;
	void Init()override;

	// 状態変更================================================================
	// 通常
	void NormalChange()
	{
		std::shared_ptr<Normal> _normal = std::make_shared<Normal>();
		m_NextState = _normal;
		m_flow = MagicPolygon::Flow::EnterType;
	}
	// 解放
	void NextChange()
	{
		std::shared_ptr<Next> _next = std::make_shared<Next>();
		m_NextState = _next;
		m_flow = MagicPolygon::Flow::EnterType;
	}
	//=========================================================================

	// セッター=============================================================================================================
	void SetStageManager(std::shared_ptr<StageManager> _stageManager) { m_stageManager = _stageManager; } // ステージマネジャ
	void SetCircle      (std::shared_ptr<Circle> _circle)             { m_circle = _circle; }             // 魔法陣の台
	void SetMatrix      (Math::Matrix _mat)                           { m_mWorld = _mat; }                // 行列
	void SetRGBChange   (float _change)                               { m_rgbChange = _change; }          // RGB値変化量
	void SetChangeAngle (float _change)                               { m_ChangeAngle = _change; }        // 角度変化量
	void SetTeleportFlg (bool _teleport)                              { m_TeleportFlg = _teleport; }      // テレポート可能フラグ
	//======================================================================================================================

	// ゲッター=============================================================================================================
	const float& GetRGBChange()  const { return m_rgbChange; }   // RGB値変化量
	const float& GetChangeAngle()const { return m_ChangeAngle; } // 角度変化量
	const bool   GetTeleport()   const { return m_TeleportFlg; } // テレポート可能フラグ
	//======================================================================================================================

private:
	// ステージマネジャ
	std::weak_ptr<StageManager>      m_stageManager;
	// 魔法陣の台
	std::weak_ptr<Circle>            m_circle;
	// ポリゴン
	std::shared_ptr<KdSquarePolygon> m_spPolygon    = nullptr;
	// 色
	Math::Color                      m_color;
	// RGB値
	float                            m_rgb          = 0.0f;
	// RGB値変化量
	float                            m_rgbChange    = 0.0f;
	// 角度変化量
	float                            m_ChangeAngle  = 0.0f;
	// テレポート可能フラグ
	bool                             m_TeleportFlg  = false;

private:
	// ステート基底
	class StateBase
	{
	public:
		StateBase() {};
		virtual ~StateBase() {};

		// ステート更新
		void SatateUpdate(std::shared_ptr<MagicPolygon> owner)
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
			}
		}

		virtual void Enter (std::shared_ptr<MagicPolygon> owner) {};
		virtual void Update(std::shared_ptr<MagicPolygon> owner) {};
		virtual void Exit  (std::shared_ptr<MagicPolygon> owner) {};
	};

	// 通常
	class Normal :public StateBase
	{
	public:
		Normal() {};
		~Normal()override {};

		void Enter (std::shared_ptr<MagicPolygon> owner)override;
		void Update(std::shared_ptr<MagicPolygon> owner)override;
		void Exit  (std::shared_ptr<MagicPolygon> owner)override;


	private:

	};

	// 解放
	class Next :public StateBase
	{
	public:
		Next() {};
		~Next()override {};

		void Enter (std::shared_ptr<MagicPolygon> owner)override;
		void Update(std::shared_ptr<MagicPolygon> owner)override;
		void Exit  (std::shared_ptr<MagicPolygon> owner)override;


	private:
		// エフェクト用ハンドル
		Effekseer::Handle m_handle = 0;
	};

	// ステート
	std::shared_ptr<StateBase>  m_state;
	// 次のステート
	std::shared_ptr<StateBase>  m_NextState = nullptr;
};