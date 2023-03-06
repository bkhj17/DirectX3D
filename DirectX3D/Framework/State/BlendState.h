#pragma once
class BlendState
{
public:
	BlendState();
	~BlendState();

	void SetState();

	void Alpha(bool value);
	void Additive();
	void AlphaToCoverage(bool value);
	//3D������ ���� ������ �ƴ� ���̸� ���� �������� �Ѵ� => ��ǻ�Ͱ� �˾Ƽ� �ϱ� �����

	void Changed();

private:
	D3D11_BLEND_DESC desc = {};
	ID3D11BlendState* state = nullptr;
};

