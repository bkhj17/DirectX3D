#pragma once

class Character : public Transform
{
public:
	Character();
	~Character();

	void Update();
	void Render();

private:
	Cube* cube;	//��ġ �׽�Ʈ ��. ���� �� �ִϸ����ͷ� ����
};

