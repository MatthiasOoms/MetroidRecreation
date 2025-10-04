#pragma once
#include "Vector2f.h"
#include <vector>

class Level;
class Texture;
class Tile;

class Enemy
{
public:
	Enemy();
	Enemy(const Enemy& other) = delete;
	Enemy& operator=(const Enemy& other) = delete;
	Enemy(Enemy&& other) = delete;
	Enemy& operator=(Enemy&& other) = delete;
	virtual ~Enemy();

	virtual void Update(float elapsedSec, const Level* level, const std::vector<Tile*>& tiles) = 0;
	virtual void Draw() const = 0;
	void Damage();
	Rectf GetShape() const;
	Vector2f GetVelocity() const;
	int GetHealth() const;
	bool IsFrozen() const;
	void Freeze();

protected:
	Rectf m_Shape;
	Vector2f m_Velocity;
	int m_Health;
	bool m_IsFrozen;

	Texture* m_pTexture;
	int m_TextRows;
	int m_TextCols;
	float m_ClipHeight;
	float m_ClipWidth;
	int m_NrOfFrames;
	int m_NrFramesPerSec;
	float m_AnimTime;
	int m_AnimFrame;

	void FrameHandling(float elapsedSec);

private:

};

