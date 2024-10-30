#pragma once
#include"../CameraBase.h"

class GameCamera_ConText;
class GameCamera_State;

class GameCamera :public CameraBase,public std::enable_shared_from_this<GameCamera>
{
public:
	GameCamera() {};
	~GameCamera()override {};

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
	void SlowChange(bool _slowFlg);

	void SetMatrix(Math::Matrix _mWorld) { m_mWorld = _mWorld; }
	void SetDegAng(Math::Vector3 _deg)   { m_DegAngList[m_CameraType] = _deg; }
	void SetPos(Math::Vector3 _pos)      { m_PosList[m_CameraType] = _pos; }
	void SetViewAng(float _viewAng)      { m_ViewAngList[m_CameraType] = _viewAng; }
	void SetDegAngList(Math::Vector3 _player, Math::Vector3 _fixed, Math::Vector3 _clear);
	void SetPosList(Math::Vector3 _player, Math::Vector3 _fixed, Math::Vector3 _clear);
	void SetViewAngList(float _player, float _fixed, float _clear);
	void SetFixedTargetList(std::shared_ptr<KdGameObject> _Obj) { m_FixedTargetList.push_back(_Obj); }
	void SetNextState(std::shared_ptr<GameCamera_State> _state) { m_NextState = _state; }
	void SetCameraType(UINT _cameraType) { m_CameraType = _cameraType; }

	const Math::Vector3 GetNowPos()   const { return m_PosList[m_CameraType]; }
	const Math::Vector3 GetNowDegAng()const { return m_DegAngList[m_CameraType]; }
	const float GetNowViewAng()const { return m_ViewAngList[m_CameraType]; }
	const std::weak_ptr<Player> GetwpTarget()const { return m_wpTarget; }
	const UINT GetCameraType()const { return m_CameraType; }
	const std::shared_ptr<GameCamera_ConText> GetConText()const { return m_conText; }
	const POINT GetFixMousePos()const { return m_FixMousePos; }
	const std::vector<std::shared_ptr<KdGameObject>> GetFixedTargetList()const { return m_FixedTargetList; }
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

private:
	std::shared_ptr<GameCamera_ConText> m_conText = nullptr;
	std::weak_ptr<GameCamera_State>     m_state;
	std::shared_ptr<GameCamera_State>   m_NextState;

	UINT                           m_CameraType = CameraType::PlayerType;
	std::vector<Math::Vector3>     m_PosList;
	std::vector<Math::Vector3>     m_DegAngList;
	std::vector<float>             m_ViewAngList;
	std::vector<std::shared_ptr<KdGameObject>> m_FixedTargetList;
};