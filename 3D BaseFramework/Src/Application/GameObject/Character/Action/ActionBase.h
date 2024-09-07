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

	virtual void Start()   {};
	virtual void Center()  {};
	virtual void End()     {};

	virtual void Event()   {};
	void Rotate(Math::Vector3 _moveDir, std::shared_ptr<CharacterBase> _target);

	UINT GetFlow() { return m_flow; }
	bool GetIsEnd() { return m_end; }

protected:
	UINT m_flow      = Flow::StartType;
	bool m_ChangeFlg = false;
	bool m_end       = false;
};