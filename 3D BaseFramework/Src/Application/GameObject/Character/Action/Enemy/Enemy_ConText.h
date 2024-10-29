#pragma once
#include"Enemy_ActionState.h"

class Enemy_ConText :public std::enable_shared_from_this<Enemy_ConText>
{
public:
	Enemy_ConText(std::shared_ptr<Enemy_ActionState> initialState) :m_state(initialState) {}
	~Enemy_ConText() {};

	void SetState(std::shared_ptr<Enemy_ActionState> _state) { m_state = _state; }

	std::shared_ptr<Enemy_ActionState> GetState()const { return m_state; }

	void Idol()   { m_state->Idol(); }    //待ち
	void Run()    { m_state->Run(); }     //走り
	void Attack() { m_state->Attack(); }  //攻撃
	void Hit(int _damage)    { m_state->Hit(_damage); }     //被弾
	void Stumble(){ m_state->Stumble(); } //のけぞり

private:
	std::shared_ptr<Enemy_ActionState> m_state;
};