#pragma once
#include "CItemStorageInfo.h"
#include "CPurchaseItemRecord.h"
#include "CPurchaseOrderRecord.h"
#include <string>
#include <vector>
class Database
{
public:
	CItemStorageInfo QueryItemStorageInfo(std::string ItemId);
	bool ModifyItemStorageInfo(CItemStorageInfo& StorageInfo);
	bool AddItemStorageInfo(CItemStorageInfo& StorageInfo);
	std::vector<CItemStorageInfo>& GetAllItemStorageInfo();
	bool DeleteItemStorageInfo(CItemStorageInfo& StorageInfo);

	
	bool AddPurchaseItemRecord(CPurchaseItemRecord& PurchaseItemRecord);
	std::vector<CPurchaseItemRecord>& GetAllPurchaseItemRecord();

	CPurchaseOrderRecord QueryPurchaseOrderRecord(int OrderId);
	bool AddPurchaseOrderRecord(CPurchaseOrderRecord& PurchaseRecord);
	std::vector<CPurchaseOrderRecord>& GetAllPurchaseOrderRecord();
};

