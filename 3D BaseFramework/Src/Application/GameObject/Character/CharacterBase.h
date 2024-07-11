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
		 MPType,  //魔力
		 DFType,  //防御力
		 SPType,  //素早さ
		 SMType   //スタミナ
	 };

	CharacterBase()                                   { Init(); }
	~CharacterBase()                         override {};

	void Update()                            override;
	virtual void GenerateDepthMapFromLight() override;
	virtual void DrawLit()                   override;
	virtual void Init()                      override;

	virtual void Action() {}; //行動
	virtual void CrushingAction(); //やられ演出

	void StatusLoad(std::string a_filePath); //ステータス読み取り

	void SetPos(Math::Vector3 a_pos) { m_pos = a_pos; }

protected:
	Status                       m_status;
	std::shared_ptr<KdModelData> m_model;
	Math::Vector3                m_pos = Math::Vector3::Zero;
	const float                  m_SpeedCorrection = 0.1f;

	Math::Matrix                 m_trans = Math::Matrix::Identity;
	Math::Matrix                 m_rot = Math::Matrix::Identity;
};