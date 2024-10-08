#pragma once

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

		 Math::Vector3 Pos = Math::Vector3::Zero; //座標
		 Math::Vector3 Dir = Math::Vector3::Zero; //方向
		 float  Angle    = 0.0f; //角度
		 float  Size     = 1.0f; //大きさ
		 float  ErrorNum = 0.0f; //モデルの原点の誤差
		 float  AtkRange = 0.0f; //攻撃範囲
		 float  ForwardX = 0.0f; //前方方向X
		 float  ForwardY = 0.0f; //前方方向Y
		 float  ForwardZ = 0.0f; //前方方向Z
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

	void SetParam(int _hp, int _atk, float _speed, int _stamina, Math::Vector3 _pos, Math::Vector3 _dir, float _angle, float _size, float _atkRange, Math::Vector3 _forword);

	void SetPos(Math::Vector3 a_pos) { m_param.Pos = a_pos; }
	void SetAngle(float _angle) { m_param.Angle = _angle; }
	void SetStamina(int _stamina) { m_param.Sm = _stamina; }
	void SetATK(int _atk) { m_param.Atk = _atk; }
	void SetAnime(std::string animeName, bool animeFlg, float animeSpeed) {
																			m_anime = animeName;
																			m_animeFlg = animeFlg;
																			m_animeSpeed = animeSpeed;
																		  }
	void SetMove(Math::Vector3 dir,float _moveSpeed=0.0f)
	{
		m_param.Dir = dir;
		m_MoveSpeed = _moveSpeed;
	}
	void SetSword(std::shared_ptr<Sword> _sword) { m_sword = _sword; }

	Param GetParam() { return m_param; }
	bool GetIsAnimator() { return m_animator->IsAnimationEnd(); }
	std::string GetAnime() { return m_anime; }
	Math::Matrix GetSwordMat() { return m_model->FindWorkNode("sword.R")->m_localTransform * m_model->FindWorkNode("hand.R")->m_worldTransform * (Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_param.Angle))*(Math::Matrix::CreateTranslation(m_mWorld.Translation()))); }
	Math::Matrix GetShieldMat() { return m_model->FindWorkNode("shield.L")->m_localTransform * m_model->FindWorkNode("forearm.L")->m_worldTransform * (Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_param.Angle)) * (Math::Matrix::CreateTranslation(m_mWorld.Translation()))); }
	std::weak_ptr<Sword> GetSword() { return m_sword; }
	Math::Vector3 GetPos()const override { return m_param.Pos; }

protected:
	Param                        m_param;
	std::shared_ptr<KdModelWork> m_model           = nullptr;
	std::shared_ptr<KdAnimator>  m_animator        = nullptr;
	std::shared_ptr<ActionBase>  m_action          = nullptr;
	std::weak_ptr<Sword>         m_sword;
	std::string                  m_anime           = "Idol";
	std::string                  m_beforeAnime     = m_anime;
	std::unordered_map<std::string, std::shared_ptr<ActionBase>> m_actionList;
	float                        m_gravity         = 0.0f;
	float                        m_animeSpeed      = 1.0f;
	float                        m_MoveSpeed       = 0.0f;
	float                        m_dossolve        = 0.0f;
	const float                  m_SpeedCorrection = 0.2f;
	const float                  m_gravityPow      = 0.1f;
	bool                         m_atkFlg          = false;
	bool                         m_animeFlg        = true;
};