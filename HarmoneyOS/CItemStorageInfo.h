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
};

