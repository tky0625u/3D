#include "Player_UI_Manager.h"
#include"HP/Player_HP.h"
#include"Stamina/Player_Stamina.h"

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
}
