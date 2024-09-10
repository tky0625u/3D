#pragma once
#include"../Player_ActionState.h"

class EnemyBase;

class Player_Roll :public Player_ActionState
{
public:
	Player_Roll() {Init();}
	~Player_Roll()  override {};

	void Start()    override;
	void End()      override;

	void Event()    override;

	void Init()     override;

	void Hit(std::shared_ptr<Player_ActionConText> context, int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)override { return; }

private:
};