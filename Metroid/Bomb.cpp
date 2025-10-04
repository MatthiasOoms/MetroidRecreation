#pragma once
#include "pch.h"
#include "Bomb.h"
#include "Texture.h"

Bomb::Bomb(Rectf player)
    : m_Life{}
    , m_Lifespan{ 2 }
    , m_Shape{ player.left + player.width / 4, player.bottom, 8, 8 }
    , m_Explosion{ Circlef(player.left, player.bottom, 20) }

    , m_pTexture{ new Texture("Resources/Bomb.png")}
    , m_TextRows{ 2 }
    , m_TextCols{ 4 }
    , m_AnimFrame{}
    , m_AnimTime{}
    , m_ClipHeight{ m_pTexture->GetHeight() / m_TextRows }
    , m_ClipWidth{ m_pTexture->GetWidth() / m_TextCols }
    , m_NrFramesPerSec{ 4 }
    , m_NrOfFrames{ 4 }

    , m_DeathFrame{}
{
}

Bomb::~Bomb()
{
    if (m_pTexture != nullptr)
    {
        delete m_pTexture;
        m_pTexture = nullptr;
    }
}

void Bomb::Update(float elapsedSec)
{
    if (IsDetonated())
    {
        ++m_DeathFrame;
    }
    else
    {
        m_AnimTime += elapsedSec;
        m_Life += elapsedSec;
    }

    FrameHandling(elapsedSec);
}

void Bomb::Draw() const
{
    Rectf srcRect{ m_AnimFrame * m_ClipWidth, m_ClipHeight, m_ClipWidth, m_ClipHeight }
    , destRect{ m_Shape.left + m_Shape.width / 2 - m_ClipWidth / 2, m_Shape.bottom + m_Shape.height / 2 - m_ClipHeight / 2, m_ClipWidth, m_ClipHeight };

    if (IsDetonated())
    {
        srcRect.left = (float)m_DeathFrame * m_ClipWidth;
        srcRect.bottom = m_pTexture->GetHeight();
    }

    m_pTexture->Draw(destRect, srcRect);
}

bool Bomb::IsDetonated() const
{
    if (m_Life >= m_Lifespan)
    {
        return true;
    }
    return false;
}

Circlef Bomb::GetCircle() const
{
    return m_Explosion;
}

int Bomb::GetFrame() const
{
    return m_DeathFrame;
}

int Bomb::GetFrameCount() const
{
    return m_NrOfFrames;
}

void Bomb::FrameHandling(float elapsedSec)
{
    m_AnimTime += m_NrFramesPerSec * elapsedSec;
    m_AnimFrame = (int)m_AnimTime % m_NrOfFrames;
}
