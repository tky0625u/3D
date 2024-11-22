#pragma once
#include"../CharacterBase.h"

class Enemy_HP;
class Player;

class EnemyBase :public CharacterBase
{
public:
	enum Flow
	{
		EnterType,
		UpdateType,
		ExitType
	};

	enum Action
	{
		IdolType,
		AttackType,
		RunType,
		StumbleType,
		HitType,
		AppealType,
		CrushingType
	};

	EnemyBase() {}
	~EnemyBase()override {};

	void PostUpdate()    override;
	void DrawOutLine();
	virtual void Init()  override;

	void CrushingAction()override;
	virtual void IdolChange() {};
	virtual void RunChange()  {};
	virtual void AttackChange(){};
	virtual void StumbleChange() {};
	virtual void Damage(int _damage) {};

	void SetTarget(std::shared_ptr<Player> _target) { m_Target = _target; }
	void SetLockONFlg(bool _LockONFlg) { m_LockONFlg = _LockONFlg; }

	const std::weak_ptr<Player>& GetTarget()const { return m_Target; }
	Math::Matrix GetHPMat()const { return m_model->FindWorkNode("HP")->m_worldTransform * m_mWorld; }
	Math::Matrix GetAttackStartPointMat()const { return m_model->FindWorkNode("AttackStartPoint")->m_worldTransform * m_mWorld; }
	Math::Matrix GetLockPointMat()const { return m_model->FindWorkNode("LockONPoint")->m_worldTransform * m_mWorld; }
	UINT GetActionType()const { return m_actionType; }
	const bool& GetLockONFlg()const { return m_LockONFlg; }

protected:
	std::weak_ptr<Player>              m_Target;
	std::weak_ptr<KdEffekseerObject>   m_Effect;
	bool                               m_LockONFlg = false;

	UINT                               m_flow = Flow::EnterType;
	UINT                               m_actionType = Action::AppealType;
	UINT                               m_NextActionType = m_actionType;
};