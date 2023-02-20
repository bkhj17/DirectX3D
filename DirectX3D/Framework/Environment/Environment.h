#pragma once

class Environment : public Singleton<Environment>
{
private:
    friend class Singleton;

    Environment();
    ~Environment();

public:
    void SetAlphaBlend();
    void SetAdditiveBlend();

    void SetViewport(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
    void SetProjection();

    //Camera* GetMainCamera() { return mainCamera; }

private:
    void CreateProjection();
    void CreateSamplerState();
    void CreateBlendState();
    void CreateRasterizerState();

private:
    MatrixBuffer* projectionBuffer;

    MatrixBuffer* viewBuffer;

    ID3D11SamplerState* samplerState;
    ID3D11BlendState* alphaBlendState;
    ID3D11BlendState* additiveBlendState;
    ID3D11RasterizerState* rasterizerState;

    //Camera* mainCamera;

    D3D11_VIEWPORT viewport;
};