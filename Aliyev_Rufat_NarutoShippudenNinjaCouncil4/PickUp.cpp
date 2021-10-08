#include "pch.h"
#include "PickUp.h"
#include "Texture.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include "Level.h"

PickUp::PickUp(const Point2f& pos, Type type)
	:m_Texture{ "Resources/Images/PickUps.png" }
	, m_Type{ type }
	, m_NrOfHealthAndWeaponFrames{ 4 }
	, m_NrOfRows{ 3 }
	, m_NrOfCoinFrames{ 13 }
	, m_AnimFrame{ 0 }
	, m_AnimTime{ 0 }
	, m_FrameOffset{ 83 }
	, m_FrameChangeSpeed{ 0.13f }
	, m_Velocity{ Vector2f{0.0f,0.0f} }
	, m_Acceleration{ Vector2f{0,-981.0f} }
{
	if (m_Type == Type::weapon)
	{
		m_TextClip = { Rectf{ 0, m_Texture.GetHeight() / m_NrOfRows ,m_Texture.GetWidth() - m_FrameOffset,m_Texture.GetHeight() / m_NrOfRows} };
		m_Shape = Rectf{ pos.x, pos.y, m_TextClip.width / m_NrOfCoinFrames, m_Texture.GetHeight() / m_NrOfRows };
	}
	else if (m_Type == Type::health)
	{
		m_TextClip = { Rectf{ 0.f,m_Texture.GetHeight() / m_NrOfRows * 2,m_Texture.GetWidth() - m_FrameOffset,m_Texture.GetHeight() / m_NrOfRows } };
		m_Shape = Rectf{ pos.x, pos.y, m_TextClip.width / m_NrOfHealthAndWeaponFrames, m_Texture.GetHeight() / m_NrOfRows };
	}
	else if (m_Type == Type::coin)
	{
		m_TextClip = { Rectf{ 0.f,0.f,m_Texture.GetWidth(),m_Texture.GetHeight() / m_NrOfRows } };
		m_Shape = Rectf{ pos.x, pos.y, m_Texture.GetWidth() / m_NrOfCoinFrames, m_Texture.GetHeight() / m_NrOfRows };
	}
}

void PickUp::Update(float elapsedSec, const Level& level)
{
	ElapsedSecCount(elapsedSec);
	//
	if (!level.IsOnGround(m_Shape, m_Velocity))
	{
		m_Shape.left += m_Velocity.x * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		m_Velocity += m_Acceleration * elapsedSec;
	}
}

void PickUp::Draw() const
{
	Rectf srcRect, dstRect;
	if (m_Type == Type::coin)
	{
		int currentColumn = m_AnimFrame % m_NrOfCoinFrames;
		srcRect = m_TextClip;
		srcRect.width = m_Texture.GetWidth() / m_NrOfCoinFrames;
		srcRect.left = srcRect.width * currentColumn;
		dstRect = m_Shape;
		dstRect.width = srcRect.width;
		dstRect.height = srcRect.height;
		m_Texture.Draw(dstRect, srcRect);
	}
	else
	{
		int currentColumn = m_AnimFrame % m_NrOfHealthAndWeaponFrames;
		srcRect = m_TextClip;
		srcRect.width = m_TextClip.width / m_NrOfHealthAndWeaponFrames;
		srcRect.left = srcRect.width * currentColumn;
		dstRect = m_Shape;
		dstRect.width = srcRect.width;
		dstRect.height = srcRect.height;
		m_Texture.Draw(dstRect, srcRect);
	}
}

bool PickUp::IsOverlapping(const Rectf& rect) const
{
	if (utils::IsOverlapping(rect, m_Shape))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PickUp::ElapsedSecCount(float elapsedSec)
{
	//health and weapon
	m_AnimTime += elapsedSec;
	if (m_AnimTime >= m_FrameChangeSpeed)
	{
		m_AnimFrame++;
		if (m_AnimFrame == m_NrOfHealthAndWeaponFrames)
		{
			m_AnimFrame = 0;
		}
		m_AnimTime = 0;
	}
	// coin
	if (m_AnimTime >= m_FrameChangeSpeed)
	{
		m_AnimFrame++;
		if (m_AnimFrame == m_NrOfCoinFrames)
		{
			m_AnimFrame = 0;
		}
		m_AnimTime = 0;
	}
}

PickUp::Type PickUp::GetType() const
{
	return m_Type;
}

void PickUp::SetVelocity(Vector2f& velocity)
{
	m_Velocity = velocity;
}