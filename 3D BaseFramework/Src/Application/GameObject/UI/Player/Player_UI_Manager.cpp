#include "Player_UI_Manager.h"
#include"HP/Player_HP.h"
#include"Stamina/Player_Stamina.h"
#include"LockON/LockON.h"
#include"Floor/Floor.h"

void Player_UI_Manager::Update()
{
	for (auto& ui : m_UIList)
	{
		ui->Update();
	}
}

void Player_UI_Manager::DrawSprite()
{
	for (auto& ui : m_UIList)
	{
		ui->DrawSprite();
	}
}

void Player_UI_Manager::Init()
{
	std::shared_ptr<Player_HP> _hp = std::make_shared<Player_HP>();
	if (m_player.expired() == false)_hp->SetTraget(m_player.lock());
	_hp->Init();
	m_UIList.push_back(_hp);

	std::shared_ptr<Player_Stamina> _stamina = std::make_shared<Player_Stamina>();
	if (m_player.expired() == false)_stamina->SetTraget(m_player.lock());
	_stamina->Init();
	m_UIList.push_back(_stamina);

	std::shared_ptr<LockON> _lock = std::make_shared<LockON>();
	if (m_player.expired() == false)_lock->SetTraget(m_player.lock());
	_lock->Init();
	m_UIList.push_back(_lock);

	std::shared_ptr<Floor> _floor = std::make_shared<Floor>();
	_floor->Init();
	m_UIList.push_back(_floor);
}
