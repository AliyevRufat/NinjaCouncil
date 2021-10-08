#include "pch.h"
#include "Camera.h"
#include "utils.h"
#include <iostream>
#include "Avatar.h"

Camera::Camera(float width, float height)
	: m_Width{ width / 2 }
	, m_Height{ height / 2 }
	, m_LevelBoundaries{ Rectf{ 0, 0, 0, 0 } }
	, m_MiddleOfMap{ 5024 }
	, m_FreeMovingAreaWidth{ 30 }
	, m_SubZone{ Rectf{0,0,m_FreeMovingAreaWidth,100} }
	, m_CameraPos{ Point2f{0,0} }
	, m_HasMovedToNewLevel{ false }
{
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

void Camera::TransformForeGround(const Rectf& target) const
{
	Point2f cameraPos = Point2f{ (m_SubZone.left + m_SubZone.width / 2) - m_Width / 2, Track(target).y };
	Clamp(cameraPos);
	glTranslatef(-cameraPos.x, -cameraPos.y, 0);
}

void Camera::TransformBackGround(const Rectf& target) const
{
	float cameraXPos = (m_SubZone.left + m_SubZone.width / 2) - m_Width / 2;
	float backgroundSpeed{ 0.4f };
	Point2f cameraPos = Point2f{ cameraXPos, Track(target).y };
	Clamp(cameraPos);
	glTranslatef(-cameraPos.x * backgroundSpeed, -cameraPos.y * backgroundSpeed, 0);
}

void Camera::SubZoneManager(const Rectf& target)
{
	float centerTargetX = target.left + (target.width / 2);
	if (centerTargetX >= m_SubZone.left + m_SubZone.width)
	{
		m_SubZone.left = centerTargetX - m_SubZone.width;
	}
	else if (centerTargetX <= m_SubZone.left)
	{
		m_SubZone.left = centerTargetX;
	}
}

Point2f Camera::Track(const Rectf& target) const
{
	return m_CameraPos;
}

void Camera::Update(float elapsedSec, Avatar* pAvatar)
{
	if (m_HasMovedToNewLevel && pAvatar->GetShape().left >= m_MiddleOfMap)
	{
		const int offset = 100;
		const int transitionVelocityCamera = 300;
		int transitionVelocityAvatar = 50;

		pAvatar->SetVelocity(0, elapsedSec);
		pAvatar->SetIsMovingToNewLevel(true);
		if (m_LevelBoundaries.width < m_MiddleOfMap + m_Width)
		{
			SetLevelBoundaries(Rectf{ 0 ,0,m_LevelBoundaries.width + transitionVelocityCamera * elapsedSec ,m_LevelBoundaries.height });
			m_SubZone.left += transitionVelocityCamera * elapsedSec;
		}
		else
		{
			pAvatar->SetVelocity(transitionVelocityAvatar, elapsedSec);
			if (pAvatar->GetShape().left > m_MiddleOfMap + offset)
			{
				SetLevelBoundaries(Rectf{ m_MiddleOfMap,0,m_MiddleOfMap * 2,m_LevelBoundaries.height });
				m_HasMovedToNewLevel = false;
				pAvatar->SetIsMovingToNewLevel(false);
			}
		}
	}
	else
	{
		SubZoneManager(pAvatar->GetShape());
	}
	const float halfCameraHeight = m_Height / 2;
	const float halfAvatarHeight = pAvatar->GetShape().height / 2;
	m_CameraPos.y = pAvatar->GetShape().bottom - (halfCameraHeight - halfAvatarHeight);
}

void Camera::SetHasMovedToNewLevel(bool hasMovedToNewLevel)
{
	m_HasMovedToNewLevel = hasMovedToNewLevel;
}

void Camera::Clamp(Point2f& bottomLeftPos) const
{
	//Checks Boundaries
	if (m_LevelBoundaries.left > bottomLeftPos.x)
	{
		bottomLeftPos.x = m_LevelBoundaries.left;
	}
	else if (m_LevelBoundaries.width < (bottomLeftPos.x + m_Width))
	{
		bottomLeftPos.x = m_LevelBoundaries.width - m_Width;
	}
	if (m_LevelBoundaries.bottom > bottomLeftPos.y)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom;
	}
	else if (m_LevelBoundaries.height < (bottomLeftPos.y + m_Height))
	{
		bottomLeftPos.y = m_LevelBoundaries.height - m_Height;
	}
}