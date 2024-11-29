#pragma once
#include"../../UIBase.h"

class GameStateUI :public UIBase,public std::enable_shared_from_this<GameStateUI>
{
public:
	enum Flow
	{
		EnterType,
		UpdateType,
		ExitType
	};

	GameStateUI() {};
	~GameStateUI()override {};

	void Update()override;

	void SetChangeAlpha(float _change) { m_alphaChange = _change; }
	void SetState(bool _IsClear)
	{
		if (_IsClear)
		{
			m_state = std::make_shared<Clear>();
			m_flow = Flow::EnterType;
		}
		else
		{
			m_state = std::make_shared<GameOver>();
			m_flow = Flow::EnterType;
		}
	}

	const float& GetAlphaChange()const { return m_alphaChange; }

private:
	float m_alphaChange = 0.05f;

private:
	class StateBase
	{
	public:
		StateBase()  {};
		virtual ~StateBase() {};
		
		void StateUpdate(std::shared_ptr<GameStateUI> owner)
		{
			switch (owner->m_flow)
			{
			case Flow::EnterType:
				Enter(owner);
				break;
			case Flow::UpdateType:
				Update(owner);
				break;
			case Flow::ExitType:
				Exit(owner);
				break;
			default:
				break;
			}
		}

		virtual void Enter (std::shared_ptr<GameStateUI> owner) {};
		virtual void Update(std::shared_ptr<GameStateUI> owner) {};
		virtual void Exit  (std::shared_ptr<GameStateUI> owner) {};

	protected:
		float m_alpha = 0.0f;
	};

	class GameOver :public StateBase
	{
	public:
		GameOver() {};
		~GameOver()override {};

		void Enter (std::shared_ptr<GameStateUI> owner)override;
		void Update(std::shared_ptr<GameStateUI> owner)override;
		void Exit  (std::shared_ptr<GameStateUI> owner)override;
	};

	class Clear :public StateBase
	{
	public:
		Clear() {};
		~Clear()override {};

		void Enter (std::shared_ptr<GameStateUI> owner)override;
		void Update(std::shared_ptr<GameStateUI> owner)override;
		void Exit  (std::shared_ptr<GameStateUI> owner)override;
	};

	std::shared_ptr<StateBase> m_state     = nullptr;
	std::shared_ptr<StateBase> m_NextState = nullptr;
	UINT                       m_flow      = Flow::UpdateType;

};