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

	void SetTarget(std::shared_ptr<CharacterBase> target) { m_target = target; }

	UINT GetFlow() { return m_flow; }
	bool GetIsEnd() { return m_end; }

protected:
	std::weak_ptr<CharacterBase> m_target;
	UINT m_flow      = Flow::StartType;
	bool m_ChangeFlg = false;
	bool m_end       = false;
};