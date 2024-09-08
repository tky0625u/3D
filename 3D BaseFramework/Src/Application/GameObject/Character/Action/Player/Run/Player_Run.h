#pragma once
#include"../../ActionBase.h"
#include"../Player_ActionState.h"

class Player_Run :public Player_ActionState
{
public:			    
	Player_Run()             { Init(); }
	~Player_Run()   override {};
				    
	void Start()    override;
	void Center()   override;
	void End()      override;

	void Event()override;
	void CameraTransform(Math::Vector3& _dir);

	void Idol(std::shared_ptr<Player_ActionConText> context)          override; //待ち
	void Attack(std::shared_ptr<Player_ActionConText> context)        override; //攻撃
	void Guard(std::shared_ptr<Player_ActionConText> context)         override; //ガード
	void Roll(std::shared_ptr<Player_ActionConText> context)          override; //回避
	void Hit(std::shared_ptr<Player_ActionConText> context)           override; //被弾

private:

};