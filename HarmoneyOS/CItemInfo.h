#pragma once
#include <string>

class CItemInfo
{
	/*
		友元函数 重载 == 运算符
		具体实现应该判断 ItemId 就够了
	*/
	friend bool operator==(const CItemInfo& item1, const CItemInfo& item2);
public:
	std::string	ItemName;
	double Cost; //进价
	double Price; //售价
	std::string ItemId;
	int ItemType; //0:按个数卖 1:称重
	/*
		无参构造
		为基本数据类型初始化
	*/
	CItemInfo();
	/*
		有参构造
		为各个成员赋初始值
	*/
	CItemInfo(const std::string& ItemName, double Cost, double Price, const std::string& ItemId, int ItemType);
};

