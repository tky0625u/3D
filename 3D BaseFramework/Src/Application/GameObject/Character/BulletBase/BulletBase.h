#pragma once

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

	void SetOwner(UINT _owner)         { m_owner = _owner; }
	void SetDir(Math::Vector3 _dir)    { m_dir = _dir; }
	void SetCrush(bool _crush)         { m_crush = _crush; }

	const Math::Vector3& GetDir()const { return m_dir; }

protected:
	UINT                         m_owner = 0;
	std::shared_ptr<KdModelWork> m_model = nullptr;
	Math::Vector3                m_dir = Math::Vector3::Zero;
	float                        m_speed = 2.0f;
	bool                         m_crush = false;
};