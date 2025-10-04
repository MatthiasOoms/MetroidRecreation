#include "pch.h"
#include "Door.h"
#include "Texture.h"

Door::Door(Point2f pos, Type doorType)
    : m_Shape{ pos.x, pos.y, 64, 48 }
    , m_pTexture{ new Texture("Resources/Doors.png") }
    , m_Type{ doorType }
    , m_Rows{ 2 }
    , m_Cols{ 3 }
    , m_ClipHeight{ m_pTexture->GetHeight() / m_Rows }
    , m_ClipWidth{ m_pTexture->GetWidth() / m_Cols }
    , m_IsOpen { false }
    , m_Frame{ 0 }
    , m_Timer{ 0 }
    , m_OpenDuration{ 3 }
{
}

Door::~Door()
{
    delete m_pTexture;
    m_pTexture = nullptr;
}

void Door::Update(float elapsedSec)
{
    if (m_IsOpen)
    {
        m_Timer += elapsedSec;
        if (m_Frame < m_Cols - 1)
        {
            ++m_Frame;
        }
        if (m_Timer >= m_OpenDuration)
        {
            m_Timer = 0;
            m_Frame = 0;
            m_IsOpen = false;
        }
    }
    else if (m_Frame > 0)
    {
        --m_Frame;
    }
}

void Door::Draw() const
{
    Rectf srcRect{ m_Frame * m_ClipWidth, ((int)m_Type + 1) * m_ClipHeight, m_ClipWidth, m_ClipHeight };
    Rectf destRect{ m_Shape.left + 6 * (m_Shape.width / 8), m_Shape.bottom, m_Shape.width / 8, m_Shape.height };

    m_pTexture->Draw(destRect, srcRect);

    destRect.left = 6 * (m_Shape.width / 8);
    destRect.bottom = 0;

    glPushMatrix();

    glTranslatef(m_Shape.left, m_Shape.bottom, 0);
    glTranslatef(m_Shape.width, 0, 0);
    glScalef(-1, 1, 1);
    
    m_pTexture->Draw(destRect, srcRect);

    glPopMatrix();
}

Rectf Door::GetShape() const
{
    return m_Shape;
}

bool Door::IsOpen() const
{
    return m_IsOpen;
}

Door::Type Door::GetType() const
{
    return m_Type;
}

void Door::Open()
{
    m_IsOpen = true;
}
