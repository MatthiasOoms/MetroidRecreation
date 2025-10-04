#pragma once
class Texture;
class Avatar;

class PickUp
{
public:
	enum class Type
	{
		nothing,
		energySmall,
		energyLarge,
		missile
	};

	PickUp(Point2f pos, bool hasMissiles);
	PickUp(const PickUp& other) = delete;
	PickUp& operator=(const PickUp& other) = delete;
	PickUp(PickUp&& other) = delete;
	PickUp& operator=(PickUp&& other) = delete;
	~PickUp();

	void Draw() const;
	void Update(Avatar* player, float elapsedSec);
	Type GetType() const;

protected:
	Type m_Type;
	Rectf m_Shape;
	Texture* m_pTexture;
	int m_Frame;
	float m_AccuSec;
	int m_Rows, m_Cols;
	int m_FramesPerSec;
	float m_ClipHeight, m_ClipWidth;

private:

};

