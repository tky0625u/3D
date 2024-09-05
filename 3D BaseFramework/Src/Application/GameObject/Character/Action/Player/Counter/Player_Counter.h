#pragma once
#include"../Player_ActionState.h"

class CameraBase;

class Player_Counter :public Player_ActionState
{
public:
	Player_Counter() { Init(); }
	~Player_Counter()override {};

	void Start()override;
	void End()override;

	void SetTargetPos(Math::Vector3 targetPos) { m_targetPos = targetPos; }

	void Idol(std::shared_ptr<Player_ActionConText> context)          override;              //待ち
	void Run(std::shared_ptr<Player_ActionConText> context, std::weak_ptr<CameraBase> _camera)override;   //走り
	void Attack(std::shared_ptr<Player_ActionConText> context)        override;              //攻撃
	void Guard(std::shared_ptr<Player_ActionConText> context)         override;              //ガード
	void Roll(std::shared_ptr<Player_ActionConText> context, std::weak_ptr<CameraBase> _camera)override; //回避

private:
	Math::Vector3 m_targetPos = Math::Vector3::Zero;
};