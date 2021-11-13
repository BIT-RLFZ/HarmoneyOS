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
};

