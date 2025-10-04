#pragma once

class Texture;

class HUD
{
public:
	HUD();
	HUD(const HUD& other) = delete;
	HUD& operator=(const HUD& other) = delete;
	HUD(HUD&& other) = delete;
	HUD& operator=(HUD&& other) = delete;
	~HUD();

	void Update(int energy, int packs, int missiles, bool hasMissiles);
	void DrawLarge(Point2f& location) const;
	void DrawSmall(Point2f& location) const;
	void DrawAmounts(Point2f& location) const;
	void SetEnergy(int energy);
	void SetPacks(int packs);
	void SetMissiles(int missiles, bool hasMissiles);
	
private:
	Texture* m_pHUDTexture, *m_pFontTexture;
	float m_Cols, m_Rows, m_FontCols, m_FontRows;

	int m_Energy;
	int m_Packs;
	int m_Missiles;
	bool m_HasMissiles;
};

