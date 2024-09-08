#pragma once
#include"Player_ActionState.h"
#include"../../../Camera/CameraBase.h"

class CameraBase;

class Player_ActionConText:public std::enable_shared_from_this<Player_ActionConText>
{
public:
	Player_ActionConText(std::shared_ptr<Player_ActionState> initialState):m_state(initialState){}
	~Player_ActionConText() {};

	void SetState(std::shared_ptr<Player_ActionState> _state)
	{
		m_state = _state;
	}

	std::shared_ptr<Player_ActionState> GetState()const { return m_state; }

	void Idol() { m_state->Idol(shared_from_this()); }                   //待ち
	void Run() { m_state->Run(shared_from_this()); }            //走り
	void Attack() {m_state->Attack(shared_from_this()); }                //攻撃
	void Guard() {m_state->Guard(shared_from_this()); }                  //ガード
	void GuardReaction() {m_state->GuardReaction(shared_from_this()); }  //ガード時の反応
	void Parry() { m_state->Parry(shared_from_this()); }                 //パリィ
	void Counter() { m_state->Counter(shared_from_this()); }             //追撃
	void Roll() { m_state->Roll(shared_from_this()); }          //回避
	void Hit(int _damage,Math::Vector3 _pos) {m_state->Hit(shared_from_this(),_damage,_pos); }                      //被弾

private:
	std::shared_ptr<Player_ActionState> m_state;
};