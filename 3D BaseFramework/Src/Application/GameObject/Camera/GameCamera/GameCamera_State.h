#pragma once
#include"../CameraBase.h"

class GameCamera;

class GameCamera_State :public CameraBase
{
public:
	GameCamera_State() {};
	~GameCamera_State() override {};

	enum Flow
	{
		StartType,
		CenterType,
		EndType
	};

	void Player();
	void Fixed();
	void Clear();

	void SetTarget(std::shared_ptr<GameCamera> _target) { m_target = _target; }
	void SetDegAng(Math::Vector3 _deg) { m_DegAng = _deg; }
	void SetPos(Math::Vector3 _pos) { m_pos = _pos; }

protected:
	std::weak_ptr<GameCamera> m_target;
	Flow                      m_flow = StartType;
};