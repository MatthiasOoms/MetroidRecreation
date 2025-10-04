#include "pch.h"
#include "Ripper.h"
#include "Level.h"
#include "Texture.h"
#include "Tile.h"
#include "utils.h"

Ripper::Ripper(Point2f botLeft, Ripper::Type type)
	: Enemy()
	, m_Type{ type }
{
	m_Shape = Rectf(botLeft.x, botLeft.y, 16, 8);
	m_pTexture = new Texture("Resources/Ripper.png");
	m_TextRows = 4;
	m_TextCols = 1;
	m_ClipHeight = m_pTexture->GetHeight() / m_TextRows;
	m_ClipWidth = m_pTexture->GetWidth() / m_TextCols;
	m_NrOfFrames = 1;
	m_NrFramesPerSec = 1;
	m_AnimTime = 0;
	m_AnimFrame = 0;
	m_Velocity.x = 100;
	m_Health = INFINITE;
}

Ripper::~Ripper()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Ripper::Update(float elapsedSec, const Level* level, const std::vector<Tile*>& tiles)
{
	if (!m_IsFrozen)
	{
		FrameHandling(elapsedSec);
	}

	level->HandleRipperCollision(m_Shape, m_Velocity);

	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	for (size_t i{}; i < tiles.size(); ++i)
	{
		if (tiles[i])
		{
			if (!tiles[i]->IsExploded())
			{
				if (tiles[i]->WallCollision(m_Shape, m_Velocity))
				{
					m_Velocity *= -1;
				}
			}
		}
	}
}

void Ripper::DrawRipper(Point2f pos) const
{
	Rectf srcRect{ 0, m_ClipHeight, m_ClipWidth, m_ClipHeight };

	srcRect.bottom = ((int)m_Type + 1) * m_ClipHeight;
	if (IsFrozen())
	{
		srcRect.bottom = 0;
	}
	srcRect.left = m_AnimFrame * m_ClipWidth;

	m_pTexture->Draw(pos, srcRect);
}

void Ripper::Draw() const
{
	// Check velocity direction
	if (m_Velocity.x > 0)
	{
		DrawRipper(Point2f{ m_Shape.left, m_Shape.bottom });
	}
	else if (m_Velocity.x < 0)
	{
		glPushMatrix();

		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glTranslatef(m_Shape.width, 0, 0);
		glScalef(-1, 1, 1);

		DrawRipper(Point2f{ 0, 0 });

		glPopMatrix();
	}
}
