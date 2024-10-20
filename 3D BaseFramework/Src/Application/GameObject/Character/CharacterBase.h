﻿#pragma once

class ObjectManager;
class ActionBase;
class Sword;

class CharacterBase :public KdGameObject
{
public:
	 struct Param
	{
		 int   Hp  = 1; //体力
		 int   Atk = 1; //攻撃力
		 float Sp  = 1; //素早さ
		 int   Sm  = 1; //スタミナ
	};

	CharacterBase()                                   { Init(); }
	~CharacterBase()                         override {};

	void Update()                            override;
	void PostUpdate()                        override;
	virtual void GenerateDepthMapFromLight() override;
	virtual void DrawLit()                   override;
	virtual void Init()                      override;

	virtual void Action() {}; //行動
	virtual void CrushingAction(); //やられ演出

	void Hit(int _damege);

	void SetParam(int _hp, int _atk, float _speed, int _stamina);
	void SetStamina(int _stamina) { m_param.Sm = _stamina; }
	void SetATK(int _atk) { m_param.Atk = _atk; }
	void SetAtkRange(float _atkRange) { m_AtkRange = _atkRange; }
	void SetForward(Math::Vector3 _forward) { m_forward = _forward; }
	void SetDir(Math::Vector3 _dir) { m_dir = _dir; }
	void SetAnime(std::string animeName, bool animeFlg, float animeSpeed) {
																			m_anime = animeName;
																			m_animeFlg = animeFlg;
																			m_animeSpeed = animeSpeed;
																		  }
	void SetMove(Math::Vector3 dir,float _moveSpeed=0.0f)
	{
		m_dir = dir;
		m_MoveSpeed = _moveSpeed;
	}
	void SetSword(std::shared_ptr<Sword> _sword) { m_sword = _sword; }
	void SetInviTime(int _inviTime) { m_inviTime = _inviTime; }

	Param GetParam() { return m_param; }
	bool GetIsAnimator() { return m_animator->IsAnimationEnd(); }
	std::string GetAnime() { return m_anime; }
	Math::Matrix GetSwordMat() { return m_model->FindWorkNode("sword.R")->m_localTransform * m_model->FindWorkNode("hand.R")->m_worldTransform * (Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle))*(Math::Matrix::CreateTranslation(m_mWorld.Translation()))); }
	Math::Matrix GetShieldMat() { return m_model->FindWorkNode("shield.L")->m_localTransform * m_model->FindWorkNode("forearm.L")->m_worldTransform * (Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle)) * (Math::Matrix::CreateTranslation(m_mWorld.Translation()))); }
	std::weak_ptr<Sword> GetSword() { return m_sword; }
	int GetinviTime()const { return m_inviTime; }
	const Math::Vector3 GetDir() const { return m_dir; }
	const Math::Vector3 GetForward() const { return m_forward; }
	const float GetAtkRange() const { return m_AtkRange; }

protected:
	Param                        m_param;
	std::shared_ptr<KdModelWork> m_model           = nullptr;
	std::shared_ptr<KdAnimator>  m_animator        = nullptr;
	std::shared_ptr<ActionBase>  m_action          = nullptr;
	std::weak_ptr<Sword>         m_sword;
	std::string                  m_anime           = "Idol";
	std::string                  m_beforeAnime     = m_anime;
	Math::Vector3 m_dir = Math::Vector3::Zero; //方向
	Math::Vector3 m_forward = Math::Vector3::Zero;
	std::unordered_map<std::string, std::shared_ptr<ActionBase>> m_actionList;
	int                          m_inviTime        = 0;
	float                        m_gravity         = 0.0f;
	float                        m_animeSpeed      = 1.0f;
	float                        m_MoveSpeed       = 0.0f;
	float                        m_dossolve        = 0.0f;
	float                        m_AtkRange = 0.0f; //攻撃範囲
	const float                  m_SpeedCorrection = 0.2f;
	const float                  m_gravityPow      = 0.1f;
	bool                         m_atkFlg          = false;
	bool                         m_animeFlg        = true;
};