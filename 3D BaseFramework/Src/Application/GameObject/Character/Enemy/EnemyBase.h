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

	virtual void Action()override;
	virtual void Init()override;

	void SetPlayer(std::shared_ptr<Player> _player) { m_player = _player; }
	void SetChaseRange(float _chaseRange) { m_chaceRange = _chaseRange; }
	void SetNextState(std::shared_ptr<Enemy_ActionState> _next) { m_NextState = _next; }
	float GetChaseRange()const { return m_chaceRange; }
	std::shared_ptr<Enemy_ConText> GetConText()const { return m_conText; }
	std::weak_ptr<Player> GetPlayer()const { return m_player; }

protected:
	std::weak_ptr<Player> m_player;
	std::shared_ptr<Enemy_ConText> m_conText = nullptr;
	std::weak_ptr<Enemy_ActionState> m_state;
	std::weak_ptr<Enemy_ActionState> m_NextState;
	float                 m_chaceRange = 0.0f;
};