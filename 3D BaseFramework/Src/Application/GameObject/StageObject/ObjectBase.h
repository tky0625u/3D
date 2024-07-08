#pragma once

class ObjectBase :public KdGameObject
{
public:
	ObjectBase()                    { Init(); }
	~ObjectBase()          override {};

	virtual void DrawLit() override;
	virtual void Init()    override;

	void SetPos(Math::Vector3 a_pos) { m_pos = a_pos; }

protected:
	std::shared_ptr<KdModelData> m_model;
	Math::Vector3                m_pos = Math::Vector3::Zero;
};