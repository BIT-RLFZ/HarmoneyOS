#pragma once
#include "CItemInfo.h"
class CPurchaseItemRecord
{
public:
	CItemInfo Item;
	double Weight; //��������
	int Count; //�������
	int Timestamp;
	int OrderId; //��Ʒ����������
};