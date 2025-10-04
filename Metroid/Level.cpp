#pragma once
#include "pch.h"
#include "Level.h"
#include "Texture.h"
#include "Projectile.h"
#include "utils.h"
#include "SVGParser.h"
#include "SoundStream.h"
#include <iostream>

Level::Level()
	: m_pBackgroundText{ new Texture{"Resources/MetroidLevel.png"} }
	, m_Vertices{}
	, m_Boundaries{ 0, 0, m_pBackgroundText->GetWidth(), m_pBackgroundText->GetHeight() }
{
	SVGParser::GetVerticesFromSvgFile("Resources/MetroidFullLevelFinal.svg", m_Vertices);
}

Level::~Level()
{
	delete m_pBackgroundText;
	m_pBackgroundText = nullptr;
}

void Level::Draw() const
{
	m_pBackgroundText->Draw();
}

// Swoopers are enemies that hang from the ceiling before attacking - Metroid wiki
bool Level::HandleSwoopersCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	// Variable declaration
	utils::HitInfo hitInfo{};
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
	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		// Left
		if (utils::Raycast(m_Vertices[i], midBotHori, leftBotHori, hitInfo) || utils::Raycast(m_Vertices[i], midMidHori, leftMidVert, hitInfo) || utils::Raycast(m_Vertices[i], midTopHori, leftTopHori, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x;
		}
		// Right
		if (utils::Raycast(m_Vertices[i], midBotHori, rightBotHori, hitInfo) || utils::Raycast(m_Vertices[i], midMidHori, rightMidVert, hitInfo) || utils::Raycast(m_Vertices[i], midTopHori, rightTopHori, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
		}
	}
	
	// Vertical collision
	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		// Floor
		if (utils::Raycast(m_Vertices[i], leftMidVert, leftBotVert, hitInfo) || utils::Raycast(m_Vertices[i], rightMidVert, rightBotVert, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y;
			return true;
		}
		// Roof
		if (utils::Raycast(m_Vertices[i], leftMidVert, leftTopVert, hitInfo) || utils::Raycast(m_Vertices[i], rightMidVert, rightTopVert, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y - actorShape.height - 2;
		}
	}
	return false;
}

bool Level::HandleMellowCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	// Variable declaration
	utils::HitInfo hitInfo{};
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

	// Vertical collision
	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		// Floor
		if (utils::Raycast(m_Vertices[i], leftMidVert, leftBotVert, hitInfo) || utils::Raycast(m_Vertices[i], rightMidVert, rightBotVert, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y;
			return true;
		}
		// Roof
		if (utils::Raycast(m_Vertices[i], leftMidVert, leftTopVert, hitInfo) || utils::Raycast(m_Vertices[i], rightMidVert, rightTopVert, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y - actorShape.height - 2;
		}
	}
	return false;
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	// Variable declaration
	utils::HitInfo hitInfo{};
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
	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		// Left
		if (utils::Raycast(m_Vertices[i], midBotHori, leftBotHori, hitInfo) || utils::Raycast(m_Vertices[i], midMidHori, leftMidVert, hitInfo) || utils::Raycast(m_Vertices[i], midTopHori, leftTopHori, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x;
		}
		// Right
		if (utils::Raycast(m_Vertices[i], midBotHori, rightBotHori, hitInfo) || utils::Raycast(m_Vertices[i], midMidHori, rightMidVert, hitInfo) || utils::Raycast(m_Vertices[i], midTopHori, rightTopHori, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
		}
	}

	// Vertical collision
	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		// Floor
		if (utils::Raycast(m_Vertices[i], leftMidVert, leftBotVert, hitInfo) || utils::Raycast(m_Vertices[i], rightMidVert, rightBotVert, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y;
			actorVelocity.y = 0;
		}
		// Roof
		if (utils::Raycast(m_Vertices[i], leftMidVert, leftTopVert, hitInfo) || utils::Raycast(m_Vertices[i], rightMidVert, rightTopVert, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y - actorShape.height - 2;
			actorVelocity.y = -50;
		}
	}
}

void Level::HandleZoomerCollision(Rectf& actorShape, Vector2f& actorVelocity, Zoomer::Direction actorDirection) const
{

	if (actorDirection == Zoomer::Direction::clockwise)
	{
		ClockwiseZoomerCollision(actorShape, actorVelocity);
	}
	else
	{
		CounterClockwiseZoomerCollision(actorShape, actorVelocity);
	}
}

void Level::HandleRipperCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	// Variable declaration
	utils::HitInfo hitInfo{};
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
	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		// Left
		if (utils::Raycast(m_Vertices[i], midBotHori, leftBotHori, hitInfo) || utils::Raycast(m_Vertices[i], midMidHori, leftMidVert, hitInfo) || utils::Raycast(m_Vertices[i], midTopHori, leftTopHori, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x;
			actorVelocity.x *= -1;
		}
		// Right
		if (utils::Raycast(m_Vertices[i], midBotHori, rightBotHori, hitInfo) || utils::Raycast(m_Vertices[i], midMidHori, rightMidVert, hitInfo) || utils::Raycast(m_Vertices[i], midTopHori, rightTopHori, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
			actorVelocity.x *= -1;
		}
	}

	// Vertical collision
	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		// Floor
		if (utils::Raycast(m_Vertices[i], leftMidVert, leftBotVert, hitInfo) || utils::Raycast(m_Vertices[i], rightMidVert, rightBotVert, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y;
		}
		// Roof
		if (utils::Raycast(m_Vertices[i], leftMidVert, leftTopVert, hitInfo) || utils::Raycast(m_Vertices[i], rightMidVert, rightTopVert, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y - actorShape.height - 2;
		}
	}
}

void Level::HandleProjectileCollision(Projectile*& projectile) const
{
	Rectf missileShape{ projectile->GetShape() };

	utils::HitInfo hitInfo{};
	Point2f leftMid{ missileShape.left, missileShape.bottom + missileShape.height / 2 };
	Point2f rightMid{ missileShape.left + missileShape.width, missileShape.bottom + missileShape.height / 2 };
	Point2f topMid{ missileShape.left + missileShape.width / 2, missileShape.bottom + missileShape.height };
	Point2f botMid{ missileShape.left + missileShape.width / 2, missileShape.bottom };

	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		if (utils::Raycast(m_Vertices[i], leftMid, rightMid, hitInfo))
		{
			projectile->Collision();
		}
		else if (utils::Raycast(m_Vertices[i], topMid, botMid, hitInfo))
		{
			projectile->Collision();
		}
	}
}

bool Level::CanStand(Rectf& actorShape) const
{
	utils::HitInfo hitInfo{};

	Point2f leftMid{ actorShape.left, actorShape.bottom + actorShape.height / 2 };
	Point2f leftFloat{ actorShape.left, actorShape.bottom + actorShape.height * 2 };
	Point2f rightMid{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 2 };
	Point2f rightFloat{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height * 2 };
	
	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		if (utils::Raycast(m_Vertices[i], leftMid, leftFloat, hitInfo) || utils::Raycast(m_Vertices[i], rightMid, rightFloat, hitInfo))
		{
			return false;
		}
	}
	return true;
}

void Level::ClockwiseZoomerCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	// Variable declaration
	utils::HitInfo hitInfo{};
	bool isHit{ false };

	// Vertical collision
	Point2f middle{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };
	Point2f midLeft{ actorShape.left, actorShape.bottom + actorShape.height / 2 };
	Point2f midRight{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 2 };
	Point2f midBottom{ actorShape.left + actorShape.width / 2, actorShape.bottom };
	Point2f midTop{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height - 1 };

	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		// Roof
		if (utils::Raycast(m_Vertices[i], middle, midTop, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y - actorShape.height + 1;
			actorVelocity.x = -50;
			actorVelocity.y = 0;
			isHit = true;
		}
		// Left
		if (utils::Raycast(m_Vertices[i], middle, midLeft, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x;
			actorVelocity.x = 0;
			actorVelocity.y = -50;
			isHit = true;
		}
		// Floor
		if (utils::Raycast(m_Vertices[i], middle, midBottom, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y;
			actorVelocity.x = 50;
			actorVelocity.y = 0;
			isHit = true;
		}
		// Right
		if (utils::Raycast(m_Vertices[i], middle, midRight, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
			actorVelocity.x = 0;
			actorVelocity.y = 50;
			isHit = true;
		}
	}

	if (!isHit)
	{
		ClockwiseZoomerLost(actorShape, actorVelocity);
	}
}

void Level::CounterClockwiseZoomerCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	// Variable declaration
	utils::HitInfo hitInfo{};
	bool isHit{ false };

	// Vertical collision
	Point2f middle{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };
	Point2f midLeft{ actorShape.left, actorShape.bottom + actorShape.height / 2 };
	Point2f midRight{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 2 };
	Point2f midBottom{ actorShape.left + actorShape.width / 2, actorShape.bottom };
	Point2f midTop{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height - 1 };

	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		// Roof
		if (utils::Raycast(m_Vertices[i], middle, midTop, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y - actorShape.height + 1;
			actorVelocity.x = 50;
			actorVelocity.y = 0;
			isHit = true;
		}
		// Right
		if (utils::Raycast(m_Vertices[i], middle, midRight, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
			actorVelocity.x = 0;
			actorVelocity.y = -50;
			isHit = true;
		}
		// Floor
		if (utils::Raycast(m_Vertices[i], middle, midBottom, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y;
			actorVelocity.x = -50;
			actorVelocity.y = 0;
			isHit = true;
		}
		// Left
		if (utils::Raycast(m_Vertices[i], middle, midLeft, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x;
			actorVelocity.x = 0;
			actorVelocity.y = 50;
			isHit = true;
		}
	}

	if (!isHit)
	{
		CounterClockwiseZoomerLost(actorShape, actorVelocity);
	}
}

void Level::ClockwiseZoomerLost(Rectf& actorShape, Vector2f& actorVelocity) const
{
	if (actorVelocity.x > 0)
	{
		actorShape.bottom -= 1;

		return;
	}
	if (actorVelocity.x < 0)
	{
		actorShape.bottom += 1;

		return;
	}
	if (actorVelocity.y > 0)
	{
		actorShape.left += 1;

		return;
	}
	if (actorVelocity.y < 0)
	{
		actorShape.left -= 1;

		return;
	}
}

void Level::CounterClockwiseZoomerLost(Rectf& actorShape, Vector2f& actorVelocity) const
{
	if (actorVelocity.x > 0)
	{
		actorShape.bottom += 1;

		return;
	}
	if (actorVelocity.x < 0)
	{
		actorShape.bottom -= 1;

		return;
	}
	if (actorVelocity.y > 0)
	{
		actorShape.left -= 1;

		return;
	}
	if (actorVelocity.y < 0)
	{
		actorShape.left += 1;

		return;
	}
}

bool Level::IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity) const
{
	Point2f leftBot{ actorShape.left, actorShape.bottom-3 };
	Point2f leftTop{ actorShape.left, actorShape.bottom + actorShape.height };
	Point2f rightBot{ actorShape.left + actorShape.width, actorShape.bottom-3 };
	Point2f rightTop{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height };
	utils::HitInfo hitInfo{};
	
	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		if (utils::Raycast(m_Vertices[i], leftTop, leftBot, hitInfo) || utils::Raycast(m_Vertices[i], rightTop, rightBot, hitInfo))
		{
			if (actorVelocity.y == 0)
			{
				return true;
			}
		}
	}
	return false;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}
