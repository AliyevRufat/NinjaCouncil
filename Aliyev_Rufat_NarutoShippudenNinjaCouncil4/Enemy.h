#pragma once
#include "Level.h"
#include "Character.h"
#include "Throwable.h"
#include "ExplosiveProjectile.h"
#include "HUD.h"

#include <vector>
class Avatar;

class Enemy : public Character
{
public:
	Enemy(const Point2f& pos);

	virtual void Update(float elapsedSec, const Level& level, Avatar* pAvatar, HUD& hud);
	virtual void Draw() const = 0;
	void SetCanSpawnAPickUp(bool canSpawn);
	bool GetCanSpawnAPickUp() const;
	bool IsDead();

protected:
	//Methods
	void Behaviour(const Avatar* pAvatar);
	void ThrowingWeapon(const Avatar* pAvatar);
	void IsTakingDamage(Avatar* pAvatar);
	bool IsAvatarOverlappingThrowable(const Rectf& rect);
	void CollisionChecks(float elapsedSec, Avatar* pAvatar);

	//Datamembers
	Throwable m_Throwable;
	//
	bool m_IsOnGround;
	bool m_Moving;
	bool m_CanSpawnAPickUp;
	bool m_HasAttacked;
	bool m_IsPushed;
	//
	float m_Delay;
	float m_Radius;
	float m_Offset;
	//
	bool m_GoBack;
	bool m_DoOnce;
	Point2f m_StartPos;
};
