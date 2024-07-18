#pragma once

class CharacterBase :public KdGameObject
{
public:
	 struct Status
	{
		 int HP  = 1; //体力
		 int ATK = 1; //攻撃力
		 int MP  = 1; //魔力
		 int DF  = 1; //防御力
		 int SP  = 1; //素早さ
		 int SM  = 1; //スタミナ
	};

	 enum StatusType
	 {
		 HPType,  //体力
		 ATKType, //攻撃力
		 DFType,  //防御力
		 SPType,  //素早さ
		 SMType   //スタミナ
	 };

	 enum CharacterType
	 {
		 OnePlayer = 1 << 0,
		 TwoPlayer = 1 << 1,
		 Leader    = 1 << 2,
		 Normal    = 1 << 3
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

	void StatusLoad(std::string a_filePath); //ステータス読み取り

	void SetPos(Math::Vector3 a_pos) { m_pos = a_pos; }

protected:
	Status                       m_status;
	CharacterType                m_characterType;
	std::shared_ptr<KdModelData> m_model;
	Math::Vector3                m_pos = Math::Vector3::Zero;
	float                        m_angle = 0.0f;
	float                        m_sizeHalf;
	const float                  m_SpeedCorrection = 0.1f;
	const float                  m_gravityPow = 0.1f;
	float                        m_gravity = 0.0f;
	float                        m_atkRange;
};