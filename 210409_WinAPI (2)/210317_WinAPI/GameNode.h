#pragma once
#include "config.h"
// ��� ����, ��� �Լ� ���� ���� ����

/*
	������ ������� ���ӱ����� ���� �⺻�� �Ǵ� Ŭ����
*/
class GameNode
{
public:
	HRESULT Init();			// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	void Release();			// �޸� ����
	void Update();			// ������ ������ ���� ���� ���� (������ ����)
	void Render(HDC hdc);	// ������ ������ ��� (�̹���, �ؽ�Ʈ ��)

	GameNode();
	~GameNode();
};

