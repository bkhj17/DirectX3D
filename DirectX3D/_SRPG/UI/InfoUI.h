#pragma once
#include "UIWindow.h"
class InfoUI : public UIWindow
{
	//ĳ���� ���� �����ϰ� ���
public:
	InfoUI(Vector3 pos);
	~InfoUI();

	void Render() override;
private:

};

