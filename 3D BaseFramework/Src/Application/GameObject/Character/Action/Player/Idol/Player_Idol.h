#pragma once
#include"../Player_ActionState.h"

class CameraBase;

class Player_Idol :public Player_ActionState
{
public:
	Player_Idol() 	{ 
						m_ChangeFlg = true;
						m_flow = Flow::CenterType;
						m_end = false;
					}
	~Player_Idol()   override {};

	void Center()   override;

	void Run(std::shared_ptr<Player_ActionConText> context, std::weak_ptr<CameraBase> _camera)override; //走り
	void Attack(std::shared_ptr<Player_ActionConText> context)        override;              //攻撃
	void Guard(std::shared_ptr<Player_ActionConText> context)         override;              //ガード
	void Roll(std::shared_ptr<Player_ActionConText> context, std::weak_ptr<CameraBase> _camera)override;//回避
	void Hit(std::shared_ptr<Player_ActionConText> context)           override;              //被弾

private:

};