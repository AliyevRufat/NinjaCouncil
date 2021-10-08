#pragma once
class Avatar;
class Camera
{
public:

	Camera(float width, float height);
	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void TransformForeGround(const Rectf& target) const;
	void TransformBackGround(const Rectf& target) const;
	void Update(float elapsedSec, Avatar* pAvatar);
	void SetHasMovedToNewLevel(bool hasMovedToNewLevel);

private:

	//Datamembers
	float m_Width;
	float m_Height;
	Rectf m_LevelBoundaries;
	float m_FreeMovingAreaWidth;
	Rectf m_SubZone;
	bool m_HasMovedToNewLevel;
	float m_MiddleOfMap;
	Point2f m_CameraPos;
	//Helper Methods
	Point2f Track(const Rectf& target) const;
	void Clamp(Point2f& bottomLeftPos) const;
	void SubZoneManager(const Rectf& target);
};
