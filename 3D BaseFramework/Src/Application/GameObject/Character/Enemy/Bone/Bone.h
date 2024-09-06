#pragma once
#include"../../CharacterBase.h"

class Player;
class Enemy_ConText;
class Enemy_ActionState;

class Bone :public CharacterBase
{
public:
	Bone() {}
	~Bone()override {};

	void PreUpdate()override;
	void Action()override;
	void Init()override;

	bool ChaceChaeck();

	void SetPlayer(std::shared_ptr<Player> _player) { m_player = _player; }
	void SetChaseRange(float _chaseRange) { m_chaceRange = _chaseRange; }

private:
	std::weak_ptr<Player> m_player;
	std::shared_ptr<Enemy_ConText> m_conText = nullptr;
	std::weak_ptr<Enemy_ActionState> m_state;
	float                 m_chaceRange = 0.0f;
};