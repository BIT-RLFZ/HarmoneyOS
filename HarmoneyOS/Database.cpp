#include "Database.h"
#include "NoImplException.h";
/*
    数据库文件结构: designed by RBH

    === Header ===

    === StringPool ===

    === AbstractItemInfoPool ===

    === AbstractItemStorageInfoPool ===

    === AbstractPurchaseItemRecordPool ===

    === AbstractPurchaseOrderRecord ===

*/
struct DatabaseHeader {
    char magicChar[11]; //HarmoneyDB
    int timestamp; //数据库最后更新时间
    int StringPoolOffset; //字符串池的文件偏移
    int AItemInfoPoolOffset;
    int AItemInfoCount;
    int AItemStorageInfoPoolOffset;
    int AItemStorageInfoCount;
    int APurchaseItemRecordPoolOffset;
    int APurchaseItemRecordCount;
    int APurchaseOrderRecordOffset;
    int APurchaseOrderRecordCount;
}; 
struct AbstractString {
    int SPOffset; // StringPoolOffset
    int length; // string length
};
struct AbstractItemInfo {
    int ItemDatabaseID; // 在数据库存储时分配的UID
    AbstractString ItemName; // Get From StringPool
    AbstractString ItemId; // Get From StringPool
    double Cost;
    double Price;
    int ItemType;
};
struct AbstractItemStorageInfo {
    int Item; // 查ItemInfo表，找到ItemDatabaseID为Item的项目即为此项
    double WeightRest;
    int CountRest;
    int Timestamp; //录入时间
    bool IsDelete;
};

inline bool fileExist(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

bool Database::InitDatabase(std::string dbFileName)
{

    throw NoImplException(__FUNCTION__);
}
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

bool Database::DeleteItemStorageInfo(std::string ItemId)
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
