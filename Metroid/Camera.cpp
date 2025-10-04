#include "pch.h"
#include "Camera.h"
#include "utils.h"
#include <iostream>

Camera::Camera(float width, float height)
    : m_Width{width}
    , m_Height{height}
    , m_LevelBoundaries{ 0, 0, width, height }
{
}

Camera::~Camera()
{
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
    m_LevelBoundaries = levelBoundaries;
}

// TODO: Make camera work like Metroid
Point2f Camera::Track(const Rectf& target) const
{
    return Point2f{ 
        (target.left + target.width / 2) - m_Width / 2, 
        (target.bottom + target.height / 2) - m_Height / 2 };
    // Camera 50 left and 20 down of target
}

void Camera::Clamp(Point2f& bottomLeftPos)
{
    // Left and right
    if (bottomLeftPos.x < m_LevelBoundaries.left)
    {
        bottomLeftPos.x = m_LevelBoundaries.left;
    }
    else if (bottomLeftPos.x + m_Width > m_LevelBoundaries.left + m_LevelBoundaries.width)
    {
        bottomLeftPos.x = (m_LevelBoundaries.left + m_LevelBoundaries.width) - m_Width;
    }
    // Bottom and top
    if (bottomLeftPos.y < m_LevelBoundaries.bottom)
    {
        bottomLeftPos.y = m_LevelBoundaries.bottom;
    }
    else if (bottomLeftPos.y + m_Height > m_LevelBoundaries.bottom + m_LevelBoundaries.height)
    {
        bottomLeftPos.y = (m_LevelBoundaries.bottom + m_LevelBoundaries.height) - m_Height;
    }
}

void Camera::Transform(const Rectf& target)
{
    Point2f botLeft{ Track(target).x, Track(target).y };
    Clamp(botLeft);

    Rectf cameraRect{};
    cameraRect.left = botLeft.x;
    cameraRect.bottom = botLeft.y;
    cameraRect.width = m_Width;
    cameraRect.height = m_Height;

    glTranslatef(-botLeft.x, -botLeft.y, 0);
}
