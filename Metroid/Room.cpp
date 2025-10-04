#include "pch.h"
#include "Room.h"
#include "Avatar.h"
#include "utils.h"

Room::Room(Point2f pos, int roomWidth, int roomHeight)
	: m_Shape{pos.x, pos.y, roomWidth * 256.f, roomHeight * 240.f}
	, m_IsActive{ false }
{
}

Room::~Room()
{
}

Rectf Room::GetShape() const
{
	return m_Shape;
}

bool Room::GetActive() const
{
	return m_IsActive;
}

void Room::Update(Avatar* player)
{
	if (utils::IsOverlapping(player->GetShape(), m_Shape))
	{
		m_IsActive = true;
	}
	else
	{
		m_IsActive = false;
	}
}
