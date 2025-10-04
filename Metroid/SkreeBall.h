#pragma once
#include "Vector2f.h"
#include "Projectile.h"

class Texture;

class SkreeBall final : public Projectile
{
public:
	SkreeBall(Vector2f bulletVelocity, Rectf actorShape, float lifespan);
	SkreeBall(const SkreeBall& other) = delete;
	SkreeBall& operator=(const SkreeBall& other) = delete;
	SkreeBall(SkreeBall&& other) = delete;
	SkreeBall& operator=(SkreeBall&& other) = delete;
	virtual ~SkreeBall() final;

	virtual void Draw(Point2f pos) const override;

private:
	
};

