#pragma once
class Reflection
{
public:
	Reflection(Transform* target);
	~Reflection();

	void Update();

	void SetReflection();	//PreRender
	void SetRender();		//Render
	void PostRender();

private:
	Transform* target;			//�ݻ��� ��ü
	RenderTarget* renderTarget;	//��ĥ ȭ��
	DepthStencil* depthStencil;	//ȭ�� ����

	Camera* camera;				//�ݻ� ���� ī�޶�

	class Quad* quad;			
};