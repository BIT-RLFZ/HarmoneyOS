#pragma once
#include "CItemInfo.h"

class CItemStorageInfo
{
public:
	CItemInfo Item;
	double WeightRest;
	int CountRest;
	int Timestamp; //录入时间
	bool IsDelete;
	/*
		无参构造
		将基本数据类型赋为默认值
	*/
	CItemStorageInfo();
	/*
		有参构造
		将所有成员赋初值
	*/
	CItemStorageInfo(const CItemInfo& Item, double WeightRest, int CountRest, int Timestamp, bool IsDelete);
};

