#pragma once
#include"Enemy_ActionState.h"

class Player;

class Enemy_ConText :public std::enable_shared_from_this<Enemy_ConText>
{
public:
	Enemy_ConText(std::shared_ptr<Enemy_ActionState> initialState) :m_state(initialState) {}
	~Enemy_ConText() {};

	void SetState(std::shared_ptr<Enemy_ActionState> _state) { m_state = _state; }

	std::shared_ptr<Enemy_ActionState> GetState()const { return m_state; }

	void Idol()   { m_state->Idol(shared_from_this()); }    //待ち
	void Run()    { m_state->Run(shared_from_this()); }     //走り
	void Attack() { m_state->Attack(shared_from_this()); }  //攻撃
	void Hit()    { m_state->Hit(shared_from_this()); }     //被弾
	void Stumble(){ m_state->Stumble(shared_from_this()); } //のけぞり

private:
	std::shared_ptr<Enemy_ActionState> m_state;
};