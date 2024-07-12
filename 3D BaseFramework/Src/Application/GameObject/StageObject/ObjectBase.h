#pragma once

class ObjectBase :public KdGameObject
{
public:
	ObjectBase()                    {}
	~ObjectBase()          override {};

	 virtual void DrawLit() override;

	void SetPos(Math::Vector3 a_pos) { m_pos = a_pos; }
	void SetModel(const std::shared_ptr<KdModelData>& _model) { m_model = _model; }

protected:
	std::shared_ptr<KdModelData> m_model;
	Math::Vector3                m_pos = Math::Vector3::Zero;
};