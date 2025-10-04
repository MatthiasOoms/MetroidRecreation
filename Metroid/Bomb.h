#pragma once

class Texture;

class Bomb
{
public:
	Bomb(Rectf player);
	Bomb(const Bomb& other) = delete;
	Bomb& operator=(const Bomb& other) = delete;
	Bomb(Bomb&& other) = delete;
	Bomb& operator=(Bomb&& other) = delete;
	~Bomb();

	void Update(float elapsedSec);
	void Draw() const;
	bool IsDetonated() const;
	Circlef GetCircle() const;
	int GetFrame() const;
	int GetFrameCount() const;

private:

	// Animation
	Texture* m_pTexture;
	float m_TextRows;
	float m_TextCols;
	float m_ClipHeight;
	float m_ClipWidth;
	int m_NrOfFrames;
	int m_NrFramesPerSec;
	float m_AnimTime;
	int m_AnimFrame;

	int m_DeathFrame;

	// General
	Rectf m_Shape;
	Circlef m_Explosion;
	float m_Lifespan;
	float m_Life;

	void FrameHandling(float elapsedSec);
};

