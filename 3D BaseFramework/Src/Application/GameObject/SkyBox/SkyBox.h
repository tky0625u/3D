#pragma once

class SkyBox :public KdGameObject
{
public:
	SkyBox()                  { Init(); }
	~SkyBox()        override {};

	void Update()    override;
	void DrawUnLit() override;
	void Init()      override;

	void SetTarget(std::shared_ptr<KdGameObject> _target) { m_target = _target; }

private:
	
	std::weak_ptr<KdGameObject> m_target;

	std::shared_ptr<KdModelWork> m_model = nullptr;
	Math::Vector3                m_pos   = Math::Vector3::Zero;
	float                        m_size  = 5.0f;
	float                        m_angle = 0.0f;
};