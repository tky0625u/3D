#pragma once

class CharacterBase;

class WeaponBase :public KdGameObject
{
public:
	WeaponBase() {}
	~WeaponBase()override {};

	virtual void GenerateDepthMapFromLight()override;
	virtual void DrawLit()override;
	virtual void Init()override;

	void SetTarget(std::shared_ptr<CharacterBase> _target) { m_target = _target; }
	void SetModelPath(std::string _modelPath) { m_modelPath = _modelPath; }

protected:
	std::weak_ptr<CharacterBase> m_target;
	std::shared_ptr<KdModelWork> m_spModel = nullptr;
	std::string m_modelPath;
	Math::Vector3 m_pos = Math::Vector3::Zero;
};