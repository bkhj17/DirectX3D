#include "Framework.h"

Environment::Environment()
{
    CreateProjection();
    CreateState();
    CreateBlendState();

    mainCamera = new Camera();
    lightBuffer = new LightBuffer;
}

Environment::~Environment()
{
    delete projectionBuffer;
    delete mainCamera;

    delete lightBuffer;

    delete samplerState;
    delete raterizerState[0];
    delete raterizerState[1];
}

void Environment::Update()
{
    if (KEY_DOWN(VK_F1))
        isWireMode = !isWireMode;

    mainCamera->Update();
}

void Environment::RenderUI()
{
    if (ImGui::TreeNode("Environment")) {
        mainCamera->RenderUI();
        ImGui::Text("LightOption");
        for (UINT i = 0; i < lightBuffer->Get().lightCount; i++) {
            string name = "Light_"+to_string(i);
            if (ImGui::TreeNode(name.c_str())) {
                ImGui::ColorEdit3("LightColor", (float*)&lightBuffer->Get().lights[i].color, ImGuiColorEditFlags_PickerHueWheel);
                ImGui::SliderFloat3("LightDir", (float*)&lightBuffer->Get().lights[i].direction, -1, 1);

                ImGui::TreePop();
            }
        }

        ImGui::ColorEdit3("AmbientLight", (float*)&lightBuffer->Get().ambientLight, ImGuiColorEditFlags_PickerHueWheel);
        ImGui::ColorEdit3("AmbientCeil", (float*)&lightBuffer->Get().ambientCeil, ImGuiColorEditFlags_PickerHueWheel);
        ImGui::TreePop();
    }
}

void Environment::Set()
{
    SetViewport();
    SetProjection();

    raterizerState[(int)isWireMode]->SetState();        

    lightBuffer->SetPS(0);
}

void Environment::SetViewport(UINT width, UINT height)
{
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    DC->RSSetViewports(1, &viewport);
}

void Environment::SetProjection()
{
    projectionBuffer->SetVS(2);
}

void Environment::SetOrthoProjection()
{
    orthoBuffer->SetVS(2);
}

void Environment::CreateProjection()
{
    projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, 
        (float)WIN_WIDTH / WIN_HEIGHT, 0.1f, 1000.0f);

    projectionBuffer = new MatrixBuffer();
    projectionBuffer->Set(projection);

    Matrix orthographic = XMMatrixOrthographicOffCenterLH(0.0f, WIN_WIDTH, 0.0f, WIN_HEIGHT, -1.0f, 1.0f);
    orthoBuffer = new MatrixBuffer();
    orthoBuffer->Set(orthographic);    
}

void Environment::CreateState()
{
    samplerState = new SamplerState;
    samplerState->SetState();

    raterizerState[0] = new RasterizerState();
    raterizerState[1] = new RasterizerState();
    raterizerState[1]->FillMode(D3D11_FILL_WIREFRAME);
}

void Environment::CreateBlendState()
{
    D3D11_BLEND_DESC blendDesc = {};    
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    DEVICE->CreateBlendState(&blendDesc, &alphaBlendState);

    float blendFactor[4] = {};
    DC->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);

    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

    DEVICE->CreateBlendState(&blendDesc, &additiveBlendState);
}
