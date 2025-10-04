#pragma once
#include "pch.h"
#include "Enemy.h"
#include "Level.h"
#include "Texture.h"
#include "Tile.h"
#include <iostream>

Enemy::Enemy()
	: m_Shape{}
	, m_Velocity{}
	, m_Health{ 5 }
	, m_pTexture{}
	, m_TextRows{}
	, m_TextCols{}
	, m_ClipHeight{}
	, m_ClipWidth{}
	, m_NrOfFrames{}
	, m_NrFramesPerSec{}
	, m_AnimTime{}
	, m_AnimFrame{}
	, m_IsFrozen{ false }
{
}

void Enemy::FrameHandling(float elapsedSec)
{
	m_AnimTime += m_NrFramesPerSec * elapsedSec;
	m_AnimFrame = (int)m_AnimTime % m_NrOfFrames;
}

void Enemy::Damage()
{
	--m_Health;
}

Rectf Enemy::GetShape() const
{
	return m_Shape;
}

Vector2f Enemy::GetVelocity() const
{
	return m_Velocity;
}

int Enemy::GetHealth() const
{
	return m_Health;
}

bool Enemy::IsFrozen() const
{
	return m_IsFrozen;
}

void Enemy::Freeze()
{
	m_IsFrozen = true;
}

Enemy::~Enemy()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}
