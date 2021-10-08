#pragma once
#include "Vector2f.h"
#include "Texture.h"

class Platform
{
public:

	enum class Type
	{
		normal = 0,
		moving = 1,
		rotating = 2
	};

	Platform(Type type, const Point2f& bottomLeft, float moveRadius);
	Platform(Type type, const Point2f& bottomLeft);
	void Draw() const;
	void Update(float elapsedSec);
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	bool IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity) const;
protected:
	Texture m_Texture;
	Rectf m_Shape;
	Type m_Type;
	//
	Point2f m_PosOffset;
	const Point2f m_StartPos;
	float m_Radius;
	bool m_Isgoingleft;
	const float m_HorSpeed;
};
