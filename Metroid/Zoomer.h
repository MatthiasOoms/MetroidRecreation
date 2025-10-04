#pragma once
#include "Enemy.h"

class Avatar;
class Level;
class Tile;

class Zoomer final : public Enemy
{
public:
	enum class Type
	{
		orange,
		pink,
		red
	};
	enum class Direction
	{
		clockwise,
		counterclockwise
	};

	Zoomer(Point2f botLeft, Type zoomerType, Direction zoomerDirection);
	Zoomer(const Zoomer& other) = delete;
	Zoomer& operator=(const Zoomer& other) = delete;
	Zoomer(Zoomer&& other) = delete;
	Zoomer& operator=(Zoomer&& other) = delete;
	virtual ~Zoomer();

	void Update(float elapsedSec, const Level* level, const std::vector<Tile*>& tiles) override final;
	void Draw() const override final;
	void DrawZoomer(Point2f pos) const;
	void ClockwiseDraw() const;
	void CounterClockwiseDraw() const;

private:

	Type m_Type;
	Direction m_Direction;
};

