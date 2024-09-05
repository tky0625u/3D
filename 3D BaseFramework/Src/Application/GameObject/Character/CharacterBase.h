#pragma once

class ActionBase;

class CharacterBase :public KdGameObject, public std::enable_shared_from_this<CharacterBase>
{
public:
	 struct Param
	{
		 int Hp  = 1; //体力
		 int Atk = 1; //攻撃力
		 float Sp  = 1; //素早さ
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

	void SetParam(int _hp, int _atk, float _speed, int _stamina, float _angle, float _size, float _atkRange, Math::Vector3 _forword);
	void Attack(UINT ObjType);

	void InviON() { m_inviFlg = true; }
	void InviOFF() { m_inviFlg = false; }

	void SetPos(Math::Vector3 a_pos) { m_pos = a_pos; }
	void SetAnime(std::string animeName, bool animeFlg, float animeSpeed) {
																			m_anime = animeName;
																			m_animeFlg = animeFlg;
																			m_animeSpeed = animeSpeed;
																		  }
	void SetMove(Math::Vector3 dir,bool moveFlg){
													m_dir = dir;
													m_moveFlg = moveFlg;
												}
	void SetNextAction(std::string next) { m_NowAction = next; }

	Param& GetParam() { return m_param; }
	bool GetIsAnimator() { return m_animator->IsAnimationEnd(); }
	std::string GetAnime() { return m_anime; }
	Math::Matrix  GetWeaponMat() { return m_model->FindWorkNode("hand.R.001")->m_localTransform * m_model->FindWorkNode("hand.R")->m_worldTransform * (Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_param.Angle))*(Math::Matrix::CreateTranslation(m_mWorld.Translation()))); }

protected:
	Param                        m_param;
	std::shared_ptr<KdModelWork> m_model           = nullptr;
	std::shared_ptr<KdAnimator>  m_animator        = nullptr;
	std::shared_ptr<ActionBase>  m_action          = nullptr;
	std::string                  m_NowAction;
	std::string                  m_anime           = "Idol";
	std::string                  m_beforeAnime     = m_anime;
	std::unordered_map<std::string, std::shared_ptr<ActionBase>> m_actionList;
	Math::Vector3                m_pos             = Math::Vector3::Zero;
	Math::Vector3                m_dir             = Math::Vector3::Zero;
	int                          m_inviTime        = 0;
	int                          m_ParryTime       = 0;
	float                        m_gravity         = 0.0f;
	float                        m_animeSpeed      = 1.0f;
	const float                  m_SpeedCorrection = 0.2f;
	const float                  m_gravityPow      = 0.1f;
	bool                         m_atkFlg          = false;
	bool                         m_animeFlg        = true;
	bool                         m_moveFlg         = false;
	bool                         m_guardFlg        = false;
	bool                         m_inviFlg         = false;
};