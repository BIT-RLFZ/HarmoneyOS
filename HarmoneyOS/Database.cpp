#include "Database.h"
#include "NoImplException.h";
/*
    ���ݿ��ļ��ṹ: designed by RBH

    === Header ===

    === StringPool ===

    === AbstractItemInfoPool ===

    === AbstractItemStorageInfoPool ===

    === AbstractPurchaseItemRecordPool ===

    === AbstractPurchaseOrderRecord ===

*/
struct DatabaseHeader {
    char magicChar[11]; //HarmoneyDB
    int timestamp; //���ݿ�������ʱ��
    int StringPoolOffset; //�ַ����ص��ļ�ƫ��
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
    int ItemDatabaseID; // �����ݿ�洢ʱ�����UID
    AbstractString ItemName; // Get From StringPool
    AbstractString ItemId; // Get From StringPool
    double Cost;
    double Price;
    int ItemType;
};
struct AbstractItemStorageInfo {
    int Item; // ��ItemInfo���ҵ�ItemDatabaseIDΪItem����Ŀ��Ϊ����
    double WeightRest;
    int CountRest;
    int Timestamp; //¼��ʱ��
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
