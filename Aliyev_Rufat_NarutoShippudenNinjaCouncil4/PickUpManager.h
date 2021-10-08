#pragma once
#include "PickUp.h"
#include <vector>
#include "structs.h"
#include "Level.h"
#include "HUD.h"
class Avatar;

class PickUpManager
{
public:
	PickUpManager();
	~PickUpManager();
	PickUpManager(const PickUpManager&) = delete;
	PickUpManager& operator=(const PickUpManager&) = delete;
	PickUpManager(PickUpManager&&) = delete;
	PickUpManager& operator=(PickUpManager&&) = delete;

	PickUp* AddItem(const Point2f& center, PickUp::Type type);
	void SetVelocity(Vector2f& velocity);
	void Update(float elapsedSec, Avatar* pAvatar, const Level& level, HUD& hud);
	void Draw() const;
	void DeleteAllPickUps();
private:
	std::vector<PickUp*> m_pItems;
	// METHOD
	void HitItem(Avatar* pAvatar, HUD& hud);
};
