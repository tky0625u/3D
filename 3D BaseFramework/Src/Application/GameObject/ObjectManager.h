#pragma once

class Player;

class ObjectManager
{
public:
	ObjectManager() {};
	~ObjectManager() {};

	void SetObjectParam();
	void SetPlayerParam();
	void SetWeaponParam();
	void SetEnemyParam(std::string _StageNum);

private:
	std::weak_ptr<Player> m_player;
};