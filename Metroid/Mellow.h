#pragma once
#include "Enemy.h"

class Texture;

class Mellow final : public Enemy
{
public:
	Mellow(Point2f botLeft);
	Mellow(const Mellow& other) = delete;
	Mellow& operator=(const Mellow& other) = delete;
	Mellow(Mellow&& other) = delete;
	Mellow& operator=(Mellow&& other) = delete;
	virtual ~Mellow() override final;

	void Update(float elapsedSec, const Level* level, const std::vector<Tile*>& tiles) override final;
	void Draw() const override final;
	void Attack(float elapsedSec, Rectf actorShape);

private:
	Texture* m_pTexture;
};

