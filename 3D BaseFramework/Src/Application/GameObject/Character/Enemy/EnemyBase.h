#pragma once
#include"../CharacterBase.h"

class Enemy_ConText;
class Enemy_ActionState;
class Enemy_HP;
class Player;

class EnemyBase :public CharacterBase, public std::enable_shared_from_this<EnemyBase>
{
public:
	enum Action
	{
		IdolType,
		AttackType,
		RunType,
		StumbleType,
		HitType,
		AppealType
	};

	EnemyBase() {}
	~EnemyBase()override {};

	void Update()        override;
	void PostUpdate()    override;
	virtual void Action()override;
	void DrawSprite()    override;
	virtual void Init()  override;

	void CrushingAction()override;

	void SetTarget(std::shared_ptr<Player> _target) { m_Target = _target; }
	void SetNextAction(std::shared_ptr<Enemy_ActionState> _action,UINT _actionType) { 
		m_NextState = _action;
		m_actionType = _actionType;
	}

	const std::weak_ptr<Player>& GetTarget()const { return m_Target; }
	std::shared_ptr<Enemy_ConText> GetConText()const { return m_conText; }
	Math::Matrix GetHPMat()const { return m_model->FindWorkNode("HP")->m_worldTransform * m_mWorld; }
	Math::Matrix GetAttackStartPointMat()const { return m_model->FindWorkNode("AttackStartPoint")->m_worldTransform * m_mWorld; }
	Math::Matrix GetLockPointMat()const { return m_model->FindWorkNode("LockONPoint")->m_worldTransform * m_mWorld; }
	UINT GetActionType()const { return m_actionType; }

protected:
	std::weak_ptr<Player>              m_Target;
	std::shared_ptr<Enemy_HP>          m_hp         = nullptr;
	std::shared_ptr<Enemy_ConText>     m_conText    = nullptr;
	std::weak_ptr<Enemy_ActionState>   m_state;
	std::shared_ptr<Enemy_ActionState> m_NextState;
	std::weak_ptr<KdEffekseerObject>   m_Effect;
	UINT                               m_actionType = Action::AppealType;
};