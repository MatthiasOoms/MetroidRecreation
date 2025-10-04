#pragma once
#include "pch.h"
#include "SkreeBall.h"
#include "Texture.h"

SkreeBall::SkreeBall(Vector2f bulletVelocity, Rectf actorShape, float lifespan)
	: Projectile(bulletVelocity, actorShape, lifespan)
{
	m_pTexture = new Texture("Resources/SkreeBall.png");
	m_Rows = 1;
	m_Cols = 1;
	m_NrOfFrames = 1;
	m_Shape = Rectf(actorShape.left + actorShape.width / 2 - 12, actorShape.bottom + 20, 8, 8);
}

SkreeBall::~SkreeBall()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void SkreeBall::Draw(Point2f pos) const
{
	if (!HasExpired())
	{
		Rectf srcRect{ 0, 0, m_pTexture->GetWidth(), m_pTexture->GetHeight()};

		m_pTexture->Draw(pos, srcRect);
	}
	else
	{
		if (m_Frame <= m_NrOfFrames)
		{
			Rectf srcRect{
				m_Frame * (m_pTexture->GetWidth() / m_Cols),
				m_Rows * m_pTexture->GetHeight() / m_Rows,
				m_pTexture->GetWidth() / m_Cols,
				m_pTexture->GetHeight() / m_Rows };

			m_pTexture->Draw(pos, srcRect);
		}
	}
}
