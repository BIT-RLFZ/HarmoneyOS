#pragma once
#include <string>

class CItemInfo
{
public:
	std::string	ItemName;
	double Cost; //进价
	double Price; //售价
	std::string ItemId;
	int ItemType; //0:按个数卖 1:称重
};

