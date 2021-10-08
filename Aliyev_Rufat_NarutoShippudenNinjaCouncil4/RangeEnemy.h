#pragma once
#include "Enemy.h"
#include "structs.h"
class Avatar;

class RangeEnemy final : public Enemy
{
public:
	RangeEnemy(const Point2f& pos);
	void Update(float elapsedSec, const Level& level, Avatar* pAvatar, HUD& hud) override;
	void Draw() const override;
private:
	void ActionStates();
};
