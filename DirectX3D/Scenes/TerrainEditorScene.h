#pragma once
#include "Scene.h"
class TerrainEditorScene : public Scene
{
public:
    TerrainEditorScene();
    ~TerrainEditorScene();
    // Scene��(��) ���� ��ӵ�
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    TerrainEditor* terrainEditor;
};

