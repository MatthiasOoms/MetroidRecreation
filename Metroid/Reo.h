#pragma once
#include "Enemy.h"

class Texture;

class Reo final : public Enemy
{
public:
	enum class Type
	{
		orange,
		pink,
		red
	};

	Reo(Point2f botLeft, Type type);
	Reo(const Reo& other) = delete;
	Reo& operator=(const Reo& other) = delete;
	Reo(Reo&& other) = delete;
	Reo& operator=(Reo&& other) = delete;
	virtual ~Reo() override final;

	void Update(float elapsedSec, const Level* level, const std::vector<Tile*>& tiles) override final;
	void Draw() const override final;
	void Attack(float elapsedSec, Rectf actorShape);

private:
	Texture* m_pTexture;
	Type m_Type;
};

