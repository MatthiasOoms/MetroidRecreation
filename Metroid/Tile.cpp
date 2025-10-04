#pragma once
#include "pch.h"
#include "Tile.h"
#include "Texture.h"
#include "Avatar.h"
#include "Enemy.h"
#include "Vector2f.h"
#include "utils.h"

Tile::Tile(Point2f pos, Type tileType)
    : m_Shape{ pos.x, pos.y, 16, 16 }
    , m_Type{ tileType }
    , m_pTexture{ new Texture("Resources/Tileset.png")}
    , m_IsExploded{ false }

    , m_Rows{ 5 }
    , m_Cols{ 4 }
    , m_ClipHeight{ m_pTexture->GetHeight() / m_Rows }
    , m_ClipWidth{ m_pTexture->GetWidth() / m_Cols }
    , m_Frame{ 0 }
    , m_Timer{ 0 }
    , m_DeadTimer { 3 }
{
}

Tile::~Tile()
{
    delete m_pTexture;
    m_pTexture = nullptr;
}

Rectf Tile::GetShape() const
{
    return m_Shape;
}

bool Tile::IsExploded() const
{
    return m_IsExploded;
}

void Tile::Draw() const
{
    if (m_Frame < m_Cols)
    {
        Rectf srcRect{ (int)m_Frame * m_ClipWidth, ((int)m_Type + 1) * m_ClipHeight, m_ClipWidth, m_ClipHeight };

        m_pTexture->Draw(m_Shape, srcRect);
    }
    else
    {
        Rectf srcRect{ m_Cols * m_ClipWidth, ((int)m_Type + 1) * m_ClipHeight, m_ClipWidth, m_ClipHeight };

    }
}

void Tile::Update(float elapsedSec)
{
    if (m_IsExploded)
    {
        m_Timer += elapsedSec;
        if (m_Frame < m_Cols)
        {
            ++m_Frame;
        }
        if (m_Timer >= m_DeadTimer)
        {
            m_Frame = 0;
            m_Timer = 0;
            m_IsExploded = false;
        }
    }
}

void Tile::Explode()
{
    m_IsExploded = true;
}

bool Tile::FloorCollision(Rectf& actorShape, Vector2f& actorVelocity)
{
	// Are we on a tile
	utils::HitInfo hitInfo{};

	Rectf tileShape{ GetShape() };
	std::vector<Point2f> tile;
	tile.push_back(Point2f(tileShape.left, tileShape.bottom));
	tile.push_back(Point2f(tileShape.left, tileShape.bottom + tileShape.height));
	tile.push_back(Point2f(tileShape.left + tileShape.width, tileShape.bottom + tileShape.height));
	tile.push_back(Point2f(tileShape.left + tileShape.width, tileShape.bottom));

	int offset{ 5 };

	// Vertical collision vars
	Point2f leftBotVert{ actorShape.left + offset, actorShape.bottom };
	Point2f leftMidVert{ actorShape.left + offset, actorShape.bottom + actorShape.height / 2 };
	Point2f leftTopVert{ actorShape.left + offset, actorShape.bottom + actorShape.height };

	Point2f rightBotVert{ actorShape.left + actorShape.width - offset, actorShape.bottom };
	Point2f rightMidVert{ actorShape.left + actorShape.width - offset, actorShape.bottom + actorShape.height / 2 };
	Point2f rightTopVert{ actorShape.left + actorShape.width - offset, actorShape.bottom + actorShape.height };

	// Horizontal collision vars
	Point2f leftBotHori{ actorShape.left, actorShape.bottom + offset };
	Point2f midBotHori{ actorShape.left + actorShape.width / 2, actorShape.bottom + offset };
	Point2f rightBotHori{ actorShape.left + actorShape.width, actorShape.bottom + offset };

	Point2f midMidHori{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };

	Point2f leftTopHori{ actorShape.left, actorShape.bottom + actorShape.height - offset };
	Point2f midTopHori{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height - offset };
	Point2f rightTopHori{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height - offset };

	// Horizontal collision
	// Left
	if (utils::Raycast(tile, midBotHori, leftBotHori, hitInfo) || utils::Raycast(tile, midMidHori, leftMidVert, hitInfo) || utils::Raycast(tile, midTopHori, leftTopHori, hitInfo))
	{
 		actorShape.left = hitInfo.intersectPoint.x;
	}
	// Right
	if (utils::Raycast(tile, midBotHori, rightBotHori, hitInfo) || utils::Raycast(tile, midMidHori, rightMidVert, hitInfo) || utils::Raycast(tile, midTopHori, rightTopHori, hitInfo))
	{
		actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
	}

	// Vertical collision
	// Floor
	if (utils::Raycast(tile, leftMidVert, leftBotVert, hitInfo) || utils::Raycast(tile, rightMidVert, rightBotVert, hitInfo))
	{
		if (actorVelocity.y <= 0)
		{
			actorShape.bottom = hitInfo.intersectPoint.y;
			return true;
		}
	}
	// Roof
	if (utils::Raycast(tile, leftMidVert, leftTopVert, hitInfo) || utils::Raycast(tile, rightMidVert, rightTopVert, hitInfo))
	{
		actorShape.bottom = hitInfo.intersectPoint.y - actorShape.height - 2;
		actorVelocity.y = -50;
	}
	return false;
}

bool Tile::WallCollision(Rectf& actorShape, Vector2f& actorVelocity)
{
	// Are we on a tile
	utils::HitInfo hitInfo{};

	Rectf tileShape{ GetShape() };
	std::vector<Point2f> tile;
	tile.push_back(Point2f(tileShape.left, tileShape.bottom));
	tile.push_back(Point2f(tileShape.left, tileShape.bottom + tileShape.height));
	tile.push_back(Point2f(tileShape.left + tileShape.width, tileShape.bottom + tileShape.height));
	tile.push_back(Point2f(tileShape.left + tileShape.width, tileShape.bottom));

	int offset{ 5 };

	// Vertical collision vars
	Point2f leftBotVert{ actorShape.left + offset, actorShape.bottom };
	Point2f leftMidVert{ actorShape.left + offset, actorShape.bottom + actorShape.height / 2 };
	Point2f leftTopVert{ actorShape.left + offset, actorShape.bottom + actorShape.height };

	Point2f rightBotVert{ actorShape.left + actorShape.width - offset, actorShape.bottom };
	Point2f rightMidVert{ actorShape.left + actorShape.width - offset, actorShape.bottom + actorShape.height / 2 };
	Point2f rightTopVert{ actorShape.left + actorShape.width - offset, actorShape.bottom + actorShape.height };

	// Horizontal collision vars
	Point2f leftBotHori{ actorShape.left, actorShape.bottom + offset };
	Point2f midBotHori{ actorShape.left + actorShape.width / 2, actorShape.bottom + offset };
	Point2f rightBotHori{ actorShape.left + actorShape.width, actorShape.bottom + offset };

	Point2f midMidHori{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };

	Point2f leftTopHori{ actorShape.left, actorShape.bottom + actorShape.height - offset };
	Point2f midTopHori{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height - offset };
	Point2f rightTopHori{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height - offset };

	// Horizontal collision
	// Left
	if (utils::Raycast(tile, midBotHori, leftBotHori, hitInfo) || utils::Raycast(tile, midMidHori, leftMidVert, hitInfo) || utils::Raycast(tile, midTopHori, leftTopHori, hitInfo))
	{
		actorShape.left = hitInfo.intersectPoint.x;
		return true;
	}
	// Right
	if (utils::Raycast(tile, midBotHori, rightBotHori, hitInfo) || utils::Raycast(tile, midMidHori, rightMidVert, hitInfo) || utils::Raycast(tile, midTopHori, rightTopHori, hitInfo))
	{
		actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
		return true;
	}

	// Vertical collision
	// Floor
	if (utils::Raycast(tile, leftMidVert, leftBotVert, hitInfo) || utils::Raycast(tile, rightMidVert, rightBotVert, hitInfo))
	{
		if (actorVelocity.y <= 0)
		{
			actorShape.bottom = hitInfo.intersectPoint.y;
		}
	}
	// Roof
	if (utils::Raycast(tile, leftMidVert, leftTopVert, hitInfo) || utils::Raycast(tile, rightMidVert, rightTopVert, hitInfo))
	{
		actorShape.bottom = hitInfo.intersectPoint.y - actorShape.height - 2;
		actorVelocity.y = -50;
	}
	return false;
}
