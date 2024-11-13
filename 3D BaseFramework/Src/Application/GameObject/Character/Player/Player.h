#pragma once

#include"../CharacterBase.h"

class ActionBase;
class Player_ActionConText;
class Player_ActionState;
class EnemyBase;
class Shield;
class StageManager;
class ObjectManager;
class StageManager;

class Player :public CharacterBase, public std::enable_shared_from_this<Player>
{
public:
	enum Action
	{
		IdolType,
		AttackType,
		RunType,
		RollType,
		GuardType,
		GuardReactionType,
		ParryType,
		CounterType,
		HitType,
		CrushingType
	};

	Player()                  {}
	~Player()        override {};

	void Update()    override;
	void Action()    override;
	void PostUpdate()override;
	void Init()      override;

	void CrushingAction()override;
	void NextStageCheck();
	void StaminaRecovery() { 
		if (m_StaminaRecoveryTime > 0)return;
		m_param.Sm++;
		if (m_param.Sm >= m_MaxStamina)m_param.Sm = m_MaxStamina;
	}

	void SetNextState(std::shared_ptr<Player_ActionState> _next,UINT _action) 
	{
		m_NextState = _next;
		m_actionType = _action;
	}
	void SetShield(std::shared_ptr<Shield> _shield) { m_shield = _shield; }
	void SetStaminaRecoveryTime(int _time) { m_StaminaRecoveryTime = _time; }
	void SetParryID(UINT _id) { m_ParryID = _id; }
	void SetStageManager(std::shared_ptr<StageManager> _stage) { m_StageManager = _stage; }
	void SetTeleportFlg(bool _teleport) { m_TeleportFlg = _teleport; }

	std::shared_ptr<Player_ActionConText>GetConText()const { return m_context; }
	int GetMaxStamina()const { return m_MaxStamina; }
	std::weak_ptr<Shield> GetShield() { return m_shield; }
	const int GetStaminaRecoveryTime()const { return m_StaminaRecoveryTime; }
	const UINT GetParryID()const { return m_ParryID; }
	const Math::Matrix& GetEnemyAttackPointMat() const { return (m_model->FindWorkNode("EnemyAttackPoint")->m_worldTransform) * m_mWorld; }
	const UINT& GetActionType()const { return m_actionType; }
	const std::weak_ptr<ObjectManager>& GetObjectManager()const { return m_ObjectManager; }
	const bool& GetTeleportFlg()const { return m_TeleportFlg; }

private:
	std::weak_ptr<StageManager>           m_StageManager;
	std::shared_ptr<Player_ActionConText> m_context        = nullptr;
	std::weak_ptr<Player_ActionState>     m_state;
	std::shared_ptr<Player_ActionState>   m_NextState;
	std::weak_ptr<Shield>                 m_shield;
	float                                 m_FocusBackRange = 2000.0f;
	int                                   m_MaxStamina     = 0;
	int                                   m_StaminaRecoveryTime = 0;
	bool                                  m_TeleportFlg    = false;
	UINT                                  m_ParryID = -1;
	UINT                                  m_actionType = Action::IdolType;
};