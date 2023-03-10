#pragma once

class GridedTerrain : public Terrain
{
public:
	GridedTerrain();
	~GridedTerrain();

	void Render();

private:
	FloatValueBuffer* widthHeightBuffer;

	vector<Cube*> cubes;


	UINT tileWidth = 10, tileHeight = 10;
	UINT row, col;
};

