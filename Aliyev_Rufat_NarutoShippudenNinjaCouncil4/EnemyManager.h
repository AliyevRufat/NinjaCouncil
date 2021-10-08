#pragma once
#include "PickUp.h"
#include <vector>
#include "structs.h"
#include "PickUpManager.h"
#include "MeleeBoss.h"
#include "RangeBoss.h"
#include "RangeEnemy.h"
#include "MeleeEnemy.h"

class EnemyManager
{
public:
	enum class TypeEnemy
	{
		rangeBoss = 1,
		meleeBoss = 2,
		rangeEnemy = 3,
		meleeEnemy = 4
	};
	explicit EnemyManager();
	~EnemyManager();
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;
	EnemyManager(EnemyManager&&) = delete;
	EnemyManager& operator=(EnemyManager&&) = delete;

	Enemy* AddItem(const Point2f& center, TypeEnemy typeEnemy);
	void Update(float elapsedSec, const Level& level, Avatar* pAvatar, PickUpManager& pickUpManager, HUD& hud);
	void Draw() const;
	void SpawnPickUp(PickUpManager& pickUpManager);
	void DeleteAllEnemies();
	bool AreEnemiesDead() const;

private:
	//DATAMEMBERS
	std::vector<Enemy*> m_pEnemies;
	float m_Counter;
	const float m_MaxTimer;
	//METHODS
	void DeleteOneEnemy(float elapsedSec);
};