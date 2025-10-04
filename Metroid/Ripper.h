#pragma once
#include "Enemy.h"

class Avatar;
class Level;
class Tile;

class Ripper final : public Enemy
{
public:
	enum class Type
	{
		orange,
		pink,
		red
	};

	Ripper(Point2f botLeft, Type type);
	Ripper(const Ripper& other) = delete;
	Ripper& operator=(const Ripper& other) = delete;
	Ripper(Ripper&& other) = delete;
	Ripper& operator=(Ripper&& other) = delete;
	virtual ~Ripper() override final;

	void Update(float elapsedSec, const Level* level, const std::vector<Tile*>& tiles) override final;
	void Draw() const override final;
	void DrawRipper(Point2f pos) const;

private:
	Type m_Type;
};

