#include "Database.h"
#include "NoImplException.h";
CItemStorageInfo Database::QueryItemStorageInfo(std::string ItemId)
{
    throw NoImplException(__FUNCTION__);
}

bool Database::ModifyItemStorageInfo(CItemStorageInfo& StorageInfo)
{
    throw NoImplException(__FUNCTION__);
}

bool Database::AddItemStorageInfo(CItemStorageInfo& StorageInfo)
{
    throw NoImplException(__FUNCTION__);
}

std::vector<CItemStorageInfo>& Database::GetAllItemStorageInfo()
{
    throw NoImplException(__FUNCTION__);
}

bool Database::DeleteItemStorageInfo(CItemStorageInfo& StorageInfo)
{
    throw NoImplException(__FUNCTION__);
}

bool Database::AddPurchaseItemRecord(CPurchaseItemRecord& PurchaseItemRecord)
{
    throw NoImplException(__FUNCTION__);
}

std::vector<CPurchaseItemRecord>& Database::GetAllPurchaseItemRecord()
{
    throw NoImplException(__FUNCTION__);
}

CPurchaseOrderRecord Database::QueryPurchaseOrderRecord(int OrderId)
{
    throw NoImplException(__FUNCTION__);
}

bool Database::AddPurchaseOrderRecord(CPurchaseOrderRecord& PurchaseRecord)
{
    throw NoImplException(__FUNCTION__);
}

int Database::GetLatestOrderId()
{
    throw NoImplException(__FUNCTION__);
}

std::vector<CPurchaseOrderRecord>& Database::GetAllPurchaseOrderRecord()
{
    throw NoImplException(__FUNCTION__);
}
