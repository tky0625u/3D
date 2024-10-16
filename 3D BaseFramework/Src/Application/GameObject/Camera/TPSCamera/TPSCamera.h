#pragma once
#include "../CameraBase.h"

class TPSCamera : public CameraBase
{
public:
	TPSCamera()                         { Init(); }
	~TPSCamera()			override	{}

	void Init()				override;
	void Update()           override;
	void PostUpdate()       override;

	void SetFixedTargetList(std::weak_ptr<KdGameObject> _fixedTarget) { m_FixedTargetList.push_back(_fixedTarget); }
	void SetPlayerTargetPos(Math::Vector3 _playerPos) { m_targetPos = _playerPos; }
	void SetFixedTargetPos(Math::Vector3 _fixPos) { m_fixPos = _fixPos; }
	void SetFixedTargetAngle(Math::Vector2 _fixAngle) { m_fixAngle = _fixAngle; }

private:
	Math::Vector3 m_targetPos = Math::Vector3::Zero;
	std::vector<std::weak_ptr<KdGameObject>> m_FixedTargetList;
	Math::Vector3 m_fixPos = Math::Vector3::Zero;
	Math::Vector2 m_fixAngle = Math::Vector2::Zero;
	bool m_fixFlg = false;
};