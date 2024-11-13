#pragma once

class MagicPolygon;

class MagicPolygon_State
{
public:
	enum Flow
	{
		StartType,
		CenterType,
		EndType
	};

	MagicPolygon_State()  {};
	~MagicPolygon_State() {};

	virtual void Start()  {};
	virtual void Center() {};
	virtual void End()    {};

	void Update()
	{
		switch (m_flow)
		{
		case MagicPolygon_State::StartType:
			Start();	  
			break;		  
		case MagicPolygon_State::CenterType:
			Center();	 
			break;		 
		case MagicPolygon_State::EndType:
			End();
			break;
		default:
			break;
		}
	}

	void Normal();
	void Next();
	void Teleport();

	void SetTarget(std::shared_ptr<MagicPolygon> _target) { m_target = _target; }

	const Effekseer::Handle& GetHandle()const { return m_handle; }

protected:
	std::weak_ptr<MagicPolygon> m_target;
	Flow                        m_flow = Flow::StartType;
	Effekseer::Handle m_handle;
};