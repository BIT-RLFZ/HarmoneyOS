#pragma once
#include "CItemInfo.h"

class CItemStorageInfo
{
public:
	CItemInfo Item;
	double WeightRest;
	int CountRest;
	int Timestamp; //¼��ʱ��
	bool IsDelete;
	/*
		�޲ι���
		�������������͸�ΪĬ��ֵ
	*/
	CItemStorageInfo();
	/*
		�вι���
		�����г�Ա����ֵ
	*/
	CItemStorageInfo(const CItemInfo& Item, double WeightRest, int CountRest, int Timestamp, bool IsDelete);
};

