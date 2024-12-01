#pragma once

class CharacterBase;

class BulletBase :public KdGameObject,public std::enable_shared_from_this<BulletBase>
{
public:
	BulletBase()                                     {};
	~BulletBase()                           override {};

	virtual void Update()                   override {};
	virtual void PostUpdate()               override {};
	virtual void GenerateDepthMapFromLight()override;
	virtual void DrawLit()                  override;
	virtual void Init()                     override { m_ObjType = KdGameObject::ObjType::oObject; }

	void SetOwner(std::shared_ptr<CharacterBase> _owner)         { m_owner = _owner; }
	void SetDir(Math::Vector3 _dir)    { m_dir = _dir; }
	void SetCrush(bool _crush)         { m_crush = _crush; }
	void SetModel(std::shared_ptr<KdModelWork> _model) { m_model = _model; }

	const Math::Vector3& GetDir()const { return m_dir; }
	const std::weak_ptr<CharacterBase>& GetOwner()const { return m_owner; }

protected:
	std::weak_ptr<CharacterBase> m_owner;
	std::shared_ptr<KdModelWork> m_model = nullptr;
	Math::Vector3                m_dir = Math::Vector3::Zero;
	float                        m_speed = 2.0f;
	bool                         m_crush = false;
};