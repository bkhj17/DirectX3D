#pragma once

class TrailEffectScene : public Scene
{
public:
	TrailEffectScene();
	~TrailEffectScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};

