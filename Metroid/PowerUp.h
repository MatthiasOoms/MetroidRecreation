#pragma once
class Texture;
class Avatar;

class PowerUp
{
public:
	enum class Type
	{
		ball,
		missiles,
		energy,
		bombs,
		varia,
		longbeam,
		icebeam
	};

	PowerUp(Type type, Point2f pos);
	PowerUp(const PowerUp& other) = delete;
	PowerUp& operator=(const PowerUp& other) = delete;
	PowerUp(PowerUp&& other) = delete;
	PowerUp& operator=(PowerUp&& other) = delete;
	~PowerUp();

	void Draw() const;
	void Update(Avatar* player);

protected:
	Type m_Type;
	Rectf m_Shape;
	Texture* m_pTexture;
	float m_Rows, m_Cols;
	float m_ClipHeight, m_ClipWidth;

private:

};

