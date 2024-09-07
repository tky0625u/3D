#pragma once

class Ground :public KdGameObject
{
public:
	Ground() { Init(); }
	~Ground()override {};

	void DrawLit()override;
	void Init()override;

private:
	Math::Vector3 m_pos = Math::Vector3::Zero;
	std::shared_ptr<KdModelWork> m_model = nullptr;
};