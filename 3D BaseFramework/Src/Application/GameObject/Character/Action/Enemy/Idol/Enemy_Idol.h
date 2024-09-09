#pragma once
#include"../Enemy_ActionState.h"

class Enemy_Idol :public Enemy_ActionState
{
public:
	Enemy_Idol() {
		m_ChangeFlg = true;
		m_flow = Flow::CenterType;
		m_end = false;
	}
	~Enemy_Idol()   override {};

	void Center()   override;
	void Event()    override;

	void Run    (std::shared_ptr<Enemy_ConText> context)override;  //走り
	void Attack (std::shared_ptr<Enemy_ConText> context)override;  //攻撃
	void Hit    (std::shared_ptr<Enemy_ConText> context,int _damage)override;  //被弾
	void Stumble(std::shared_ptr<Enemy_ConText> context)override;  //のけぞり

private:
};