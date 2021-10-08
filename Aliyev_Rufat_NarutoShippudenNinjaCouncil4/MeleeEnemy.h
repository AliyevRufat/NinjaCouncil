#pragma once
#include "Enemy.h"
#include "structs.h"
class Avatar;

class MeleeEnemy final : public Enemy
{
public:
	MeleeEnemy(const Point2f& pos);
	void Update(float elapsedSec, const Level& level, Avatar* pAvatar, HUD& hud) override;
	void Draw() const override;
private:
	void Behaviour(const Avatar* pAvatar);
	void ActionStates();
	bool IsEnemyAttackingAvatar(const Rectf& rect) const;
	void ElapsedSecCount(float elapsedSec);
	void Attack(Avatar* pAvatar);
};
