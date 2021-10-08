#pragma once
#include "Enemy.h"
#include "structs.h"
class Avatar;

class MeleeBoss final : public Enemy
{
public:
	MeleeBoss(const Point2f& pos);
	void Update(float elapsedSec, const Level& level, Avatar* pAvatar, HUD& hud) override;
	void Draw() const override;
private:
	bool IsEnemyAttackingAvatar(const Rectf& rect) const;
	void SpecialAttack(float elapsedSec);
	void ActionStates();
	void ElapsedSecCount(float elapsedSec);

	const int m_NrOfSpecialAttackFrames;
};
