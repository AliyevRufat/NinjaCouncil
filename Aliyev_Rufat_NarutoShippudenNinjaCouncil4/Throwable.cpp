#include "pch.h"
#include <iostream>
#include "Throwable.h"
#include "utils.h"

Throwable::Throwable(const int nrOfThrowables)
	:m_Shape{ 0 , 0 , m_pThrowableTexture->GetWidth() , m_pThrowableTexture->GetHeight() }
	, m_pThrowableTexture{ new Texture {"Resources/Images/Weapon.png" } }
	, m_IsThrowing{ false }
	, m_NrOfThrowables{ nrOfThrowables }
	, m_Offset{ 30 }
	, m_Speed{ 500 }
	, m_IsLookingLeft{ false }
{
}

Throwable::~Throwable()
{
	delete m_pThrowableTexture;
}

void Throwable::SetPosition(const Point2f& pos)
{
	m_Shape.left = pos.x + m_Offset / 3.f;
	m_Shape.bottom = pos.y + m_Offset;
}

void Throwable::IsLookingLeft(bool isLookingLeft)
{
	m_IsLookingLeft = isLookingLeft;
}

void Throwable::Draw() const
{
	if (m_IsThrowing && m_NrOfThrowables > -1)
	{
		if (m_IsLookingLeft)
		{
			glPushMatrix();
			glTranslatef(m_Shape.left * 2.f + m_Shape.width, 0, 0);
			glScalef(-1, 1, 0);
			m_pThrowableTexture->Draw(m_Shape);
			glPopMatrix();
		}
		else
		{
			glPushMatrix();
			glScalef(1, 1, 0);
			m_pThrowableTexture->Draw(m_Shape);
			glPopMatrix();
		}
	}
}

void Throwable::Update(float elapsedSec)
{
	if (m_IsThrowing && m_NrOfThrowables > -1)
	{
		if (m_IsLookingLeft)
		{
			m_Shape.left -= elapsedSec * m_Speed;
		}
		else
		{
			m_Shape.left += elapsedSec * m_Speed;
		}
	}
	else if (!m_IsThrowing)
	{
		m_Shape.left = 0;
		m_Shape.bottom = 0;
	}
}

void Throwable::SetIsThrowing(bool isThrowing)
{
	m_IsThrowing = isThrowing;
}

void Throwable::DecreaseThrowables()
{
	m_NrOfThrowables--;
}

void Throwable::IncreaseThrowables()
{
	m_NrOfThrowables++;
}

int Throwable::GetAmountOfThrowables() const
{
	return m_NrOfThrowables;
}

void Throwable::SetAmountOfThrowables(int amountOfThrowables)
{
	m_NrOfThrowables = amountOfThrowables;
}

void Throwable::IsNotRange(const Rectf& avatar)
{
	if (m_Shape.left > avatar.left + avatar.width * 10 || m_Shape.left < avatar.left - avatar.width * 10)
	{
		m_IsThrowing = false;
	}
}

Rectf Throwable::GetShape() const
{
	return m_Shape;
}

bool Throwable::GetIsThrowing() const
{
	if (m_IsThrowing && m_NrOfThrowables > -1)
	{
		return true;
	}
	return false;
}

bool Throwable::IsOverlapping(const Rectf& rect) const
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