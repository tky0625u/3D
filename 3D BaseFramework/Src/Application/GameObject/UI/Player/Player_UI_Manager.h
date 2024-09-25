#pragma once

class Player;
class UIBase;

class Player_UI_Manager:public KdGameObject
{
public:
	Player_UI_Manager()          {};
	~Player_UI_Manager()override {};

	void Update()       override;
	void DrawSprite()   override;
	void Init()         override;

	void SetPlayer(std::shared_ptr<Player> _player) { m_player = _player; }

private:
	std::list<std::shared_ptr<UIBase>> m_UIList;
	std::weak_ptr<Player>              m_player;
};