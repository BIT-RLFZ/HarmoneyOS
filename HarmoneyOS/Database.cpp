#include "Database.h"


inline bool fileExist(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

void Database::GenerateEmptyDBFile(std::string dbFileName) {
    int totSize = 0;
    DatabaseHeader tmpHeader;
    strcpy_s(tmpHeader.magicChar,"HarmoneyDB");
    tmpHeader.timestamp = time(0);
    tmpHeader.StringPoolOffset = sizeof(DatabaseHeader);
    tmpHeader.StringPoolLength = strlen("StringPool") + 1;
    // ͷ�ṹ�Ĵ�С
    totSize += sizeof(DatabaseHeader);
    // Լ���Ĺ̶��ַ������ַ����еĴ�С
    totSize += strlen("StringPool") + 1; //\0��λ��
    totSize += 1;
    tmpHeader.AItemInfoPoolOffset = tmpHeader.AItemStorageInfoPoolOffset = tmpHeader.APurchaseItemRecordPoolOffset = tmpHeader.APurchaseOrderRecordOffset = totSize;
    tmpHeader.AItemInfoCount = tmpHeader.AItemStorageInfoCount = tmpHeader.APurchaseItemRecordCount = tmpHeader.APurchaseOrderRecordCount = 0;
    char* fileBuf = new char[totSize + 1];

    // �����ļ�ͷ
    memcpy_s(fileBuf, sizeof(DatabaseHeader), &tmpHeader, sizeof(DatabaseHeader));
    // �����ַ�����
    memcpy_s(fileBuf + tmpHeader.StringPoolOffset, strlen("StringPool") + 1, "StringPool", strlen("StringPool") + 1);

    // д���ļ���
    remove(dbFileName.c_str()); //ȷ��û���ļ�����
    std::ofstream file(dbFileName, std::ios::out | std::ios::binary);
    file.write(fileBuf, totSize + 1);
    file.close();
}

std::map<int, std::string> cacheAbstractString;

std::string Database::GetAbstractString(const AbstractString &as) {
    if (cacheAbstractString.count(as.SPOffset)) return cacheAbstractString[as.SPOffset];
    char* buf = new char[as.length];
    memcpy_s(buf, as.length, gStringPool + as.SPOffset, as.length);
    std::string ret = buf;
    delete[] buf;
    cacheAbstractString[as.SPOffset] = ret;
    return ret;
}

void Database::LoadBinaryDBFile(std::string dbFileName) {
    // �ļ����ݵĶ���
    struct _stat info;
    _stat(dbFileName.c_str(), &info);
    int size = info.st_size;
    if (size < 11) throw HarmoneyException("���ݿ��ļ���С����!");
    gFileData = new char[size];
    std::ifstream file(dbFileName, std::ios::in | std::ios::binary);
    file.read(gFileData, size);
    file.close();

    // �ļ����ݵĽ���
    gFileHeader = (DatabaseHeader*)gFileData;
    // У���ļ�ͷ�Ϸ���
    if (strcmp(gFileHeader->magicChar, "HarmoneyDB") != 0) {
        throw HarmoneyException("���ݿ��ļ���ʽ����!");
    }
    // �ַ����ص�����
    gStringPool = new char[gFileHeader->StringPoolLength];
    memcpy_s(gStringPool, gFileHeader->StringPoolLength, (void*)((long long)gFileHeader + gFileHeader->StringPoolOffset), gFileHeader->StringPoolLength);
    // �����ַ������Ƿ������ȷ���涨��һ���ַ�����Ϊ StringPool
    AbstractString verifyAS;
    verifyAS.SPOffset = 0;
    verifyAS.length = 11;
    auto vStr = GetAbstractString(verifyAS);
    if (vStr != "StringPool") throw HarmoneyException("���ݿ��ַ�����У�����!");

}

bool Database::InitDatabase(std::string dbFileName)
{
    // ��ֹ��ε����ڴ�й¶
    if (gFileData) delete[] gFileData;
    if (gStringPool) delete[] gStringPool;
    gFileData = NULL;
    gStringPool = NULL;
    if (!fileExist(dbFileName)) {
        // ���ݿ��ļ������ڣ�����Ĭ�����ݿ��ļ�
        GenerateEmptyDBFile(dbFileName);
    }
    LoadBinaryDBFile(dbFileName);
    return true;
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
