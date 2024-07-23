#pragma once

class CharacterBase :public KdGameObject
{
public:
	 struct Param
	{
		 int Hp  = 1; //体力
		 int Atk = 1; //攻撃力
		 int Df  = 1; //防御力
		 int Sp  = 1; //素早さ
		 int Sm  = 1; //スタミナ

		 float  Angle    = 0.0f; //角度
		 float  Size     = 1.0f; //大きさ
		 float  ErrorNum = 0.0f; //モデルの原点の誤差
		 float  AtkRange; //攻撃範囲
		 float  ForwardX = 0.0f; //前方方向X
		 float  ForwardY = 0.0f; //前方方向Y
		 float  ForwardZ = 0.0f; //前方方向Z
	};

	 enum ParamType
	 {
		 HpType,  //体力
		 AtkType, //攻撃力
		 DfType,  //防御力
		 SpType,  //素早さ
		 SmType,  //スタミナ
		 AgType,  //角度
		 SzType,  //大きさ
		 EnType,  //モデルの原点の誤差
		 ArType,  //攻撃範囲
		 FXType,  //前方方向X
		 FYType,  //前方方向Y
		 FZType,  //前方方向Z
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

	void Rotation(Math::Vector3 _moveDir);

	void ParamLoad(std::string a_filePath); //ステータス読み取り

	void SetPos(Math::Vector3 a_pos) { m_pos = a_pos; }

protected:
	Param                        m_param;
	std::shared_ptr<KdModelWork> m_model;
	std::shared_ptr<KdAnimator>  m_animator;
	std::string                  m_Action = "Idol";
	std::string                  m_beforeAction    = m_Action;
	Math::Vector3                m_pos             = Math::Vector3::Zero;
	float                        m_gravity         = 0.0f;
	const float                  m_SpeedCorrection = 0.1f;
	const float                  m_gravityPow      = 0.1f;
	bool                         m_atkFlg          = false;
	bool                         m_animeFlg        = true;
};