#pragma once
#include"../Player_ActionState.h"

class CameraBase;

class Player_Roll :public Player_ActionState
{
public:
	Player_Roll() {Init();}
	~Player_Roll()  override {};

	void Start()    override;
	void End()      override;

	void Event()    override;

	void Init()     override;

	void SetCamera(std::weak_ptr<CameraBase> _camera) { m_camera = _camera; }

	void Idol(std::shared_ptr<Player_ActionConText> context)          override;             //待ち

private:
	std::weak_ptr<CameraBase> m_camera;
};