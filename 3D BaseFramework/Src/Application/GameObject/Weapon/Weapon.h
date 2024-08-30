#pragma once

class CharacterBase;

class Weapon :public KdGameObject
{
public:
	Weapon() { Init(); }
	~Weapon()override {};

	void Update()override;
	void GenerateDepthMapFromLight()override;
	void DrawLit()override;
	void Init()override;

	void SetTarget(std::shared_ptr<CharacterBase> _target) { m_target = _target; }

private:
	std::weak_ptr<CharacterBase> m_target;
	std::shared_ptr<KdModelWork> m_spModel = nullptr;
	Math::Vector3 m_pos = Math::Vector3::Zero;
};