#pragma once
#include "CItemInfo.h"
class CPurchaseItemRecord
{
	/*
		������Ʒ��¼������
		���ص� == �����
		ֻҪ��Ʒһ������Ϊһ��
		������������
	*/
	friend bool operator==(const CPurchaseItemRecord &RecordA, const CPurchaseItemRecord& RecordB);
public:
	CItemInfo Item;
	double Weight; //��������
	int Count; //�������
	int Timestamp;
	int OrderId; //��Ʒ����������
	/*
		�޲ι��캯��
		����ʼ����������
	*/
	CPurchaseItemRecord();
	/*
		���캯��
		Ϊ������Ա����ֵ
	*/
	CPurchaseItemRecord(const CItemInfo& Item, double Weight, int Count, int TimeStamp, int OrderId);
};