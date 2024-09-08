#pragma once
#include"../Enemy_ActionState.h"

class Enemy_Run :public Enemy_ActionState
{
public:
	Enemy_Run() { Init(); }
	~Enemy_Run()   override {};

	void Init()override;

	void Start()    override;
	void Center()   override;
	void End()      override;

	void Event()override;

	void Idol   (std::shared_ptr<Enemy_ConText> context)override;  //待ち
	void Attack (std::shared_ptr<Enemy_ConText> context)override;  //攻撃
	void Hit    (std::shared_ptr<Enemy_ConText> context)override;  //被弾
	void Stumble(std::shared_ptr<Enemy_ConText> context)override;  //のけぞり

private:
	bool m_atkFlg = false;
};