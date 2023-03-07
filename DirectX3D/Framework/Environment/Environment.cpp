#include "Framework.h"

Environment::Environment()
{
    CreateProjection();
    CreateState();

    mainCamera = new Camera();
    lightBuffer = new LightBuffer;

    uiViewBuffer = new ViewBuffer;
}

Environment::~Environment()
{
    delete projectionBuffer;
    delete mainCamera;

    delete lightBuffer;

    delete samplerState;
    delete raterizerState[0];
    delete raterizerState[1];
    delete blendState[0];
    delete blendState[1];

    delete uiViewBuffer;
}

void Environment::Update()
{
    if (KEY_DOWN(VK_F1))
        isWireMode = !isWireMode;
    if (KEY_DOWN(VK_F2))
        Collider::RenderOnOff();

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
    SetPerspective();

    depthStencilState[0]->SetState();
    raterizerState[(int)isWireMode]->SetState();        

    lightBuffer->SetPS(0);
}

void Environment::PostSet()
{
    depthStencilState[1]->SetState();
    uiViewBuffer->SetVS(1);
    SetOrthographic();
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

void Environment::SetPerspective()
{
    projectionBuffer->Set(perspective);
    projectionBuffer->SetVS(2);
}

void Environment::SetOrthographic()
{
    projectionBuffer->Set(orthographic);
    projectionBuffer->SetVS(2);
}

void Environment::CreateProjection()
{
    orthographic = XMMatrixOrthographicOffCenterLH(0.0f, (float)WIN_WIDTH, 0.0f, (float)WIN_HEIGHT, -1.0f, 1.0f);
    
    perspective = XMMatrixPerspectiveFovLH(XM_PIDIV4, 
       (float)WIN_WIDTH / WIN_HEIGHT, 0.1f, 1000.0f);

    projectionBuffer = new MatrixBuffer();
}

void Environment::CreateState()
{
    samplerState = new SamplerState;
    samplerState->SetState();

    raterizerState[0] = new RasterizerState();
    raterizerState[1] = new RasterizerState();
    raterizerState[1]->FillMode(D3D11_FILL_WIREFRAME);

    blendState[0] = new BlendState();
    blendState[1] = new BlendState();
    blendState[1]->SetState();

    depthStencilState[0] = new DepthStencilState();
    depthStencilState[1] = new DepthStencilState();
    depthStencilState[1]->DepthEnable(false);
}