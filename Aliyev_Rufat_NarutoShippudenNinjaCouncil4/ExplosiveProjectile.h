#pragma once
#include "Texture.h"
#include "SoundEffect.h"

class ExplosiveProjectile
{
public:
	enum class ProjectileState
	{
		normalState = 1,
		nearTarget = 2,
	};
	explicit ExplosiveProjectile(const Point2f& pos, bool isLookingLeft);
	~ExplosiveProjectile();
	ExplosiveProjectile(const ExplosiveProjectile&) = delete;
	ExplosiveProjectile& operator=(const ExplosiveProjectile&) = delete;
	ExplosiveProjectile(ExplosiveProjectile&&) = delete;
	ExplosiveProjectile& operator=(ExplosiveProjectile&&) = delete;
	//--
	void Draw() const;
	void Update(float elapsedSec);
	bool IsOverlapping(const Rectf& rect) const;
	void SetExplosiveState(ProjectileState explosiveState);
	Rectf GetShape() const;
	int GetAnimeExplosionFrame();
	void SetIsExploded(bool isExploded);

private:
	//Methods
	void ElapsedSecCount(float elapsedSec);
	void ExplosiveStates();
	//Datamembers
	const int m_NrOfProjectileFrames;
	const int m_NrOfExplosionFrames;
	const int m_NrOfRows;
	const float m_FrameChangeSpeed;
	//
	ProjectileState m_ProjectileState;
	Texture* m_pProjectileTexture;
	Texture* m_pExplosionTexture;
	Rectf m_ShapeProjectile;
	Rectf m_ShapeExplosion;
	Rectf m_TextClip;
	//
	bool m_IsLookingLeft;
	bool m_IsExploded;

	//
	float m_Speed;
	float m_AnimTime;
	int m_AnimFrame;
	int m_AnimExplosionFrame;
};
