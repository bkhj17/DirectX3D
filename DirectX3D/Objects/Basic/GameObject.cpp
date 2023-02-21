#include "Framework.h"

GameObject::GameObject(wstring shaderFile)
{
    SetShader(shaderFile);

    worldBuffer = new MatrixBuffer();
}

GameObject::~GameObject()
{ 
    delete worldBuffer;
}

void GameObject::SetRender()
{   
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);    

    vertexShader->Set();
    pixelShader->Set();    
}

void GameObject::SetShader(wstring shaderFile)
{
    pixelShader = Shader::AddPS(shaderFile);
    vertexShader = Shader::AddVS(shaderFile);
}