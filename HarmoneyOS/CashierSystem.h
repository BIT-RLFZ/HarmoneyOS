#pragma once
#include "CPurchaseItemRecord.h"
#include <string>
#include <vector>
class CashierSystem
{
public:
	bool AddItemToCart(std::string ItemProcessedId,int cnt);
	bool RemoveItemFromCart(std::string ItemProcessedId);
	double GetCurrentPrice();
	std::vector<CPurchaseItemRecord>& GetCurrentPurchaseList();
	bool Checkout(std::vector<CPurchaseItemRecord>& CurrentPurchaseList);
};

