#pragma once

class RenderTargetScene : public Scene
{
public:
    RenderTargetScene();
    ~RenderTargetScene();

    // Scene을(를) 통해 상속됨
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    Model* forest;
    Human* human;

    RenderTarget* renderTarget;
    DepthStencil* depthStencil;

    Quad* quad;
    FloatValueBuffer* valueBuffer;
};