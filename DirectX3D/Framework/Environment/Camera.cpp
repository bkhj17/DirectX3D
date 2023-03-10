#include "Framework.h"

Camera::Camera()
{
    tag = "Camera";

    viewBuffer = new ViewBuffer();
    viewBuffer->SetVS(1);

    Load();

    prevMousePos = mousePos;
}

Camera::~Camera()
{
    delete viewBuffer;

    Save();
}

void Camera::Update()
{
    projection = Environment::Get()->GetProjection();
    Frustum();

    if (target)
        FollowMode();
    else
        FreeMode();
    if (Rot().x > 360.0f)
        Rot().x -= 360.0f;    
    if (Rot().x < -360.0f)
        Rot().x += 360.0f;
    if (Rot().y > 360.0f)
        Rot().y -= 360.0f;
    if (Rot().y < -360.0f)
        Rot().y += 360.0f;
    
    SetView();
}

void Camera::GUIRender()
{
    if (ImGui::TreeNode("CameraOption")) {
        ImGui::DragFloat("MoveSpeed", &moveSpeed);
        ImGui::DragFloat("RotSpeed", &rotSpeed);

        if (target && ImGui::TreeNode("TargetOption")) {
            ImGui::DragFloat("Distance", &distance, 0.1f);
            ImGui::DragFloat("Height", &height, 0.1f);
            ImGui::DragFloat3("FocusOffset", (float*)&focusOffset, 0.1f);

            float degree = XMConvertToDegrees(rotY);
            ImGui::DragFloat("RotY", &degree, 0.1f);
            rotY = XMConvertToRadians(degree);

            ImGui::DragFloat("MoveDamping", &moveDamping, 0.1f);
            ImGui::DragFloat("RotDamping", &rotDamping, 0.1f);

            ImGui::InputText("File", file, 128);

            if (ImGui::Button("save")) 
                TargetOptionSave(file);
            if (ImGui::Button("load")) 
                TargetOptionLoad(file);

            ImGui::TreePop();
        }
        __super::GUIRender();
        ImGui::TreePop();
    }
}

void Camera::SetView()
{
    UpdateWorld();

    view = XMMatrixInverse(nullptr, world);

    viewBuffer->Set(view, world);
    viewBuffer->SetVS(1);
}

Vector3 Camera::ScreenToWorld(Vector3 pos)
{
    return XMVector3TransformCoord(pos, world);
}

Vector3 Camera::WorldToScreen(Vector3 pos)
{
    Vector3 result = XMVector3TransformCoord(pos, view);
    result = XMVector3TransformCoord(result, projection);
    Vector3 screen;
    screen.x = (result.x + 1) * 0.5f * WIN_WIDTH;
    screen.y = (result.y + 1) * 0.5f * WIN_HEIGHT;
    
    return screen;
}

Ray Camera::ScreenPointToRay(Vector3 screenPoint)
{
    Vector3 screenSize(WIN_WIDTH, WIN_HEIGHT, 1.0f);

    Vector2 point;
    point.x = (screenPoint.x / screenSize.x) * 2.0f - 1.0f;
    point.y = (screenPoint.y / screenSize.y) * 2.0f - 1.0f;

    Float4x4 temp;
    XMStoreFloat4x4(&temp, projection);
    screenPoint.x = point.x / temp._11;
    screenPoint.y = point.y / temp._22;
    screenPoint.z = 1.0f;

    screenPoint = XMVector3TransformNormal(screenPoint, world);

    Ray ray;
    ray.pos = Pos();
    ray.dir = screenPoint.GetNormalized();

    return ray;
}

void Camera::FreeMode()
{
    Vector3 delta = mousePos - prevMousePos;
    prevMousePos = mousePos;

    if (KEY_PRESS(VK_RBUTTON))
    {
        if (KEY_PRESS('W'))
            Pos() += Forward() * moveSpeed * DELTA;
        if (KEY_PRESS('S'))
            Pos() += Back() * moveSpeed * DELTA;
        if (KEY_PRESS('A'))
            Pos() += Left() * moveSpeed *DELTA;
        if (KEY_PRESS('D'))
            Pos() += Right() * moveSpeed * DELTA;
        if (KEY_PRESS('Q'))
            Pos() += Down() * moveSpeed * DELTA;
        if (KEY_PRESS('E'))
            Pos() += Up() * moveSpeed * DELTA;
        Rot().x -= delta.y * rotSpeed * DELTA;
        Rot().y += delta.x * rotSpeed * DELTA;
    }
}

void Camera::FollowMode()
{
    destRot = Lerp(destRot, target->Rot().y, rotDamping * DELTA);
    rotMatrix = XMMatrixRotationY(destRot + rotY);

    Vector3 forward = XMVector3TransformNormal(Vector3::Forward(), rotMatrix);

    destPos = target->GlobalPos() + forward * -distance;
    destPos.y += height;

    Pos() = Lerp(Pos(), destPos, moveDamping * DELTA);

    if (isLookAtTarget)
    {
        Vector3 offset = XMVector3TransformCoord(focusOffset, rotMatrix);
        Vector3 targetPos = target->GlobalPos() + offset;

        Vector3 dir = (targetPos - Pos()).GetNormalized();
        forward = Vector3(dir.x, 0.0f, dir.z).GetNormalized();

        Rot().x = acos(Dot(forward, dir));
        Rot().y = atan2(dir.x, dir.z);
    }
}

void Camera::Frustum()
{
    Float4x4 VP;
    XMStoreFloat4x4(&VP, view * projection);

    //Left
    a = VP._14 + VP._11;
    b = VP._24 + VP._21;
    c = VP._34 + VP._31;
    d = VP._44 + VP._41;
    planes[0] = XMVectorSet(a, b, c, d);

    //Right
    a = VP._14 - VP._11;
    b = VP._24 - VP._21;
    c = VP._34 - VP._31;
    d = VP._44 - VP._41;
    planes[1] = XMVectorSet(a, b, c, d);


    //Bottom
    a = VP._14 + VP._12;
    b = VP._24 + VP._22;
    c = VP._34 + VP._32;
    d = VP._44 + VP._42;
    planes[2] = XMVectorSet(a, b, c, d);

    //Top
    a = VP._14 - VP._12;
    b = VP._24 - VP._22;
    c = VP._34 - VP._32;
    d = VP._44 - VP._42;
    planes[3] = XMVectorSet(a, b, c, d);

    //Near
    a = VP._14 + VP._13;
    b = VP._24 + VP._23;
    c = VP._34 + VP._33;
    d = VP._44 + VP._43;
    planes[4] = XMVectorSet(a, b, c, d);

    //Far
    a = VP._14 - VP._13;
    b = VP._24 - VP._23;
    c = VP._34 - VP._33;
    d = VP._44 - VP._43;
    planes[5] = XMVectorSet(a, b, c, d);


    for (int i = 0; i < 6; i++) {
        planes[i] = XMPlaneNormalize(planes[i]);
    }
}

void Camera::TargetOptionSave(string file)
{
    string path = "TextData/Camera/" + file + ".cam";
    BinaryWriter* writer = new BinaryWriter(path);
    writer->Float(distance);
    writer->Float(height);
    writer->Float(moveDamping);
    writer->Float(rotDamping);
    writer->Float(rotY);
    writer->Vector(focusOffset);
    writer->Bool(isLookAtTarget);
    delete writer;
}

void Camera::TargetOptionLoad(string file)
{
    string path = "TextData/Camera/" + file + ".cam";
    BinaryReader* reader = new BinaryReader(path);
    distance = reader->Float();
    height = reader->Float();
    moveDamping = reader->Float();
    rotDamping = reader->Float();
    rotY = reader->Float();
    focusOffset = reader->Vector();
    isLookAtTarget = reader->Bool();
    delete reader;
}

bool Camera::ContainPoint(Vector3 point)
{
    for (int i = 0; i < 6; i++) {
        Vector3 dot = XMPlaneDotCoord(planes[i], point);
        if (dot.x < 0.0f)
            return false;
    }
    return true;
}
