#pragma once

class InfoUI : public UIWindow
{
	//ĳ���� ���� �����ϰ� ���
public:
	InfoUI(Vector3 pos);
	~InfoUI() = default;

	void Render() override;

	void Close() override {}	//��� ��� â

	void SetCharacter(Character* character);
private:
	Character* character = nullptr;
};

