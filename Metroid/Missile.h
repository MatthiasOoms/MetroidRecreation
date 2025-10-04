#pragma once
#include "Vector2f.h"
#include "Projectile.h"

class Texture;

class Missile final : public Projectile
{
public:
	Missile(Vector2f bulletVelocity, Rectf playerShape, float lifespan);
	Missile(const Missile& other) = delete;
	Missile& operator=(const Missile& other) = delete;
	Missile(Missile&& other) = delete;
	Missile& operator=(Missile&& other) = delete;
	~Missile();

private:
};

