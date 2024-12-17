#pragma once

class Circle;
class GameCamera;
class MagicPolygon_ConText;
class MagicPolygon_State;
class StageManager;

class MagicPolygon :public KdGameObject,public std::enable_shared_from_this<MagicPolygon>
{
public:
	enum Flow
	{
		EnterType,
		UpdateType,
		ExitType
	};

	MagicPolygon() {};
	~MagicPolygon()override {};

	void Update()override;
	void DrawLit()override;
	void DrawBright()override;
	void Init()override;

	void NormalChange()
	{
		std::shared_ptr<Normal> _normal = std::make_shared<Normal>();
		m_NextState = _normal;
		m_flow = MagicPolygon::Flow::EnterType;
	}
	void NextChange()
	{
		std::shared_ptr<Next> _next = std::make_shared<Next>();
		m_NextState = _next;
		m_flow = MagicPolygon::Flow::EnterType;
	}

	void SetCircle(std::shared_ptr<Circle> _circle) { m_circle = _circle; }
	void SetMatrix(Math::Matrix _mat) { m_mWorld = _mat; }
	void SetRBG(float _rgb) { m_rgb = _rgb; }
	void SetTeleportFlg(bool _teleport) { m_TeleportFlg = _teleport; }
	void SetStageManager(std::shared_ptr<StageManager> _stageManager) { m_stageManager = _stageManager; }

	const float& GetRGB() const { return m_rgb; }
	const bool GetTeleport()const { return m_TeleportFlg; }

private:
	std::weak_ptr<StageManager>      m_stageManager;
	std::weak_ptr<Circle>            m_circle;
	std::shared_ptr<KdSquarePolygon> m_spPolygon = nullptr;
	Math::Color                      m_color;
	float                            m_rgb       = 0.0f;
	bool                             m_TeleportFlg  = false;

private:
	class StateBase
	{
	public:
		StateBase() {};
		virtual ~StateBase() {};

		virtual void Enter (std::shared_ptr<MagicPolygon> owner) {};
		virtual void Update(std::shared_ptr<MagicPolygon> owner) {};
		virtual void Exit  (std::shared_ptr<MagicPolygon> owner) {};
	};

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

	class Next :public StateBase
	{
	public:
		Next() {};
		~Next()override {};

		void Enter (std::shared_ptr<MagicPolygon> owner)override;
		void Update(std::shared_ptr<MagicPolygon> owner)override;
		void Exit  (std::shared_ptr<MagicPolygon> owner)override;


	private:
		Effekseer::Handle m_handle = 0;
	};

	std::shared_ptr<StateBase>  m_state;
	std::shared_ptr<StateBase>  m_NextState = nullptr;
	UINT                        m_flow = Flow::UpdateType;
};