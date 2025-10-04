#pragma once
#include "Enemy.h"

class Avatar;
class Level;
class Tile;

class Waver final : public Enemy
{
public:
	enum class Type
	{
		green,
		blue
	};

	Waver(Point2f botLeft, Type type);
	Waver(const Waver& other) = delete;
	Waver& operator=(const Waver& other) = delete;
	Waver(Waver&& other) = delete;
	Waver& operator=(Waver&& other) = delete;
	virtual ~Waver() override final;

	void Update(float elapsedSec, const Level* level, const std::vector<Tile*>& tiles) override final;
	void Draw() const override final;
	void DrawWaver(Point2f pos) const;

private:
	Type m_Type;
	float m_Increment;

};



