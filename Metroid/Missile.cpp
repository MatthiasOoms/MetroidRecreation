#pragma once
#include "pch.h"
#include "Missile.h"
#include "Texture.h"

Missile::Missile(Vector2f bulletVelocity, Rectf playerShape, float lifespan)
	: Projectile(bulletVelocity, playerShape, lifespan)
{
	m_pTexture = new Texture("Resources/Missile.png");
	m_Rows = 2;
	m_Cols = 2;
	m_NrOfFrames = 2;

	if (m_Velocity.x > 0)
	{
		m_Shape = Rectf(playerShape.left + playerShape.width, playerShape.bottom + playerShape.height / 1.7f, 8, 8);
	}
	else if (m_Velocity.x < 0)
	{
		m_Shape = Rectf(playerShape.left, playerShape.bottom + playerShape.height / 1.7f, 8, 8);
	}
}

Missile::~Missile()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}
