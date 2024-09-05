#pragma once
#include"../ActionBase.h"

class Player_ActionConText;
class CameraBase;

class Player_ActionState:public ActionBase
{
public:
	Player_ActionState() {};
	~Player_ActionState()override {};

	virtual void Idol(std::shared_ptr<Player_ActionConText> context)          { return; };  //待ち
	virtual void Run(std::shared_ptr<Player_ActionConText> context,std::weak_ptr<CameraBase> _camera)  { return; };  //走り
	virtual void Attack(std::shared_ptr<Player_ActionConText> context)        { return; };  //攻撃
	virtual void Guard(std::shared_ptr<Player_ActionConText> context)         { return; };  //ガード
	virtual void GuardReaction(std::shared_ptr<Player_ActionConText> context) { return; };  //ガード時の反応
	virtual void Parry(std::shared_ptr<Player_ActionConText> context)         { return; };  //パリィ
	virtual void Counter(std::shared_ptr<Player_ActionConText> context)       { return; };  //追撃
	virtual void Roll(std::shared_ptr<Player_ActionConText> context, std::weak_ptr<CameraBase> _camera) { return; };  //回避
	virtual void Hit(std::shared_ptr<Player_ActionConText> context)           { return; };  //被弾

private:

};