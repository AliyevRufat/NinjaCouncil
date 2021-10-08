#include "pch.h"
#include "ExplosiveProjectile.h"
#include "utils.h"
#include <iostream>
#include "Sound.h"

ExplosiveProjectile::ExplosiveProjectile(const Point2f& pos, bool isLookingLeft)
	:m_pProjectileTexture{ new Texture{"Resources/Images/ExplosiveProjectile.png"} }
	, m_pExplosionTexture{ new Texture{"Resources/Images/Explosion.png"} }
	, m_ShapeProjectile{ Rectf{pos.x,pos.y ,m_pProjectileTexture->GetWidth() / m_NrOfProjectileFrames / 2 , m_pProjectileTexture->GetHeight() / m_NrOfRows / 2} }
	, m_ShapeExplosion{ Rectf{0,0,m_pExplosionTexture->GetWidth() / m_NrOfExplosionFrames * 2 , m_pExplosionTexture->GetHeight() * 2 } }
	, m_IsLookingLeft{ isLookingLeft }
	, m_IsExploded{ false }
	, m_NrOfProjectileFrames{ 6 }
	, m_NrOfExplosionFrames{ 7 }
	, m_Speed{ 400 }
	, m_AnimFrame{ 0 }
	, m_AnimExplosionFrame{ 0 }
	, m_AnimTime{ 0 }
	, m_FrameChangeSpeed{ 0.1f }
	, m_NrOfRows{ 2 }
	, m_ProjectileState{ ProjectileState::normalState }
{
	ExplosiveStates();
}

ExplosiveProjectile::~ExplosiveProjectile()
{
	delete m_pProjectileTexture;
	delete m_pExplosionTexture;
}

void ExplosiveProjectile::Draw() const
{
	Rectf srcRect, dstRect;
	int currentColumn = 0;
	if (m_IsExploded)
	{
		currentColumn = m_AnimExplosionFrame % m_NrOfExplosionFrames;
		srcRect = m_TextClip;
		srcRect.width = m_pExplosionTexture->GetWidth() / m_NrOfExplosionFrames;
		srcRect.left = srcRect.width * currentColumn;
		dstRect = m_ShapeExplosion;

		if (m_IsLookingLeft)
		{
			const float doublePosShape = m_ShapeExplosion.left * 2.f;
			dstRect.left = dstRect.left - dstRect.width;
			glPushMatrix();
			glTranslatef(doublePosShape, 0, 0);
			glScalef(-1, 1, 0);
			m_pExplosionTexture->Draw(dstRect, srcRect);
			glPopMatrix();
		}
		else
		{
			glPushMatrix();
			glScalef(1, 1, 0);
			m_pExplosionTexture->Draw(dstRect, srcRect);
			glPopMatrix();
		}
	}
	else
	{
		currentColumn = m_AnimFrame % m_NrOfProjectileFrames;
		srcRect = m_TextClip;
		srcRect.width = m_pProjectileTexture->GetWidth() / m_NrOfProjectileFrames;
		srcRect.left = srcRect.width * currentColumn;
		dstRect = m_ShapeProjectile;

		if (m_IsLookingLeft)
		{
			const float doublePosShape = m_ShapeProjectile.left * 2.f;
			dstRect.left = dstRect.left - dstRect.width;
			glPushMatrix();
			glTranslatef(doublePosShape, 0, 0);
			glScalef(-1, 1, 0);
			m_pProjectileTexture->Draw(dstRect, srcRect);
			glPopMatrix();
		}
		else
		{
			glPushMatrix();
			glScalef(1, 1, 0);
			m_pProjectileTexture->Draw(dstRect, srcRect);
			glPopMatrix();
		}
	}
}

void ExplosiveProjectile::Update(float elapsedSec)
{
	ExplosiveStates();
	if (!m_IsExploded)
	{
		if (m_IsLookingLeft)
		{
			m_ShapeProjectile.left -= elapsedSec * m_Speed;
		}
		else
		{
			m_ShapeProjectile.left += elapsedSec * m_Speed;
		}
	}
	ElapsedSecCount(elapsedSec);
	m_ShapeExplosion.left = m_ShapeProjectile.left;
	m_ShapeExplosion.bottom = m_ShapeProjectile.bottom;
}

void ExplosiveProjectile::ElapsedSecCount(float elapsedSec)
{
	m_AnimTime += elapsedSec;

	if (m_IsExploded)
	{
		if (m_AnimTime >= m_FrameChangeSpeed)
		{
			m_AnimExplosionFrame++;

			if (m_AnimExplosionFrame == m_NrOfExplosionFrames)
			{
				m_AnimExplosionFrame = 0;
			}
			m_AnimTime = 0;
		}
	}
	else
	{
		if (m_AnimTime >= m_FrameChangeSpeed)
		{
			m_AnimFrame++;

			if (m_AnimFrame == m_NrOfProjectileFrames)
			{
				m_AnimFrame = 0;
			}
			m_AnimTime = 0;
		}
	}
}

bool ExplosiveProjectile::IsOverlapping(const Rectf& rect) const
{
	if (utils::IsOverlapping(rect, m_ShapeProjectile) && !m_IsExploded)
	{
		Sound::GetInstance()->PlaySoundEffect(15);
		return true;
	}
	else
	{
		return false;
	}
}

void ExplosiveProjectile::SetExplosiveState(ProjectileState projectileState)
{
	m_ProjectileState = projectileState;
}

Rectf ExplosiveProjectile::GetShape() const
{
	return m_ShapeProjectile;
}

void ExplosiveProjectile::ExplosiveStates()
{
	if (m_IsExploded)
	{
		m_TextClip = { Rectf{ 0, 0,m_pExplosionTexture->GetWidth(),m_pExplosionTexture->GetHeight()} };
	}
	else
	{
		if (m_ProjectileState == ProjectileState::normalState)
		{
			m_TextClip = { Rectf{ 0, 0,m_pProjectileTexture->GetWidth(),m_pProjectileTexture->GetHeight() / m_NrOfRows} };
		}
		else if (m_ProjectileState == ProjectileState::nearTarget)
		{
			m_TextClip = { Rectf{ 0.f,m_pProjectileTexture->GetWidth() / m_NrOfRows ,m_pProjectileTexture->GetWidth(),m_pProjectileTexture->GetHeight() / m_NrOfRows } };
		}
	}
}

int ExplosiveProjectile::GetAnimeExplosionFrame()
{
	return m_AnimExplosionFrame;
}

void ExplosiveProjectile::SetIsExploded(bool isExploded)
{
	m_IsExploded = isExploded;
}