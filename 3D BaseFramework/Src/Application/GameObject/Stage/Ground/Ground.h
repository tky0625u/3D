#pragma once

class Ground :public KdGameObject
{
public:
	Ground() {}
	~Ground()override {};

	void Update()override;
	void DrawLit()override;
	void Init()override;

	void SetMatrix(Math::Matrix _mat) { m_mWorld = _mat; }

private:
	std::shared_ptr<KdModelWork> m_model = nullptr;
};