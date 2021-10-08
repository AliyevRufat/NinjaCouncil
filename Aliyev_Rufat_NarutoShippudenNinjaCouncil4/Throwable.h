#pragma once
#include "Texture.h"

class Throwable
{
public:
	Throwable(const int nrOfThrowables);
	virtual ~Throwable();
	Throwable(const Throwable&) = delete;
	Throwable& operator=(const Throwable&) = delete;
	Throwable(Throwable&&) = delete;
	Throwable& operator=(Throwable&&) = delete;
	//--
	void Draw() const;
	void Update(float elapsedSec);
	void SetIsThrowing(bool isThrowing);
	bool GetIsThrowing() const;
	void SetPosition(const Point2f& pos);
	void DecreaseThrowables();
	void IncreaseThrowables();
	int GetAmountOfThrowables() const;
	void SetAmountOfThrowables(int amountOfThrowables);
	void IsLookingLeft(bool isLookingLeft);
	void IsNotRange(const Rectf& avatar);
	Rectf GetShape() const;
	bool IsOverlapping(const Rectf& rect) const;
private:
	bool m_IsLookingLeft;
	Texture* m_pThrowableTexture;
	Rectf m_Shape;
	bool m_IsThrowing;
	int m_NrOfThrowables;
	const float m_Offset;
	const float m_Speed;
};
