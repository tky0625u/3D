#pragma once
#include"../Player_ActionState.h"

class CameraBase;

class Player_Parry :public Player_ActionState
{
public:
	Player_Parry() { Init(); }
	~Player_Parry()override {};

	void Start()override;
	void End()override;

	void Idol(std::shared_ptr<Player_ActionConText> context)    override; //待ち
	void Run(std::shared_ptr<Player_ActionConText> context)     override; //走り
	void Attack(std::shared_ptr<Player_ActionConText> context)  override; //攻撃
	void Roll(std::shared_ptr<Player_ActionConText> context)    override; //回避
	void Counter(std::shared_ptr<Player_ActionConText> context) override; //追撃

private:

};