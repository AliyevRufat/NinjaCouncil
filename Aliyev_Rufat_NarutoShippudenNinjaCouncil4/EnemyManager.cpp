#include "pch.h"
#include "EnemyManager.h"
#include <iostream>
#include "PickUp.h"
#include "Character.h"
#include <algorithm>

EnemyManager::EnemyManager()
	: m_Counter{ 0 }
	, m_MaxTimer{ 5 }
{
}

EnemyManager::~EnemyManager()
{
	for (size_t index = 0; index < m_pEnemies.size(); index++)
	{
		delete m_pEnemies[index];
	}
}

Enemy* EnemyManager::AddItem(const Point2f& center, TypeEnemy typeEnemy)
{
	if (typeEnemy == TypeEnemy::meleeBoss)
	{
		m_pEnemies.push_back(new MeleeBoss(Point2f(center)));
		return m_pEnemies[m_pEnemies.size() - 1];
	}
	else if (typeEnemy == TypeEnemy::rangeBoss)
	{
		m_pEnemies.push_back(new RangeBoss(Point2f(center)));
		return m_pEnemies[m_pEnemies.size() - 1];
	}
	else if (typeEnemy == TypeEnemy::rangeEnemy)
	{
		m_pEnemies.push_back(new RangeEnemy(Point2f(center)));
		return m_pEnemies[m_pEnemies.size() - 1];
	}
	else if (typeEnemy == TypeEnemy::meleeEnemy)
	{
		m_pEnemies.push_back(new MeleeEnemy(Point2f(center)));
		return m_pEnemies[m_pEnemies.size() - 1];
	}
	else
	{
		return 0;
	}
}

void EnemyManager::Update(float elapsedSec, const Level& level, Avatar* pAvatar, PickUpManager& pickUpManager, HUD& hud)
{
	for (size_t index = 0; index < m_pEnemies.size(); index++)
	{
		m_pEnemies[index]->Update(elapsedSec, level, pAvatar, hud);
	}
	SpawnPickUp(pickUpManager);
	DeleteOneEnemy(elapsedSec);
	AreEnemiesDead();
}

void EnemyManager::Draw() const
{
	for (size_t index = 0; index < m_pEnemies.size(); index++)
	{
		m_pEnemies[index]->Draw();
	}
}

void EnemyManager::SpawnPickUp(PickUpManager& pickUpManager)
{
	const int dropHeightVelocity = 250;
	for (size_t index = 0; index < m_pEnemies.size(); index++)
	{
		if (m_pEnemies[index]->IsDead() && !m_pEnemies[index]->GetCanSpawnAPickUp())
		{
			m_pEnemies[index]->SetCanSpawnAPickUp(true);
			int randNr = rand() % 2;
			int randDropLength = rand() % 100 + 80;
			if (randNr == 0)
			{
				pickUpManager.AddItem(Point2f{ m_pEnemies[index]->GetShape().left , m_pEnemies[index]->GetShape().bottom }, PickUp::Type::health);
			}
			else
			{
				pickUpManager.AddItem(Point2f{ m_pEnemies[index]->GetShape().left , m_pEnemies[index]->GetShape().bottom }, PickUp::Type::weapon);
			}
			Vector2f velocity{ float(randDropLength), dropHeightVelocity };
			pickUpManager.SetVelocity(velocity);
		}
	}
}

void EnemyManager::DeleteOneEnemy(float elapsedSec)
{
	for (size_t i = 0; i < m_pEnemies.size(); i++)
	{
		if (m_pEnemies[i]->IsDead())
		{
			m_Counter += elapsedSec;

			if (m_Counter >= m_MaxTimer)
			{
				delete m_pEnemies[i];
				m_pEnemies[i] = m_pEnemies[m_pEnemies.size() - 1];
				m_pEnemies.pop_back();
				m_Counter = 0;
			}
		}
	}
}

void EnemyManager::DeleteAllEnemies()
{
	for (size_t index = 0; index < m_pEnemies.size(); ++index)
	{
		delete m_pEnemies[index];
	}
	m_pEnemies.clear();
}

bool EnemyManager::AreEnemiesDead() const
{
	if (std::all_of(m_pEnemies.begin(), m_pEnemies.end(), [](Enemy* pEnemy) {return pEnemy->IsDead(); }))
	{
		return true;
	}
	else
	{
		return 0;
	}
}