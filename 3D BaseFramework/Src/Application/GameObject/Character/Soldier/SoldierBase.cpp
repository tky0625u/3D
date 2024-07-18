#include "SoldierBase.h"
#include"../Leader/LeaderBase.h"

void SoldierBase::Action()
{
	Math::Vector3 enemyLeaderPos = Math::Vector3::Zero;
	if (m_enemyLeader.expired() == false)
	{
		enemyLeaderPos = m_enemyLeader.lock()->GetPos();
	}

	Math::Vector3 dir = enemyLeaderPos - m_pos;
	dir.Normalize();

	CharacterBase::Rotation(dir);

	m_pos += m_status.SP * m_SpeedCorrection * dir;
}

void SoldierBase::Init()
{
	//陣営によって相手を決める
	if (m_characterType & CharacterType::OnePlayer)m_enemyType = CharacterType::TwoPlayer;
	else{ m_enemyType = CharacterType::TwoPlayer; }
}
