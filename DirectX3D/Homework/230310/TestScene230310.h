#pragma once

class TestScene230310 : public Scene
{
public:
	TestScene230310();
	~TestScene230310();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
private:
	void CharacterMoveEnd(void* characterPtr);
private:
	class GridedTerrain* terrain;
	class MapCursor* cursor;
	class Character* testCharacter;
};

