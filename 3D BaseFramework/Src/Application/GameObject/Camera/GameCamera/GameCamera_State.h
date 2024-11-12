#pragma once
#include"../CameraBase.h"

class GameCamera;

class GameCamera_State :public CameraBase
{
public:
	GameCamera_State() {};
	~GameCamera_State() override {};

	void PlayerChange();
	void FixedChange();
	void ClearChange();

	void Shake();

	void SetTarget(std::shared_ptr<GameCamera> _target) { m_target = _target; }
	void SetDegAng(Math::Vector3 _deg) { m_DegAng = _deg; }
	void SetPos(Math::Vector3 _pos) { m_pos = _pos; }
	void SetShakeFlg(bool _shakeFlg) { m_shakeFlg = _shakeFlg; }

	const float& GetShakeFlg()const { return m_shakeFlg; }

protected:
	std::weak_ptr<GameCamera> m_target;
	float                     m_move = 0.1f;
	float                     m_shakeTime = 10.0f;
	bool                      m_shakeFlg = false;
};