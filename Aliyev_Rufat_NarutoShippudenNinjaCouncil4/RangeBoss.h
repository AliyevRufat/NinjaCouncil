#pragma once
#include "Enemy.h"
#include "structs.h"
#include "ExplosiveProjectile.h"
#include "SoundEffect.h"
#include "Texture.h"

class Avatar;

class RangeBoss final : public Enemy
{
public:
	explicit RangeBoss(const Point2f& pos);
	RangeBoss(const RangeBoss& other) = delete;
	RangeBoss& operator=(const RangeBoss& other) = delete;
	RangeBoss(RangeBoss&& other) = delete;
	RangeBoss& operator=(RangeBoss&& other) = delete;
	~RangeBoss();
	//
	void Update(float elapsedSec, const Level& level, Avatar* pAvatar, HUD& hud) override;
	void Draw() const override;
private:
	void DrawExplosiveProjectiles() const;
	void UpdateExplosiveProjectiles(float elapsedSec, Avatar* pAvatar);
	void Teleportation();
	void ActionStates();
	void ElapsedSecCount(float elapsedSec);
	bool IsDeathExplosionHittingAvatar(const Rectf& rect) const;

	bool m_HasTeleported;
	int m_AnimTeleportFrame;
	const int m_NrOfTeleportFrames;
	const int m_NrOfDeathFrames;
	//
	std::vector <ExplosiveProjectile*> m_ExplosiveProjectiles;
	//
	const int m_NrOfRows;
	int m_AnimDeathExplosionFrame;
	const int m_NrOfDeathExplosionFrames;
	Texture* m_pDeathExplosionTexture;
	Rectf m_ShapeDeathExplosion;
	Rectf m_TextClip;

	//
};
