#pragma once

class Scene230224 : public Scene
{
public:
    Scene230224();
    ~Scene230224();
    // Scene��(��) ���� ��ӵ�
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    class Terrain230224* terrain;
    class Car230224* car;

};

