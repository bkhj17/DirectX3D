#pragma once
#include "Scene.h"
class CollisionScene :
    public Scene
{
public:
    CollisionScene();
    ~CollisionScene();
    // Scene��(��) ���� ��ӵ�
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    vector<Collider*> colliders;
};

