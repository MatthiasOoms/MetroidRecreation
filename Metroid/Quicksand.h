#pragma once

class Texture;
class Avatar;

class Quicksand
{
public:
	Quicksand(Point2f pos, int width);
	Quicksand(const Quicksand& other) = delete;
	Quicksand& operator=(const Quicksand& other) = delete;
	Quicksand(Quicksand&& other) = delete;
	Quicksand& operator=(Quicksand&& other) = delete;
	~Quicksand();

	void Draw() const;
	void Update(Avatar*& avatar);
private:
	Rectf m_Shape;
	Texture* m_pTexture;
	int m_DstWidth;
};

