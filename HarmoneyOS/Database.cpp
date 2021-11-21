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
    tmpHeader.AItemInfoPoolOffset = tmpHeader.AItemStorageInfoPoolOffset = tmpHeader.APurchaseItemRecordPoolOffset = tmpHeader.APurchaseOrderRecordPoolOffset = totSize;
    tmpHeader.AItemInfoCount = tmpHeader.AItemStorageInfoCount = tmpHeader.APurchaseItemRecordCount = tmpHeader.APurchaseOrderRecordCount = 0;
    char* fileBuf = new char[totSize + 1];

    // �����ļ�ͷ
    memcpy_s(fileBuf, sizeof(DatabaseHeader), &tmpHeader, sizeof(DatabaseHeader));
    // �����ַ�����
    memcpy_s(fileBuf + tmpHeader.StringPoolOffset, strlen("StringPool") + 1, "StringPool", strlen("StringPool") + 1);

    // д���ļ���
    remove(dbFileName.c_str()); // ȷ��û���ļ�����
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
    // ȫ�����ݱ�����
    ItemInfoTable.clear();
    cacheAbstractString.clear();
    GlobalString.clear();
    ItemStorageTable_mp.clear();
    PurchaseItemRecordTable_vec.clear();
    PurchaseOrderTable_mp.clear();
    PurchaseOrderTable_vec.clear();

    maxPurchaseItemRecordID = 0;
    LatestOrderId = 0;

    curDbFileName = dbFileName;

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
    
    // AbstractItemInfo �ص�����
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

    // AbstractItemStorageInfoPool �ص�����
    auto AItemStorageInfoPoolBegin = (AbstractItemStorageInfo*)(gFileData + gFileHeader->AItemStorageInfoPoolOffset);
    for (int i = 0; i < gFileHeader->AItemStorageInfoCount; i++) {
        auto pCurItemStorageInfo = AItemStorageInfoPoolBegin + i;
        CItemStorageInfo realItemStorageInfo;
        if (ItemInfoTable.count(pCurItemStorageInfo->Item) == 0) throw HarmoneyException("�޷���ItemInfo�����ҵ�ָ��ID��Item��");
        realItemStorageInfo.Item = ItemInfoTable[pCurItemStorageInfo->Item];
        realItemStorageInfo.CountRest = pCurItemStorageInfo->CountRest;
        realItemStorageInfo.IsDelete = pCurItemStorageInfo->IsDelete;
        realItemStorageInfo.Timestamp = pCurItemStorageInfo->Timestamp;
        realItemStorageInfo.WeightRest = pCurItemStorageInfo->WeightRest;
        ItemStorageTable_mp[realItemStorageInfo.Item.ItemId] = realItemStorageInfo;
    }

    // AbstractPurchaseItemRecordPool ������
    auto APurchaseItemRecordPoolBegin = (AbstractPurchaseItemRecord*)(gFileData + gFileHeader->APurchaseItemRecordPoolOffset);
    for (int i = 0; i < gFileHeader->APurchaseItemRecordCount; i++) {
        auto pCurPurchaseItemRecord = APurchaseItemRecordPoolBegin + i;
        CPurchaseItemRecord realPIRecord;
        if (ItemInfoTable.count(pCurPurchaseItemRecord->Item) == 0) throw HarmoneyException("�޷���ItemInfo�����ҵ�ָ��ID��Item��");
        realPIRecord.Item = ItemInfoTable[pCurPurchaseItemRecord->Item];
        realPIRecord.Count = pCurPurchaseItemRecord->Count;
        realPIRecord.Timestamp = pCurPurchaseItemRecord->Timestamp;
        realPIRecord.Weight = pCurPurchaseItemRecord->Weight;
        realPIRecord.OrderId = pCurPurchaseItemRecord->OrderId;
        maxPurchaseItemRecordID = std::max(maxPurchaseItemRecordID, realPIRecord.OrderId);
        PurchaseItemRecordTable_vec.push_back(realPIRecord);
    }

    // PurchaseOrderRecordPool ������
    auto PurchaseOrderRecordPoolBegin = (CPurchaseOrderRecord*)(gFileData + gFileHeader->APurchaseOrderRecordPoolOffset);
    for (int i = 0; i < gFileHeader->APurchaseOrderRecordCount; i++) {
        auto pCurPurchaseOrder = PurchaseOrderRecordPoolBegin + i;
        CPurchaseOrderRecord real;
        real.OrderId = pCurPurchaseOrder->OrderId;
        real.Timestamp = pCurPurchaseOrder->Timestamp;
        LatestOrderId = std::max(LatestOrderId, real.OrderId);
        PurchaseOrderTable_mp[real.OrderId] = real;
    }
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
    if (ItemStorageTable_mp.count(ItemId)) {
        return ItemStorageTable_mp[ItemId];
    }
    throw HarmoneyException("�޷��ҵ�ָ��ItemId��CItemStorageInfo!");
}

bool Database::ModifyItemStorageInfo(CItemStorageInfo& StorageInfo)
{
    if (ItemStorageTable_mp.count(StorageInfo.Item.ItemId)) {
        ItemStorageTable_mp[StorageInfo.Item.ItemId] = StorageInfo;
        return true;
    }
    throw HarmoneyException("�޷��ҵ�ָ��ItemId��CItemStorageInfo!");
}

bool Database::AddItemStorageInfo(CItemStorageInfo& StorageInfo)
{
    if (ItemStorageTable_mp.count(StorageInfo.Item.ItemId)) {
        auto& info = ItemStorageTable_mp[StorageInfo.Item.ItemId];
        info.WeightRest += StorageInfo.WeightRest;
        info.CountRest += StorageInfo.CountRest;
        info.Timestamp = StorageInfo.Timestamp;
        info.Item = StorageInfo.Item;
        return true;
    }
    // �����ڣ���ֱ�Ӳ���
    ItemStorageTable_mp[StorageInfo.Item.ItemId] = StorageInfo;
    return true;
}

std::vector<CItemStorageInfo>& Database::GetAllItemStorageInfo()
{
    ItemStorageTable_vec.clear();
    for (auto it = ItemStorageTable_mp.begin(); it != ItemStorageTable_mp.end(); it++) {
        ItemStorageTable_vec.push_back(it->second);
    }
    return ItemStorageTable_vec;
}

bool Database::DeleteItemStorageInfo(std::string ItemId)
{
    if (ItemStorageTable_mp.count(ItemId)) {
        ItemStorageTable_mp[ItemId].IsDelete = true;
        return true;
    }
    throw HarmoneyException("�޷��ҵ�ָ��ItemId��CItemStorageInfo!");
}

bool Database::AddPurchaseItemRecord(CPurchaseItemRecord& PurchaseItemRecord)
{
    PurchaseItemRecordTable_vec.push_back(PurchaseItemRecord);
    return true;
}

std::vector<CPurchaseItemRecord>& Database::GetAllPurchaseItemRecord()
{
    return PurchaseItemRecordTable_vec;
}

CPurchaseOrderRecord Database::QueryPurchaseOrderRecord(int OrderId)
{
    if (!PurchaseOrderTable_mp.count(OrderId)) throw HarmoneyException("ָ��OrderId�Ķ���û���ҵ���");
    return PurchaseOrderTable_mp[OrderId];
}

bool Database::AddPurchaseOrderRecord(CPurchaseOrderRecord& PurchaseRecord)
{
    if (PurchaseOrderTable_mp.count(PurchaseRecord.OrderId)) throw HarmoneyException("ָ��OrderId�Ķ����Ѿ�����!");
    PurchaseOrderTable_mp[PurchaseRecord.OrderId] = PurchaseRecord;
    LatestOrderId = std::max(LatestOrderId, PurchaseRecord.OrderId);
}

int Database::GetLatestOrderId()
{
    return LatestOrderId;
}

std::vector<CPurchaseOrderRecord>& Database::GetAllPurchaseOrderRecord()
{
    PurchaseOrderTable_vec.clear();
    for (auto it = PurchaseOrderTable_mp.begin(); it != PurchaseOrderTable_mp.end(); it++) {
        PurchaseOrderTable_vec.push_back(it->second);
    }
    return PurchaseOrderTable_vec;
}

bool Database::UpdateDatabaseFile()
{
    static std::set<std::string> AllStrings;
    static std::map<std::string, AbstractString> abStrMap;
    static std::map<std::string, AbstractItemInfo> abItemInfoMap;
    static std::vector<AbstractItemStorageInfo> abItemStorageInfoTable;
    static std::vector<AbstractPurchaseItemRecord> abPurchaseItemRecordTable;
    static int AbstractItemInfoCnt = 0;
    AllStrings.clear();
    abStrMap.clear();
    abItemInfoMap.clear();
    abItemStorageInfoTable.clear();
    abPurchaseItemRecordTable.clear();

    AbstractItemInfoCnt = 0;
    GetAllItemStorageInfo();
    GetAllPurchaseOrderRecord();
    for (CItemStorageInfo& info : ItemStorageTable_vec) {
        AllStrings.insert(info.Item.ItemId);
        AllStrings.insert(info.Item.ItemName);
    }
    // �����ַ����ش�С
    int stringPoolSize = strlen("StringPool") + 1;
    for (std::set<std::string>::iterator it = AllStrings.begin(); it != AllStrings.end(); it++) {
        AbstractString as;
        as.SPOffset = stringPoolSize;
        as.length = it->size() + 1;
        abStrMap[*it] = as;
        stringPoolSize += (it->size() + 1);
    }
    
    // ��ȡ����Item��AbstractItem��
    for (CItemStorageInfo& info : ItemStorageTable_vec) {
        AbstractItemInfo abi;
        abi.ItemDatabaseID = ++AbstractItemInfoCnt;
        abi.Cost = info.Item.Cost;
        abi.ItemId = abStrMap[info.Item.ItemId];
        abi.ItemName = abStrMap[info.Item.ItemName];
        abi.ItemType = info.Item.ItemType;
        abi.Price = info.Item.Price;
        abItemInfoMap[info.Item.ItemId] = abi;
    }

    // ��ȡ����ItemStorage��AbstractItemStorage��
    for (CItemStorageInfo& info : ItemStorageTable_vec) {
        AbstractItemStorageInfo aisi;
        aisi.CountRest = info.CountRest;
        aisi.IsDelete = info.IsDelete;
        aisi.Item = abItemInfoMap[info.Item.ItemId].ItemDatabaseID;
        aisi.Timestamp = info.Timestamp;
        aisi.WeightRest = info.WeightRest;
        abItemStorageInfoTable.push_back(aisi);
    }

    for (auto& purchaseRecord : PurchaseItemRecordTable_vec) {
        AbstractPurchaseItemRecord apir;
        apir.Count = purchaseRecord.Count;
        apir.Item = abItemInfoMap[purchaseRecord.Item.ItemId].ItemDatabaseID;
        apir.OrderId = purchaseRecord.OrderId;
        apir.Timestamp = purchaseRecord.Timestamp;
        apir.Weight = purchaseRecord.Weight;
        abPurchaseItemRecordTable.push_back(apir);
    }

    int AbstractItemInfoPoolSize = abItemInfoMap.size() * sizeof(AbstractItemInfo);
    int AbstractItemStorageInfoPoolSize = abItemStorageInfoTable.size() * sizeof(AbstractItemStorageInfo);
    int AbstractPurchaseItemRecordPoolSize = PurchaseItemRecordTable_vec.size() * sizeof(AbstractPurchaseItemRecord);
    int AbstractPurchaseOrderRecordPoolSize = PurchaseOrderTable_vec.size() * sizeof(CPurchaseOrderRecord);

    int totDBFileLength = sizeof(DatabaseHeader) + stringPoolSize + AbstractItemInfoPoolSize + AbstractItemStorageInfoPoolSize + AbstractPurchaseItemRecordPoolSize + AbstractPurchaseOrderRecordPoolSize;

    // ��ʼ�����ļ�����
    char* fileBuf = new char[totDBFileLength+2];
    // ����header
    DatabaseHeader header;
    strcpy_s(header.magicChar, "HarmoneyDB");
    header.timestamp = time(0);
    header.StringPoolOffset = sizeof(DatabaseHeader);
    header.StringPoolLength = stringPoolSize;
    header.AItemInfoPoolOffset = header.StringPoolOffset + header.StringPoolLength;
    header.AItemInfoCount = abItemInfoMap.size();
    header.AItemStorageInfoPoolOffset = header.AItemInfoPoolOffset + AbstractItemInfoPoolSize;
    header.AItemStorageInfoCount = abItemStorageInfoTable.size();
    header.APurchaseItemRecordPoolOffset = header.AItemStorageInfoPoolOffset + AbstractItemStorageInfoPoolSize;
    header.APurchaseItemRecordCount = PurchaseItemRecordTable_vec.size();
    header.APurchaseOrderRecordCount = PurchaseOrderTable_vec.size();
    header.APurchaseOrderRecordPoolOffset = header.APurchaseItemRecordPoolOffset + AbstractPurchaseItemRecordPoolSize;

    // д��header
    memcpy_s(fileBuf, totDBFileLength, &header, sizeof(header));

    // д��string
    memcpy_s(fileBuf + header.StringPoolOffset, header.StringPoolLength, "StringPool", strlen("StringPool") + 1);
    for (auto it = AllStrings.begin(); it != AllStrings.end(); it++) {
        auto as = abStrMap[*it];
        memcpy_s(fileBuf + header.StringPoolOffset + as.SPOffset, header.StringPoolLength, it->c_str(), as.length);
    }

    // д��AbstractItemInfo
    int nowWrite = 0;
    for (auto it = abItemInfoMap.begin(); it != abItemInfoMap.end(); it++) {
        memcpy_s(fileBuf + header.AItemInfoPoolOffset + nowWrite * sizeof(AbstractItemInfo), sizeof(AbstractItemInfo), &(it->second), sizeof(AbstractItemInfo));
        nowWrite++;
    }

    // д��AbstractStorageItemInfo
    nowWrite = 0;
    for (auto aisi : abItemStorageInfoTable) {
        memcpy_s(fileBuf + header.AItemStorageInfoPoolOffset + nowWrite * sizeof(aisi), sizeof(aisi), &aisi, sizeof(aisi));
        nowWrite++;
    }

    // д��AbstractPurchase
    nowWrite = 0;
    for (auto apir : abPurchaseItemRecordTable) {
        memcpy_s(fileBuf + header.APurchaseItemRecordPoolOffset + nowWrite * sizeof(apir), sizeof(apir), &apir, sizeof(apir));
        nowWrite++;
    }

    // д��PurchaseOrderRecord
    nowWrite = 0;
    for (auto po : PurchaseOrderTable_vec) {
        memcpy_s(fileBuf + header.APurchaseOrderRecordPoolOffset + nowWrite * sizeof(po), sizeof(po), &po, sizeof(po));
        nowWrite++;
    }

    remove(curDbFileName.c_str()); //ȷ��û���ļ�����
    std::ofstream file(curDbFileName, std::ios::out | std::ios::binary);
    file.write(fileBuf, totDBFileLength + 2);
    file.close();
    return true;
}


Database* DB = new Database(); // ��ȫ����