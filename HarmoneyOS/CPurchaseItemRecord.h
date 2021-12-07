#pragma once
#include "CItemInfo.h"
class CPurchaseItemRecord
{
	/*
		购物商品记录的判重
		重载的 == 运算符
		只要商品一样就认为一样
		所属：冯宇鹏
	*/
	friend bool operator==(const CPurchaseItemRecord &RecordA, const CPurchaseItemRecord& RecordB);
public:
	CItemInfo Item;
	double Weight; //购买重量
	int Count; //购买个数
	int Timestamp;
	int OrderId; //物品所属订单号
	/*
		无参构造函数
		仅初始化基本类型
	*/
	CPurchaseItemRecord();
	/*
		构造函数
		为各个成员赋初值
	*/
	CPurchaseItemRecord(const CItemInfo& Item, double Weight, int Count, int TimeStamp, int OrderId);
};