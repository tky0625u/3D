#pragma once

class Laser :public KdGameObject
{
public:
	Laser() {};
	~Laser()override {};

	enum Flow
	{
		StartType,
		CenterType,
		EndType
	};

	void Start();
	void Center();
	void End();

	void Update()     override;
	void PostUpdate() override;
	void Init()       override;

	void SetTargetMat(Math::Matrix _targetMat) { m_targetMat = _targetMat; }
	void SetAliveTime(int _aliveTime) { m_AliveTime = _aliveTime; }
	void SetDamage(int _damage) { m_damage = _damage; }

private:
	Math::Matrix                 m_targetMat;
	Flow                         m_flow  = Flow::StartType;
	bool                         m_effectFlg = false;
	int                          m_AliveTime = 0;
	int                          m_damage = 0;
	Effekseer::Handle            m_effectHandle;
	Effekseer::Handle            m_laserHandle;
};