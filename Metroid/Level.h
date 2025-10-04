#pragma once
#include <vector>
#include "Vector2f.h"
#include "Zoomer.h"

class Texture;
class Projectile;

class Level
{
public:
	Level();
	Level(const Level& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(Level&& other) = delete;
	~Level();
	
	void Draw() const;
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	void HandleZoomerCollision(Rectf& actorShape, Vector2f& actorVelocity, Zoomer::Direction actorDirection) const;
	void HandleRipperCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	bool HandleSwoopersCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	bool HandleMellowCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	void HandleProjectileCollision(Projectile*& projetile) const;
	bool CanStand(Rectf& actorShape) const;
	void ClockwiseZoomerCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	void CounterClockwiseZoomerCollision(Rectf& actorShape, Vector2f& actorVelocity) const;

	void ClockwiseZoomerLost(Rectf& actorShape, Vector2f& actorVelocity) const;
	void CounterClockwiseZoomerLost(Rectf& actorShape, Vector2f& actorVelocity) const;

	bool IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity) const;
	Rectf GetBoundaries() const;

private:
	std::vector<std::vector<Point2f>> m_Vertices;

	Texture* m_pBackgroundText;
	Rectf m_Boundaries;
};

