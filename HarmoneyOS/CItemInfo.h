#pragma once
#include <string>

class CItemInfo
{
	/*
		��Ԫ���� ���� == �����
		����ʵ��Ӧ���ж� ItemId �͹���
	*/
	friend bool operator==(const CItemInfo& item1, const CItemInfo& item2);
public:
	std::string	ItemName;
	double Cost; //����
	double Price; //�ۼ�
	std::string ItemId;
	int ItemType; //0:�������� 1:����
	/*
		�޲ι���
		Ϊ�����������ͳ�ʼ��
	*/
	CItemInfo();
	/*
		�вι���
		Ϊ������Ա����ʼֵ
	*/
	CItemInfo(const std::string& ItemName, double Cost, double Price, const std::string& ItemId, int ItemType);
};

