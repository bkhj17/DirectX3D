#pragma once
#include "Scene.h"
class CubeScene :
    public Scene
{
public:
    CubeScene();
    ~CubeScene();
    // Scene��(��) ���� ��ӵ�
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    Cube* cube;
};

