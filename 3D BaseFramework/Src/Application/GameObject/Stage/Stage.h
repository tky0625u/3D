#pragma once

class Stage :public KdGameObject
{
public:
	Stage() { Init(); }
	~Stage()override {};

	void DrawLit()override;
	void Init()override;

private:
	Math::Vector3 m_pos = Math::Vector3::Zero;
	std::shared_ptr<KdModelWork> m_model = nullptr;
};