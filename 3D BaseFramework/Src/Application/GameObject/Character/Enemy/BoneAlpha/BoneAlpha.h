﻿#pragma once
#include"../Bone/Bone.h"

class BoneAlpha :public Bone
{
public:
	BoneAlpha() {};
	~BoneAlpha()override {};

	void Init()  override;

	void AttackChange()override
	{
		std::shared_ptr<Attack> _attack = std::make_shared<Attack>();
		m_NextState = _attack;
		m_NextActionType = Action::AttackType;
		m_flow = EnemyBase::Flow::EnterType;
	}

private:

	class Attack :public Bone::Attack
	{
	public:
		Attack() {};
		~Attack()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

	private:
	};
};