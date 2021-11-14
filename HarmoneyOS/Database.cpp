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
    // 头结构的大小
    totSize += sizeof(DatabaseHeader);
    // 约定的固定字符串在字符池中的大小
    totSize += strlen("StringPool") + 1; //\0的位置
    totSize += 1;
    tmpHeader.AItemInfoPoolOffset = tmpHeader.AItemStorageInfoPoolOffset = tmpHeader.APurchaseItemRecordPoolOffset = tmpHeader.APurchaseOrderRecordPoolOffset = totSize;
    tmpHeader.AItemInfoCount = tmpHeader.AItemStorageInfoCount = tmpHeader.APurchaseItemRecordCount = tmpHeader.APurchaseOrderRecordCount = 0;
    char* fileBuf = new char[totSize + 1];

    // 复制文件头
    memcpy_s(fileBuf, sizeof(DatabaseHeader), &tmpHeader, sizeof(DatabaseHeader));
    // 复制字符串池
    memcpy_s(fileBuf + tmpHeader.StringPoolOffset, strlen("StringPool") + 1, "StringPool", strlen("StringPool") + 1);

    // 写到文件内
    remove(dbFileName.c_str()); //确保没有文件存在
    std::ofstream file(dbFileName, std::ios::out | std::ios::binary);
    file.write(fileBuf, totSize + 1);
    file.close();
}

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
    // 全局数据表的清空
    ItemInfoTable.clear();
    cacheAbstractString.clear();
    GlobalString.clear();
    ItemStorageTable.clear();

    curDbFileName = dbFileName;

    // 文件数据的读入
    struct _stat info;
    _stat(dbFileName.c_str(), &info);
    int size = info.st_size;
    if (size < 11) throw HarmoneyException("数据库文件大小错误!");
    gFileData = new char[size];
    std::ifstream file(dbFileName, std::ios::in | std::ios::binary);
    file.read(gFileData, size);
    file.close();

    // 文件数据的解析
    gFileHeader = (DatabaseHeader*)gFileData;
    // 校验文件头合法性
    if (strcmp(gFileHeader->magicChar, "HarmoneyDB") != 0) {
        throw HarmoneyException("数据库文件格式错误!");
    }
    // 字符串池的载入
    gStringPool = new char[gFileHeader->StringPoolLength];
    memcpy_s(gStringPool, gFileHeader->StringPoolLength, (void*)((long long)gFileHeader + gFileHeader->StringPoolOffset), gFileHeader->StringPoolLength);
    // 检验字符串池是否读入正确，规定第一个字符串恒为 StringPool
    AbstractString verifyAS;
    verifyAS.SPOffset = 0;
    verifyAS.length = 11;
    auto vStr = GetAbstractString(verifyAS);
    if (vStr != "StringPool") throw HarmoneyException("数据库字符串池校验错误!");
    
    // AbstractItemInfo 池的载入
    auto AItemInfoBegin = (AbstractItemInfo*)(gFileData + gFileHeader->AItemInfoPoolOffset);
    for (int i = 0; i < gFileHeader->AItemInfoCount; i++) {
        auto pCurItemInfo = AItemInfoBegin + i;
        CItemInfo realIteminfo;
        realIteminfo.ItemId = GetAbstractString(pCurItemInfo->ItemId);
        realIteminfo.ItemName = GetAbstractString(pCurItemInfo->ItemName);
        realIteminfo.Cost = pCurItemInfo->Cost;
        realIteminfo.ItemType = pCurItemInfo->ItemType;
        realIteminfo.Price = pCurItemInfo->Price;
        ItemInfoTable[pCurItemInfo->ItemDatabaseID] = realIteminfo;
    }

    // AbstractItemStorageInfoPool 池的载入
    auto AItemStorageInfoPoolBegin = (AbstractItemStorageInfo*)(gFileData + gFileHeader->AItemStorageInfoPoolOffset);
    for (int i = 0; i < gFileHeader->AItemStorageInfoCount; i++) {
        auto pCurItemStorageInfo = AItemStorageInfoPoolBegin + i;
        CItemStorageInfo realItemStorageInfo;
        if (ItemInfoTable.count(pCurItemStorageInfo->Item) == 0) throw HarmoneyException("无法在ItemInfo表中找到指定ID的Item！");
        realItemStorageInfo.Item = ItemInfoTable[pCurItemStorageInfo->Item];
        realItemStorageInfo.CountRest = pCurItemStorageInfo->CountRest;
        realItemStorageInfo.IsDelete = pCurItemStorageInfo->IsDelete;
        realItemStorageInfo.Timestamp = pCurItemStorageInfo->Timestamp;
        realItemStorageInfo.WeightRest = pCurItemStorageInfo->WeightRest;
        ItemStorageTable.push_back(realItemStorageInfo);
    }
}


bool Database::InitDatabase(std::string dbFileName)
{
    // 防止多次调用内存泄露
    if (gFileData) delete[] gFileData;
    if (gStringPool) delete[] gStringPool;
    gFileData = NULL;
    gStringPool = NULL;
    if (!fileExist(dbFileName)) {
        // 数据库文件不存在，创建默认数据库文件
        GenerateEmptyDBFile(dbFileName);
    }
    LoadBinaryDBFile(dbFileName);
    return true;
}
CItemStorageInfo Database::QueryItemStorageInfo(std::string ItemId)
{
    for (CItemStorageInfo& info : ItemStorageTable) {
        if (info.Item.ItemId == ItemId) return info;
    }
    throw HarmoneyException("无法找到指定ItemId的CItemStorageInfo!");
}

bool Database::ModifyItemStorageInfo(CItemStorageInfo& StorageInfo)
{
    for (CItemStorageInfo& info : ItemStorageTable) {
        if (info.Item.ItemId == StorageInfo.Item.ItemId) {
            info = StorageInfo;
            return true;
        }
    }
    throw HarmoneyException("无法找到指定ItemId的CItemStorageInfo!");
}

bool Database::AddItemStorageInfo(CItemStorageInfo& StorageInfo)
{
    for (CItemStorageInfo& info : ItemStorageTable) {
        if (info.Item.ItemId == StorageInfo.Item.ItemId) {
            // 存在，那么就合并
            info.WeightRest += StorageInfo.WeightRest;
            info.CountRest += StorageInfo.CountRest;
            info.Timestamp = StorageInfo.Timestamp;
            info.Item = StorageInfo.Item;
            return true;
        }
    }
    // 不存在，就直接插入
    ItemStorageTable.push_back(StorageInfo);
    return true;
}

std::vector<CItemStorageInfo>& Database::GetAllItemStorageInfo()
{
    return ItemStorageTable;
}

bool Database::DeleteItemStorageInfo(std::string ItemId)
{
    for (CItemStorageInfo& info : ItemStorageTable) {
        if (info.Item.ItemId == ItemId) {
            info.IsDelete = true;
            return true;
        }
    }
    throw HarmoneyException("无法找到指定ItemId的CItemStorageInfo!");
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

bool Database::UpdateDatabaseFile()
{
    static std::set<std::string> AllStrings;
    static std::map<std::string, AbstractString> abStrMap;
    static std::map<std::string, AbstractItemInfo> abItemInfoMap;
    static std::vector<AbstractItemStorageInfo> abItemStorageInfoTable;
    static int AbstractItemInfoCnt = 0;
    AllStrings.clear();
    abStrMap.clear();
    abItemInfoMap.clear();
    abItemStorageInfoTable.clear();
    AbstractItemInfoCnt = 0;
    for (CItemStorageInfo& info : ItemStorageTable) {
        AllStrings.insert(info.Item.ItemId);
        AllStrings.insert(info.Item.ItemName);
    }
    // 计算字符串池大小
    int stringPoolSize = strlen("StringPool") + 1;
    for (std::set<std::string>::iterator it = AllStrings.begin(); it != AllStrings.end(); it++) {
        AbstractString as;
        as.SPOffset = stringPoolSize;
        as.length = it->size() + 1;
        abStrMap[*it] = as;
        stringPoolSize += (it->size() + 1);
    }

    // 提取所有Item到AbstractItem表
    for (CItemStorageInfo& info : ItemStorageTable) {
        AbstractItemInfo abi;
        abi.ItemDatabaseID = ++AbstractItemInfoCnt;
        abi.Cost = info.Item.Cost;
        abi.ItemId = abStrMap[info.Item.ItemId];
        abi.ItemName = abStrMap[info.Item.ItemName];
        abi.ItemType = info.Item.ItemType;
        abi.Price = info.Item.Price;
        abItemInfoMap[info.Item.ItemId] = abi;
    }

    // 提取所有ItemStorage到AbstractItemStorage表
    for (CItemStorageInfo& info : ItemStorageTable) {
        AbstractItemStorageInfo aisi;
        aisi.CountRest = info.CountRest;
        aisi.IsDelete = info.IsDelete;
        aisi.Item = abItemInfoMap[info.Item.ItemId].ItemDatabaseID;
        aisi.Timestamp = info.Timestamp;
        aisi.WeightRest = info.WeightRest;
        abItemStorageInfoTable.push_back(aisi);
    }

    int AbstractItemInfoPoolSize = abItemInfoMap.size() * sizeof(AbstractItemInfo);
    int AbstractItemStorageInfoPoolSize = abItemStorageInfoTable.size() * sizeof(AbstractItemStorageInfo);
    int AbstractPurchaseItemRecordPoolSize = 0;
    int AbstractPurchaseOrderRecordPoolSize = 0;

    int totDBFileLength = sizeof(DatabaseHeader) + stringPoolSize + AbstractItemInfoPoolSize + AbstractItemStorageInfoPoolSize + AbstractPurchaseItemRecordPoolSize + AbstractPurchaseOrderRecordPoolSize;

    // 开始生成文件数据
    char* fileBuf = new char[totDBFileLength+2];
    // 构造header
    DatabaseHeader header;
    strcpy_s(header.magicChar, "HarmoneyDB");
    header.timestamp = time(0);
    header.StringPoolOffset = sizeof(DatabaseHeader);
    header.StringPoolLength = stringPoolSize;
    header.AItemInfoPoolOffset = header.StringPoolOffset + header.StringPoolLength;
    header.AItemInfoCount = abItemInfoMap.size();
    header.AItemStorageInfoPoolOffset = header.AItemInfoPoolOffset + AbstractItemInfoPoolSize;
    header.AItemStorageInfoCount = abItemStorageInfoTable.size();
    header.APurchaseItemRecordCount = header.APurchaseOrderRecordCount = 0;
    header.APurchaseItemRecordPoolOffset = header.APurchaseOrderRecordPoolOffset = 0;

    // 写入header
    memcpy_s(fileBuf, totDBFileLength, &header, sizeof(header));

    // 写入string
    memcpy_s(fileBuf + header.StringPoolOffset, header.StringPoolLength, "StringPool", strlen("StringPool") + 1);
    for (std::set<std::string>::iterator it = AllStrings.begin(); it != AllStrings.end(); it++) {
        auto as = abStrMap[*it];
        memcpy_s(fileBuf + header.StringPoolOffset + as.SPOffset, header.StringPoolLength, it->c_str(), as.length);
    }

    // 写入AbstractItemInfo
    int nowWrite = 0;
    for (std::map<std::string, AbstractItemInfo>::iterator it = abItemInfoMap.begin(); it != abItemInfoMap.end(); it++) {
        memcpy_s(fileBuf + header.AItemInfoPoolOffset + nowWrite * sizeof(AbstractItemInfo), sizeof(AbstractItemInfo), &(it->second), sizeof(AbstractItemInfo));
        nowWrite++;
    }

    // 写入AbstractStorageItemInfo
    nowWrite = 0;
    for (auto aisi : abItemStorageInfoTable) {
        memcpy_s(fileBuf + header.AItemStorageInfoPoolOffset + nowWrite * sizeof(aisi), sizeof(aisi), &aisi, sizeof(aisi));
        nowWrite++;
    }

    remove(curDbFileName.c_str()); //确保没有文件存在
    std::ofstream file(curDbFileName, std::ios::out | std::ios::binary);
    file.write(fileBuf, totDBFileLength + 2);
    file.close();
    return true;
}


Database* DB = new Database(); // 给全局用