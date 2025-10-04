#pragma once
#include <vector>

class Texture;
class Zeb;
class Avatar;
class Level;
class Tile;

class ZebSpawner
{
public:
	ZebSpawner(Point2f botLeft);
	ZebSpawner(const ZebSpawner& other) = delete;
	ZebSpawner& operator=(const ZebSpawner& other) = delete;
	ZebSpawner(ZebSpawner&& other) = delete;
	ZebSpawner& operator=(ZebSpawner&& other) = delete;
	~ZebSpawner();

	Zeb*& GetZeb(); // Should be const
	void Update(float elapsedSec, Avatar*& player, const Level* level, const std::vector<Tile*>& tiles);
	void Draw() const;

private:
	Zeb* m_pZeb;
	Rectf m_Shape;
	float m_AccuSec, m_Cooldown;
	Texture* m_pTexture;
};

