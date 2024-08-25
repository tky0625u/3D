#pragma once
#include"../../CharacterBase.h"

class Player;

class Bone :public CharacterBase
{
public:
	Bone() {}
	~Bone()override {};

	void Action()override;
	void Init()override;

	bool ChaceChaeck();

	void SetPlayer(std::shared_ptr<Player> _player) { m_player = _player; }

private:
	std::weak_ptr<Player> m_player;
	float                 m_chaceRange = 10.0f;
};