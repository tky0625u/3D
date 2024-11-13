#pragma once
#include"../../UIBase.h"

class Player;

class Teleport :public UIBase
{
public:
	Teleport()          {};
	~Teleport()override {};

	void Update()    override;
	void DrawSprite()override;
	void Init()      override;

	void SetPlayer(std::shared_ptr<Player> _player) { m_player = _player; }

private:
	std::weak_ptr<Player> m_player;
	float                 m_alpha = 1.0f;
};