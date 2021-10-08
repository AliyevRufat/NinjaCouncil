#pragma once
#include "Vector2f.h"
#include <vector>
#include "Texture.h"
#include "SVGParser.h"
class Avatar;
class Platform;

class Level
{
public:
	explicit Level();
	Level(const Level& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(Level&& other) = delete;
	~Level();
	void DrawBackground() const;
	void DrawForeground() const;
	void Update(float elapsedSec, Avatar* pAvatar);
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	bool IsOnGround(const Rectf& actorShape, Vector2f& actorVelocity) const;
	Rectf GetBoundaries() const;

private:
	//Methods
	void CloudsUpdate(float elapsedSec, Avatar* pAvatar);
	//Datamembers
	std::vector<std::vector<Point2f>> m_Vertices;
	std::vector<Platform*> m_Platforms;
	//
	Texture m_LevelTexture;

	Texture m_SkyTexture;

	Texture m_CloudTexture;
	Texture m_CloudTexture2;

	const Rectf m_Boundaries;
	//
	float m_CloudXPos;
	float m_CloudXPos2;
	//
	const float m_HorCloudSpeed;
};