#include "pch.h"
#include "Level.h"
#include "utils.h"
#include <iostream>
#include "Platform.h"
#include "Avatar.h"

Level::Level()
	:m_LevelTexture{ "Resources/Images/Background.png" }
	, m_SkyTexture{ "Resources/Images/Sky.png" }
	//, m_SkyTexture2{ "Resources/Images/Sky2.png" }
	, m_CloudTexture{ "Resources/Images/Clouds.png" }
	, m_CloudTexture2{ "Resources/Images/Clouds.png" }
	, m_Boundaries{ Rectf{ 0, 0, m_LevelTexture.GetWidth() , m_LevelTexture.GetHeight() + 100} }
	, m_CloudXPos{ 0 }
	, m_CloudXPos2{ m_CloudXPos + m_SkyTexture.GetWidth() }
	, m_HorCloudSpeed{ 10.0f }
{
	SVGParser::GetVerticesFromSvgFile("Resources/Images/level.svg", m_Vertices);
	//level1
	m_Platforms.push_back(new Platform(Platform::Type::moving, Point2f{ 1050  ,90 }, 250));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 1650,180 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 1530,280 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 1650,380 }));
	m_Platforms.push_back(new Platform(Platform::Type::moving, Point2f{ 1040, 450 }, 210));
	m_Platforms.push_back(new Platform(Platform::Type::moving, Point2f{ 2050, 450 }, 120));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 3522,550 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 3522,650 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 3522,750 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 3365,850 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 3365,950 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 3365,1050 }));
	m_Platforms.push_back(new Platform(Platform::Type::moving, Point2f{ 2410  ,750 }, 310));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 1333,860 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 1333,960 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 1333,1060 }));
	//level2
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 6394,860 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 6394,950 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 6394,1040 }));
	m_Platforms.push_back(new Platform(Platform::Type::moving, Point2f{ 6020,765 }, 205));
	m_Platforms.push_back(new Platform(Platform::Type::moving, Point2f{ 7990,1075 }, 1530));
	m_Platforms.push_back(new Platform(Platform::Type::moving, Point2f{ 8200,765 }, 500));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 7455,170 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 7455,260 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 7455,350 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 7455,440 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 8611,540 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 8611,640 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 8611,735 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 9388,850 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 9388,930 }));
	m_Platforms.push_back(new Platform(Platform::Type::normal, Point2f{ 9388,1010 }));
}

Level::~Level()
{
	for (size_t index = 0; index < m_Platforms.size(); index++)
	{
		delete m_Platforms[index];
	}
}

void Level::DrawForeground() const
{
	glPushMatrix();
	glScalef(4, 4, 0);
	m_LevelTexture.Draw();
	glScalef(0.5, 0.5, 0);
	for (size_t index = 0; index < m_Platforms.size(); index++)
	{
		m_Platforms[index]->Draw();
	}
	glScalef(0.25, 0.25, 0);
	glPopMatrix();
}

void Level::DrawBackground() const
{
	glPushMatrix();
	glScalef(4, 4, 0);
	m_SkyTexture.Draw(Rectf{ 0 ,0 , m_SkyTexture.GetWidth(),m_SkyTexture.GetHeight() });
	//
	m_CloudTexture.Draw(Rectf{ m_CloudXPos ,0 , m_CloudTexture.GetWidth(),m_CloudTexture.GetHeight() });
	m_CloudTexture2.Draw(Rectf{ m_CloudXPos2 ,0 ,m_CloudTexture2.GetWidth(),m_CloudTexture2.GetHeight() });
	glScalef(0.25, 0.25, 0);
	glPopMatrix();
}

void Level::Update(float elapsedSec, Avatar* pAvatar)
{
	CloudsUpdate(elapsedSec, pAvatar);
	//
	for (size_t index = 0; index < m_Platforms.size(); index++)
	{
		m_Platforms[index]->Update(elapsedSec);
	}
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	//checks from top to bottom raycast
	Point2f startPointRay{ actorShape.width / 2.f + actorShape.left ,actorShape.bottom + actorShape.height };
	Point2f endPointRay{ actorShape.width / 2.f + actorShape.left ,actorShape.bottom };
	utils::HitInfo hitInfo;
	for (size_t element = 0; element < m_Vertices.size(); element++)
	{
		if (utils::Raycast(m_Vertices[element], startPointRay, endPointRay, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y;
			actorVelocity.y = 0;
		}
	}
	if (actorVelocity.y != 0)
	{
		//checks from bottom to top raycast
		const int offset = 5;
		startPointRay = Point2f{ actorShape.width / 2.f + actorShape.left ,actorShape.bottom };
		endPointRay = Point2f{ actorShape.width / 2.f + actorShape.left ,actorShape.bottom + actorShape.height + offset };
		for (size_t element = 0; element < m_Vertices.size(); element++)
		{
			if (utils::Raycast(m_Vertices[element], startPointRay, endPointRay, hitInfo))
			{
				const float gravity = -9.81f;
				actorShape.bottom = hitInfo.intersectPoint.y - actorShape.height - offset;
				actorVelocity.y = gravity;
			}
		}
	}
	if (actorVelocity.x != 0)
	{
		//checks rightside of actor with raycast if it hits a svg wall
		startPointRay = Point2f{ actorShape.left,actorShape.bottom + actorShape.height / 3 };
		endPointRay = Point2f{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 3 };
		for (size_t element = 0; element < m_Vertices.size(); element++)
		{
			if (utils::Raycast(m_Vertices[element], startPointRay, endPointRay, hitInfo) && actorVelocity.x >= 0)
			{
				actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
				actorVelocity.x = 0;
			}
		}
		startPointRay = Point2f{ actorShape.left,actorShape.bottom + actorShape.height / 3 * 2 };
		endPointRay = Point2f{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 3 * 2 };
		for (size_t element = 0; element < m_Vertices.size(); element++)
		{
			if (utils::Raycast(m_Vertices[element], startPointRay, endPointRay, hitInfo) && actorVelocity.x >= 0)
			{
				actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
				actorVelocity.x = 0;
			}
		}
		startPointRay = Point2f{ actorShape.left,actorShape.bottom + actorShape.height };
		endPointRay = Point2f{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height };
		for (size_t element = 0; element < m_Vertices.size(); element++)
		{
			if (utils::Raycast(m_Vertices[element], startPointRay, endPointRay, hitInfo) && actorVelocity.x >= 0)
			{
				actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
				actorVelocity.x = 0;
			}
		}
		//checks leftside of actor with raycast if it hits a svg wall
		startPointRay = Point2f{ actorShape.left + actorShape.width,actorShape.bottom + actorShape.height / 3 };
		endPointRay = Point2f{ actorShape.left , actorShape.bottom + actorShape.height / 3 };
		for (size_t element = 0; element < m_Vertices.size(); element++)
		{
			if (utils::Raycast(m_Vertices[element], startPointRay, endPointRay, hitInfo) && actorVelocity.x <= 0)
			{
				actorShape.left = hitInfo.intersectPoint.x;
				actorVelocity.x = 0;
			}
		}
		startPointRay = Point2f{ actorShape.left + actorShape.width,actorShape.bottom + actorShape.height / 3 * 2 };
		endPointRay = Point2f{ actorShape.left , actorShape.bottom + actorShape.height / 3 * 2 };
		for (size_t element = 0; element < m_Vertices.size(); element++)
		{
			if (utils::Raycast(m_Vertices[element], startPointRay, endPointRay, hitInfo) && actorVelocity.x <= 0)
			{
				actorShape.left = hitInfo.intersectPoint.x;
				actorVelocity.x = 0;
			}
		}
		startPointRay = Point2f{ actorShape.left + actorShape.width,actorShape.bottom + actorShape.height };
		endPointRay = Point2f{ actorShape.left , actorShape.bottom + actorShape.height };
		for (size_t element = 0; element < m_Vertices.size(); element++)
		{
			if (utils::Raycast(m_Vertices[element], startPointRay, endPointRay, hitInfo) && actorVelocity.x <= 0)
			{
				actorShape.left = hitInfo.intersectPoint.x;
				actorVelocity.x = 0;
			}
		}
	}
	//check platform handlecollision
	for (size_t index = 0; index < m_Platforms.size(); index++)
	{
		m_Platforms[index]->HandleCollision(actorShape, actorVelocity);
	}
}

bool Level::IsOnGround(const Rectf& actorShape, Vector2f& actorVelocity) const
{
	Point2f startPointRay{ actorShape.width / 2.f + actorShape.left ,actorShape.bottom + actorShape.height };
	Point2f endPointRay{ actorShape.width / 2.f + actorShape.left ,actorShape.bottom - 1 };
	utils::HitInfo hitInfo;

	for (size_t i = 0; i < m_Vertices.size(); i++)
	{
		if ((utils::Raycast(m_Vertices[i], startPointRay, endPointRay, hitInfo) || utils::Raycast(m_Vertices[i], startPointRay, endPointRay, hitInfo)) && actorVelocity.y <= 0)
		{
			return true;
		}
	}
	bool collides = false;
	for (size_t i = 0; i < m_Platforms.size(); i++)
	{
		if (m_Platforms[i]->IsOnGround(actorShape, actorVelocity))
		{
			collides = true;
		}
	}
	if (collides)
	{
		return true;
	}
	return false;
}

Rectf Level::GetBoundaries()const
{
	return m_Boundaries;
}

void Level::CloudsUpdate(float elapsedSec, Avatar* pAvatar)
{
	if (m_CloudXPos + m_CloudTexture.GetWidth() <= 0)
	{
		m_CloudXPos = m_CloudTexture.GetWidth();
	}
	else if (m_CloudXPos2 + m_CloudTexture2.GetWidth() <= 0)
	{
		m_CloudXPos2 = m_CloudTexture2.GetWidth();
	}
	//
	m_CloudXPos -= m_HorCloudSpeed * elapsedSec;
	m_CloudXPos2 -= m_HorCloudSpeed * elapsedSec;
}