#pragma once
#include"../CharacterBase.h"

class Player;
class Enemy_ConText;
class Enemy_ActionState;
class Enemy_UI;

class EnemyBase :public CharacterBase, public std::enable_shared_from_this<EnemyBase>
{
public:
	EnemyBase() {}
	~EnemyBase()override {};

	void Update()        override;
	virtual void Action()override;
	void DrawSprite()    override;
	virtual void Init()  override;

	void CrushingAction()override;

	void SetPlayer(std::shared_ptr<Player> _player) { m_player = _player; }
	void SetChaseRange(float _chaseRange) { m_chaceRange = _chaseRange; }
	void SetNextState(std::shared_ptr<Enemy_ActionState> _next) { m_NextState = _next; }
	float GetChaseRange()const { return m_chaceRange; }
	std::shared_ptr<Enemy_ConText> GetConText()const { return m_conText; }
	std::weak_ptr<Player> GetPlayer()const { return m_player; }
	int GetStopTime()const { return m_StopTime; }
	Math::Matrix GetHPMat()const { return m_model->FindWorkNode("HP")->m_worldTransform * m_mWorld; }

protected:
	std::shared_ptr<Enemy_UI>          m_ui = nullptr;
	std::shared_ptr<Enemy_ConText>     m_conText = nullptr;
	std::weak_ptr<Player> m_player;
	std::weak_ptr<Enemy_ActionState>   m_state;
	std::shared_ptr<Enemy_ActionState> m_NextState;
	std::weak_ptr<KdEffekseerObject>   m_Effect;
	float                              m_chaceRange = 0.0f;
};