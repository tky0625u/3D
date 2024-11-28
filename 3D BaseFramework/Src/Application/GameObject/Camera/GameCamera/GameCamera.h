#pragma once
#include"../CameraBase.h"

class GameCamera_ConText;
class GameCamera_State;
class MagicPolygon;
class StageManager;

class GameCamera :public CameraBase,public std::enable_shared_from_this<GameCamera>
{
public:
	GameCamera() {};
	~GameCamera()override {};

	enum Flow
	{
		EnterType,
		UpdateType,
		ExitType
	};

	enum CameraType
	{
		PlayerType,
		FixedType,
		ClearType
	};

	void Update()    override;
	void PostUpdate()override;
	void Init()      override;

	void UpdateRotateByMouse()override;

	void SetMatrix(Math::Matrix _mWorld) { m_mWorld = _mWorld; }
	void SetDegAng(Math::Vector3 _deg)   { m_DegAngList[m_CameraType] = _deg; }
	void SetPos(Math::Vector3 _pos)      { m_PosList[m_CameraType] = _pos; }
	void SetViewAng(float _viewAng)      { m_ViewAngList[m_CameraType] = _viewAng; }
	void SetDegAngList(Math::Vector3 _player, Math::Vector3 _fixed, Math::Vector3 _clear);
	void SetPosList(Math::Vector3 _player, Math::Vector3 _fixed, Math::Vector3 _clear);
	void SetViewAngList(float _player, float _fixed, float _clear);
	void SetFixedTarget(std::shared_ptr<MagicPolygon> _Obj) { m_FixedTarget = _Obj; }
	void SetCameraType(UINT _cameraType) { m_CameraType = _cameraType; }
	void SetStageManager(std::shared_ptr<StageManager> _stageManager) { m_stageManager = _stageManager; }

	const Math::Vector3 GetNowPos()   const { return m_PosList[m_CameraType]; }
	const Math::Vector3 GetNowDegAng()const { return m_DegAngList[m_CameraType]; }
	const float GetNowViewAng()const { return m_ViewAngList[m_CameraType]; }
	const std::weak_ptr<Player> GetwpTarget()const { return m_wpTarget; }
	const UINT GetCameraType()const { return m_CameraType; }
	const std::shared_ptr<MagicPolygon> GetFixedTarget()const { return m_FixedTarget; }
	const Math::Matrix GetRotationMatrix()const override
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

	virtual const Math::Matrix GetRotationYMatrix() const override
	{
		return Math::Matrix::CreateRotationY(
			DirectX::XMConvertToRadians(m_DegAngList[m_CameraType].y));
	}

	// デバッグ
	void PlayerChange()
	{
		std::shared_ptr<PlayerCamera> _player = std::make_shared<PlayerCamera>();
		m_NextState = _player;
		m_flow = GameCamera::Flow::EnterType;
		return;
	}
	void FixedChange()
	{
		std::shared_ptr<FixedCamera> _fixed = std::make_shared<FixedCamera>();
		m_NextState = _fixed;
		m_flow = GameCamera::Flow::EnterType;
		return;
	}
	void ClearChange()
	{
		std::shared_ptr<ClearCamera> _clear = std::make_shared<ClearCamera>();
		m_NextState = _clear;
		m_flow = GameCamera::Flow::EnterType;
		return;
	}

private:
	std::vector<Math::Vector3>     m_PosList;
	std::vector<Math::Vector3>     m_DegAngList;
	std::vector<float>             m_ViewAngList;
	std::shared_ptr<MagicPolygon>  m_FixedTarget;
	std::weak_ptr<StageManager>    m_stageManager;

	// デバッグ
	bool showFlg = false;

private:
	class StateBase
	{
	public:
		StateBase()          {};
		virtual ~StateBase() {};

		virtual void Enter (std::shared_ptr<GameCamera> owner) {};
		virtual void Update(std::shared_ptr<GameCamera> owner) {};
		virtual void Exit  (std::shared_ptr<GameCamera> owner) {};

		virtual void ChangeState(std::shared_ptr<GameCamera> owner) = 0;

		void SetShakeFlg(bool _shakeFlg) { m_shakeFlg = _shakeFlg; }

		const float& GetShakeFlg()const  { return m_shakeFlg; }

	protected:
		float m_move      = 0.1f;
		float m_shakeTime = 10.0f;
		bool  m_shakeFlg  = false;
	};

	class PlayerCamera:public StateBase
	{
	public:
		PlayerCamera()          {};
		virtual ~PlayerCamera() {};

		void Enter (std::shared_ptr<GameCamera> owner)override;
		void Update(std::shared_ptr<GameCamera> owner)override;
		void Exit  (std::shared_ptr<GameCamera> owner)override;

		void ChangeState(std::shared_ptr<GameCamera> owner)override;

		void LockON(std::shared_ptr<GameCamera> owner);
		void Shake (std::shared_ptr<GameCamera> owner, Math::Matrix& _trans);

	private:

	};		  

	class FixedCamera :public StateBase
	{
	public:
		FixedCamera()          {};
		virtual ~FixedCamera() {};

		void Enter(std::shared_ptr<GameCamera> owner)override;
		void Update(std::shared_ptr<GameCamera> owner)override;
		void Exit(std::shared_ptr<GameCamera> owner)override;

		void ChangeState(std::shared_ptr<GameCamera> owner)override;

	private:

	};

	class ClearCamera :public StateBase
	{
	public:
		ClearCamera()          {};
		virtual ~ClearCamera() {};

		void Enter(std::shared_ptr<GameCamera> owner)override;
		void Update(std::shared_ptr<GameCamera> owner)override;
		void Exit(std::shared_ptr<GameCamera> owner)override;

		void ChangeState(std::shared_ptr<GameCamera> owner)override;

	private:

	};

	std::shared_ptr<StateBase> m_state      = nullptr;
	std::shared_ptr<StateBase> m_NextState  = nullptr;
	UINT                       m_flow       = Flow::UpdateType;
	UINT                       m_CameraType = CameraType::PlayerType;

public:
	const std::shared_ptr<StateBase>& GetState()const { return m_state; }
};