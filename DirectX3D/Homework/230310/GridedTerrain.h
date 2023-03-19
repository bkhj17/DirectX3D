#pragma once

class GridedTerrain : public Terrain
{
public:
	GridedTerrain();
	~GridedTerrain();

	void Update();
	void Render();

private:

	Vector3 CoordToPos(int x, int y);

private:
	FloatValueBuffer* widthHeightBuffer;

	vector<Cube*> cubes;

	UINT tileWidth = 10, tileHeight = 10;
	UINT row, col;

	int selected = -1;
	ColorBuffer* tileColorBuffer;
};

