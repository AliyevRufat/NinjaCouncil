#pragma once
#include "Texture.h"
#include "structs.h"
#include "Level.h"

class PickUp
{
public:
	enum class Type
	{
		weapon = 1,
		health = 2,
		coin = 3
	};
	explicit PickUp(const Point2f& pos, Type type);
	void Update(float elapsedSec, const Level& level);
	void Draw() const;
	bool IsOverlapping(const Rectf& rect) const;
	Type GetType() const;
	void SetVelocity(Vector2f& velocity);

private:
	const Type m_Type;
	const Texture m_Texture;
	Rectf m_TextClip;
	Rectf m_Shape;
	const int m_FrameOffset;
	const int m_NrOfRows;
	const int m_NrOfHealthAndWeaponFrames;
	const int m_NrOfCoinFrames;
	int m_AnimFrame;
	float m_AnimTime;
	const float m_FrameChangeSpeed;
	Vector2f m_Velocity;
	const Vector2f m_Acceleration;
	//HelperMethods
	void ElapsedSecCount(float elapsedSec);
};
