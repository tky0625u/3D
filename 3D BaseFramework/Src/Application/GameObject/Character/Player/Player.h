#pragma once

#include"../CharacterBase.h"

class GameCamera;
class ActionBase;
class Player_ActionConText;
class Player_ActionState;
class Player_UI_Manager;
class EnemyBase;
class Shield;

class Player :public CharacterBase, public std::enable_shared_from_this<Player>
{
public:
	Player()                  {}
	~Player()        override {};

	void Action()    override;
	void Update()    override;
	void PostUpdate()override;
	void DrawSprite()override;
	void Init()      override;

	void CrushingAction()override;
	void StaminaRecovery() { 
		if (m_StaminaRecoveryTime > 0)return;
		m_param.Sm++;
		if (m_param.Sm >= m_MaxStamina)m_param.Sm = m_MaxStamina;
	}

	void SetCamera(std::shared_ptr<GameCamera> a_camera) { m_camera = a_camera; }
	void SetNextState(std::shared_ptr<Player_ActionState> _next) { m_NextState = _next; }
	void SetShield(std::shared_ptr<Shield> _shield) { m_shield = _shield; }
	void SetStaminaRecoveryTime(int _time) { m_StaminaRecoveryTime = _time; }
	void SetParryID(UINT _id) { m_ParryID = _id; }

	std::shared_ptr<Player_ActionConText>GetConText()const { return m_context; }
	int GetMaxStamina()const { return m_MaxStamina; }
	std::weak_ptr<Shield> GetShield() { return m_shield; }
	const int GetStaminaRecoveryTime()const { return m_StaminaRecoveryTime; }
	const UINT GetParryID()const { return m_ParryID; }
	const Math::Matrix& GetEnemyAttackPointMat() const { return (m_model->FindWorkNode("EnemyAttackPoint")->m_worldTransform) * m_mWorld; }
private:
	std::weak_ptr<GameCamera>             m_camera;
	std::shared_ptr<Player_ActionConText> m_context        = nullptr;
	std::weak_ptr<Player_ActionState>     m_state;
	std::shared_ptr<Player_ActionState>   m_NextState;
	std::shared_ptr<Player_UI_Manager>    m_ui             = nullptr;
	std::weak_ptr<Shield>                 m_shield;
	float                                 m_FocusBackRange = 2000.0f;
	int                                   m_MaxStamina     = 0;
	int                                   m_StaminaRecoveryTime = 0;
	UINT                                  m_ParryID = 0;
};