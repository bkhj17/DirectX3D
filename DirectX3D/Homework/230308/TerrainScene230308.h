#pragma once

class TerrainScene230308 : public Scene
{
public:
	TerrainScene230308();
	~TerrainScene230308();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void GetHeightFromTerrain(void* transformPtr, bool updateNormal = true);

private:
	Terrain* terrain;
};

