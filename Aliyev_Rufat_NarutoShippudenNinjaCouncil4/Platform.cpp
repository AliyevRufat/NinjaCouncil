#include "pch.h"
#include "Platform.h"
#include "utils.h"
#include <iostream>

Platform::Platform(Type type, const Point2f& bottomLeft)
	: m_Type{ type }
	, m_Texture{ "Resources/Images/Platform.png" }
	, m_Shape{ Rectf{bottomLeft.x , bottomLeft.y , 60,15} }
	, m_PosOffset{}
	, m_Isgoingleft{ false }
	, m_StartPos{ bottomLeft }
	, m_HorSpeed{ 200 }
	, m_Radius{ 0 }
{
}

Platform::Platform(Type type, const Point2f& bottomLeft, float moveRadius)
	:Platform(type, bottomLeft)
{
	m_Radius = { moveRadius };
}

void Platform::Draw() const
{
	m_Texture.Draw(m_Shape);
}

void Platform::Update(float elapsedSec)
{
	if (m_Type == Type::moving)
	{
		Point2f prevPos;
		prevPos.x = m_Shape.left;
		if (!m_Isgoingleft)
		{
			m_Shape.left += m_HorSpeed * elapsedSec;
		}
		else
		{
			m_Shape.left += -m_HorSpeed * elapsedSec;
		}
		if (m_Shape.left - m_StartPos.x >= m_Radius && m_Shape.left - m_StartPos.x > 0)
		{
			m_Isgoingleft = true;
		}
		else if (m_Shape.left - m_StartPos.x <= -m_Radius && m_Shape.left - m_StartPos.x < 0)
		{
			m_Isgoingleft = false;
		}
		m_PosOffset.x = m_Shape.left - prevPos.x;
	}
}

void Platform::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	//WITHOUT RAYCAST
	if (m_Type == Type::normal)
	{
		Rectf smallerActorShape = Rectf{ actorShape.left + actorShape.width / 4.0f, actorShape.bottom ,actorShape.width / 2.0f, actorShape.height };
		if (utils::IsOverlapping(m_Shape, smallerActorShape) && actorShape.bottom + actorShape.height / 5.f > m_Shape.bottom&& actorVelocity.y < 0)
		{
			actorShape.bottom = m_Shape.bottom + m_Shape.height;
			actorVelocity.y = 0;
		}
	}
	else if (m_Type == Type::moving)
	{
		Rectf smallerActorShape = Rectf{ actorShape.left + actorShape.width / 4.0f, actorShape.bottom ,actorShape.width / 2.0f, actorShape.height };
		if (utils::IsOverlapping(m_Shape, smallerActorShape) && actorShape.bottom + actorShape.height / 5.f > m_Shape.bottom&& actorVelocity.y < 0)
		{
			actorShape.bottom = m_Shape.bottom + m_Shape.height;
			actorShape.left += m_PosOffset.x;
			actorVelocity.y = 0;
		}
	}

	//WITH RAYCAST
	//if (!(actorVelocity.y > 0) && utils::IsOverlapping(m_Shape, actorShape))
	//{
	//	std::vector<Point2f> m_Vertice;
	//	m_Vertice.push_back(Point2f{ m_Shape.left, m_Shape.bottom });
	//	m_Vertice.push_back(Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom });
	//	m_Vertice.push_back(Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height });
	//	m_Vertice.push_back(Point2f{ m_Shape.left, m_Shape.bottom + m_Shape.height });
	//	Point2f startPointRay{ actorShape.width / 2.f + actorShape.left ,actorShape.bottom + actorShape.height };
	//	Point2f endPointRay{ actorShape.width / 2.f + actorShape.left ,actorShape.bottom };
	//	utils::HitInfo hitInfo;
	//	if (utils::Raycast(m_Vertice, startPointRay, endPointRay, hitInfo))
	//	{
	//		actorShape.bottom = hitInfo.intersectPoint.y;
	//		actorVelocity.y = 0;
	//	}
	//}
}

bool Platform::IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity) const
{
	if (actorVelocity.y <= 0 && actorShape.bottom == (m_Shape.bottom + m_Shape.height))
	{
		return true;
	}
	else
	{
		return false;
	}
}