#pragma once
#include"../Player_ActionState.h"

class CharacterBase;

class Player_Guard :public Player_ActionState
{
public:
	Player_Guard() { Init(); }
	~Player_Guard()   override {};

	void Start()    override;
	void Center()   override;
	void End()      override;

	void Init()     override;
	void Event()    override;

	void Idol(std::shared_ptr<Player_ActionConText> context)          override; //待ち
	void GuardReaction(std::shared_ptr<Player_ActionConText> context) override; //ガード時の反応
	void Parry(std::shared_ptr<Player_ActionConText> context)         override; //パリィ
	void Hit(std::shared_ptr<Player_ActionConText> context)           override; //被弾

private:
	int m_guardTime = 0;
};