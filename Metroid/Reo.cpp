#include "pch.h"
#include "Reo.h"
#include "Texture.h"
#include "Level.h"
#include "Tile.h"

Reo::Reo(Point2f botLeft, Type type)
	: Enemy()
	, m_Type{ type }
{
	m_Shape = Rectf(botLeft.x, botLeft.y, 32, 24);
	m_pTexture = new Texture("Resources/Reo.png");
	m_TextRows = 4;
	m_TextCols = 2;
	m_ClipHeight = m_pTexture->GetHeight() / m_TextRows;
	m_ClipWidth = m_pTexture->GetWidth() / m_TextCols;
	m_NrOfFrames = 2;
	m_NrFramesPerSec = 8;
	m_AnimTime = 0;
	m_AnimFrame = 0;
	m_Velocity.y = 5;
}

Reo::~Reo()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Reo::Update(float elapsedSec, const Level* level, const std::vector<Tile*>& tiles)
{
	if (!m_IsFrozen)
	{
		FrameHandling(elapsedSec);

		if (level->HandleSwoopersCollision(m_Shape, m_Velocity))
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

void Reo::Draw() const
{
	Rectf srcRect{ 0, m_ClipHeight, m_ClipWidth, m_ClipHeight };

	srcRect.bottom = ((int)m_Type + 1) * m_ClipHeight;
	if (IsFrozen())
	{
		srcRect.bottom = 0;
	}
	srcRect.left = m_AnimFrame * m_ClipWidth;

	m_pTexture->Draw(Point2f(m_Shape.left, m_Shape.bottom), srcRect);
}

void Reo::Attack(float elapsedSec, Rectf actorShape)
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
