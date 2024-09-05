#pragma once
#include"../Player_ActionState.h"

class CameraBase;

class Player_Attack :public Player_ActionState
{
public:
	Player_Attack() { Init(); }
	~Player_Attack()   override {};

	void Center()   override;
	void End()      override;

	void Init()     override;

	void Idol(std::shared_ptr<Player_ActionConText> context)          override;             //待ち
	void Run(std::shared_ptr<Player_ActionConText> context, std::weak_ptr<CameraBase> _camera)override;//走り
	void Attack(std::shared_ptr<Player_ActionConText> context)        override;             //攻撃
	void Guard(std::shared_ptr<Player_ActionConText> context)         override;             //ガード
	void Roll(std::shared_ptr<Player_ActionConText> context, std::weak_ptr<CameraBase> _camera)override;//回避
	void Hit(std::shared_ptr<Player_ActionConText> context)           override;             //被弾

private:
	static const int AttackNUM = 3;
	int m_atkNum = 1;
};