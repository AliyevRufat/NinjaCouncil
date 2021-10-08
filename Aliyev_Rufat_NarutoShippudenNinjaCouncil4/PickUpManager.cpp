#include "pch.h"
#include "PickUpManager.h"
#include <iostream>
#include "Avatar.h"

PickUpManager::PickUpManager()
{
}

PickUpManager::~PickUpManager()
{
	for (size_t index = 0; index < m_pItems.size(); index++)
	{
		delete m_pItems[index];
	}
}

PickUp* PickUpManager::AddItem(const Point2f& center, PickUp::Type type)
{
	m_pItems.push_back(new PickUp(center, type));
	return m_pItems[m_pItems.size() - 1];
}

void PickUpManager::Update(float elapsedSec, Avatar* pAvatar, const Level& level, HUD& hud)
{
	for (size_t index = 0; index < m_pItems.size(); index++)
	{
		m_pItems[index]->Update(elapsedSec, level);
	}
	HitItem(pAvatar, hud);
}

void PickUpManager::Draw() const
{
	for (size_t index = 0; index < m_pItems.size(); index++)
	{
		m_pItems[index]->Draw();
	}
}

void PickUpManager::HitItem(Avatar* pAvatar, HUD& hud)
{
	for (size_t index = 0; index < m_pItems.size(); ++index)
	{
		if (m_pItems[index]->IsOverlapping(pAvatar->GetShape()) && m_pItems[index])
		{
			if (m_pItems[index]->GetType() == PickUp::Type::weapon)
			{
				pAvatar->IncreaseThrowables();
			}
			else if (m_pItems[index]->GetType() == PickUp::Type::health)
			{
				pAvatar->IncreaseAmountOfPickUps();
			}
			else if (m_pItems[index]->GetType() == PickUp::Type::coin)
			{
				const int scoreIncrease = 5;
				hud.IncreaseScore(scoreIncrease);
			}
			delete m_pItems[index];
			m_pItems[index] = m_pItems[m_pItems.size() - 1];
			m_pItems.pop_back();
		}
	}
}

void PickUpManager::SetVelocity(Vector2f& velocity)
{
	m_pItems[m_pItems.size() - 1]->SetVelocity(velocity);
}

void PickUpManager::DeleteAllPickUps()
{
	for (size_t index = 0; index < m_pItems.size(); ++index)
	{
		delete m_pItems[index];
	}
	m_pItems.clear();
}