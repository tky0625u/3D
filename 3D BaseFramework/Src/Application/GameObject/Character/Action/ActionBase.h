#pragma once

class CharacterBase;

class ActionBase :public KdGameObject
{
public:
	enum Flow
	{
		StartType,
		CenterType,
		EndType,
	};

	ActionBase()           {};
	~ActionBase()override  {};

	virtual void Update()override
	{
		if (m_flow == Flow::StartType) { Start(); }
		else if (m_flow == Flow::CenterType) { Center(); }
		else if (m_flow == Flow::EndType) { End(); }
	}

	virtual void Start()   {};
	virtual void Center()  {};
	virtual void End()     {};

	void Rotate(Math::Vector3 _moveDir, std::shared_ptr<CharacterBase> _target);

protected:
	UINT m_flow      = Flow::StartType;
};