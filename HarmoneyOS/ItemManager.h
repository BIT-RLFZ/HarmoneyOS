#pragma once
#include "CItemStorageInfo.h"
#include "CPurchaseItemRecord.h"
#include "CPurchaseOrderRecord.h"
#include <string>
#include <vector>

struct OrderYouWannaKnow {
	CPurchaseOrderRecord MaxProfit;
	CPurchaseOrderRecord MinProfit;
	CPurchaseOrderRecord MaxCost;
	CPurchaseOrderRecord MinCost;
	int OrderAmount;
	OrderYouWannaKnow(const CPurchaseOrderRecord &a, const CPurchaseOrderRecord &b,
		              const CPurchaseOrderRecord &c, const CPurchaseOrderRecord &d, const int &e) {
		MaxProfit = a;
		MinProfit = b;
		MaxCost = c;
		MinCost = d;
		OrderAmount = e;
	}
};
class ItemManager
{
public:
	CItemStorageInfo ReceiveAction();

	bool AddItem(const CItemStorageInfo &NewItem);
	bool ModifyItem(const CItemStorageInfo &NewItem);
	
	bool DeleteItem(std :: string ItemId);
	std::vector<CItemStorageInfo>& GetAllItemStorageInfo();
	std::vector<CPurchaseOrderRecord>& GetAllPurchaseOrderRecord();

	double GetProfit(const std::vector<CItemStorageInfo>& ItemList);

	OrderYouWannaKnow GetOrders(const std::vector<CItemStorageInfo>& OrderList);

	bool NewMonthlyRecord(const double& Profit);

	bool CreateNewMonthlyRecord(const int& Timestamp, const double& Profit);


//	bool AddItem(std::string ItemId, std::string ItemName, double WeightRest, int CountRest,);
};

