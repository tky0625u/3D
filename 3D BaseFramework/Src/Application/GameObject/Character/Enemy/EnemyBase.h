#pragma once
#include"../CharacterBase.h"

class Player;
class Enemy_ConText;
class Enemy_ActionState;

class EnemyBase :public CharacterBase, public std::enable_shared_from_this<EnemyBase>
{
public:
	EnemyBase() {}
	~EnemyBase()override {};

	virtual void PreUpdate()override;
	virtual void Action()override;
	virtual void Init()override;

	void SetPlayer(std::shared_ptr<Player> _player) { m_player = _player; }
	void SetChaseRange(float _chaseRange) { m_chaceRange = _chaseRange; }
	float GetChaseRange()const { return m_chaceRange; }

protected:
	std::weak_ptr<Player> m_player;
	std::shared_ptr<Enemy_ConText> m_conText = nullptr;
	std::weak_ptr<Enemy_ActionState> m_state;
	float                 m_chaceRange = 0.0f;
};