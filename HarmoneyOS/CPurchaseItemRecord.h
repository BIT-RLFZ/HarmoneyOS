#pragma once
#include "CItemInfo.h"
class CPurchaseItemRecord
{
public:
	CItemInfo Item;
	double Weight; //购买重量
	int Count; //购买个数
	int Timestamp;
	int OrderId; //物品所属订单号
};