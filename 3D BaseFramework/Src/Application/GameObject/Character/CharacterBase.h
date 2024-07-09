#pragma once

class CharacterBase :public KdGameObject
{
public:
	CharacterBase()                                   { Init(); }
	~CharacterBase()                         override {};

	virtual void Update()                    override;
	virtual void GenerateDepthMapFromLight() override;
	virtual void DrawLit()                   override;
	virtual void Init()                      override;

	void SetPos(Math::Vector3 a_pos) { m_pos = a_pos; }

protected:
	std::shared_ptr<KdModelData> m_model;
	Math::Vector3                m_pos = Math::Vector3::Zero;
	float                        m_move;
};