#include "pch.h"
#include "Mellow.h"
#include "Texture.h"
#include "Level.h"
#include "Tile.h"
// TODO: Make Mellow enemy
// Hangs on ceiling, swoops down until hits floor when player is under, ignores collision on way up

Mellow::Mellow(Point2f botLeft)
	: Enemy()
{
	m_Shape = Rectf(botLeft.x, botLeft.y, 24, 8);
	m_pTexture = new Texture("Resources/Mellow.png");
	m_TextRows = 2;
	m_TextCols = 2;
	m_ClipHeight = m_pTexture->GetHeight() / m_TextRows;
	m_ClipWidth = m_pTexture->GetWidth() / m_TextCols;
	m_NrOfFrames = 2;
	m_NrFramesPerSec = 8;
	m_AnimTime = 0;
	m_AnimFrame = 0;
	m_Velocity.y = 5;
}

Mellow::~Mellow()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Mellow::Update(float elapsedSec, const Level* level, const std::vector<Tile*>& tiles)
{
	if (!m_IsFrozen)
	{
		FrameHandling(elapsedSec);

		if (level->HandleMellowCollision(m_Shape, m_Velocity))
		{
			m_Velocity.y *= -1;
		}

		m_Shape.left += m_Velocity.x * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;

		for (size_t i{}; i < tiles.size(); ++i)
		{
			if (tiles[i])
			{
				if (!tiles[i]->IsExploded())
				{
					if (tiles[i]->FloorCollision(m_Shape, m_Velocity))
					{
						m_Velocity.y *= -1;
					}
				}
			}
		}
	}
}

void Mellow::Draw() const
{
	Rectf srcRect{ 0, m_ClipHeight, m_ClipWidth, m_ClipHeight };

	srcRect.bottom = m_ClipHeight;
	if (IsFrozen())
	{
		srcRect.bottom = 0;
	}
	srcRect.left = m_AnimFrame * m_ClipWidth;

	m_pTexture->Draw(Point2f(m_Shape.left, m_Shape.bottom), srcRect);
}

void Mellow::Attack(float elapsedSec, Rectf actorShape)
{
	if (abs(m_Shape.left - actorShape.left) < 50)
	{
		if (m_Shape.left < actorShape.left)
		{
			m_Velocity.x = 100;
		}
		else
		{
			m_Velocity.x = -100;
		}
		if (abs(m_Shape.bottom - actorShape.bottom > 150))
		{
			m_Velocity.y = -150;
		}
	}

	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	m_Velocity.x = 0;
}
