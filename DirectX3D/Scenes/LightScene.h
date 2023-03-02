#pragma once

class LightScene :  public Scene
{
public:
	LightScene();
	~LightScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	Sphere* sphere;
};

