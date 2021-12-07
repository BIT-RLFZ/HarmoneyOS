#pragma once
#include "CItemStorageInfo.h"
#include "CItemInfo.h"
#include "CPurchaseItemRecord.h"
#include "CPurchaseOrderRecord.h"
#include "HarmoneyException.h"
#include "NoImplException.h"
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <set>

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
	int timestamp; //���ݿ���������ʱ��
	int StringPoolOffset; //�ַ����ص��ļ�ƫ��
	int StringPoolLength; //�ַ����صĳ���
	int AItemInfoPoolOffset;
	int AItemInfoCount;
	int AItemStorageInfoPoolOffset;
	int AItemStorageInfoCount;
	int APurchaseItemRecordPoolOffset;
	int APurchaseItemRecordCount;
	int APurchaseOrderRecordPoolOffset;
	int APurchaseOrderRecordCount;
};
struct AbstractString {
	int SPOffset; // StringPoolOffset
	int length; // string length
};
struct AbstractItemInfo {
	int ItemDatabaseID; // �����ݿ��洢ʱ������UID
	AbstractString ItemName; // Get From StringPool
	AbstractString ItemId; // Get From StringPool
	double Cost;
	double Price;
	int ItemType;
};
struct AbstractItemStorageInfo {
	int Item; // ��ItemInfo�����ҵ�ItemDatabaseIDΪItem����Ŀ��Ϊ����
	double WeightRest;
	int CountRest;
	int Timestamp; //¼��ʱ��
	bool IsDelete;
};

struct AbstractPurchaseItemRecord {
	int Item; // ��ItemInfo�����ҵ�ItemDatabaseIDΪItem����Ŀ��Ϊ����
	double Weight;
	int Count;
	int Timestamp;
	int OrderId;
};

class Database
{
public:
	Database() {
		gFileHeader = NULL;
		gFileData = NULL;
		gStringPool = NULL;
	}

	~Database() {
		try{
			UpdateDatabaseFile();
		}
		catch (NoImplException ex) {};

		if (gFileData) delete[] gFileData;
		if (gStringPool) delete[] gStringPool;
	}
	/*
		��ʼ�����ݿ⣬�������ݿ��ļ����ݲ�����
		�ļ�������ʱ���Զ���ʼ��һ���µ����ݿ��ļ�
	*/
	bool InitDatabase(std::string dbFileName="Harmoney.osdb");
	/*
		��ѯָ��ItemId��Ʒ����Ʒ��Ϣ��������CItemStorageInfo
		�����쳣��HarmoneyException
			 �ڣ��޷��ҵ�ָ����ƷIDʱ
	*/
	CItemStorageInfo QueryItemStorageInfo(std::string ItemId);
	/*
		�޸�ָ��ItemId��Ʒ����Ʒ��Ϣ������bool
		�����쳣��HarmoneyException
			 �ڣ��޷��ҵ�ָ����ƷIDʱ
	*/
	bool ModifyItemStorageInfo(CItemStorageInfo& StorageInfo);
	/*
		��ָ����Ʒ�������ݿ⣬�Ѿ����ڵ���Ʒ�ᱻ�ϲ����ϲ������ǿ���ֱ�����ӣ����µĶ��󸲸ǽ����ۼ�����
	*/
	bool AddItemStorageInfo(CItemStorageInfo& StorageInfo);
	/*
		��ѯ���ݿ������е���Ʒ������Ϣ��������Ʒ��Ϣ���б�
		ע�⣡�������Ѿ��¼�(ɾ��)����Ʒ��Ȼ�᷵�أ�ע���ж� IsDelete
	*/
	std::vector<CItemStorageInfo>& GetAllItemStorageInfo();
	/*
		��ָ����Ʒ��Delete״̬���ó�true����ʾ��Ʒ�Ѿ��¼�
		�����쳣��HarmoneyException
			 �ڣ��޷��ҵ�ָ����ƷIDʱ
	*/
	bool DeleteItemStorageInfo(std::string ItemId);

	/*
		����һ����Ʒ�Ĺ�����¼��ע��ͬһ�������п��ܻ����ɶ�����Ʒ������¼����Ϊ�����ж�����Ʒ������
	*/
	bool AddPurchaseItemRecord(CPurchaseItemRecord& PurchaseItemRecord);
	/*
		��ȡ���ݿ���������Ʒ������¼
	*/
	std::vector<CPurchaseItemRecord>& GetAllPurchaseItemRecord();

	/*
		���ݶ����ı��Ų�ѯָ������������ʱ��
	*/
	CPurchaseOrderRecord QueryPurchaseOrderRecord(int OrderId);
	/*
		�����ݿ�����һ��������¼
	*/
	bool AddPurchaseOrderRecord(CPurchaseOrderRecord& PurchaseRecord);
	/*
		��ȡ����һ�������ı��ţ������µĶ�������ʱ�������ţ�ֱ����������������ֵ+1����
		û�ж���ʱ����0
	*/
	int GetLatestOrderId();
	/*
		��ѯ���ݿ������еĶ�����¼
	*/
	std::vector<CPurchaseOrderRecord>& GetAllPurchaseOrderRecord();
	/*
		�ѵ�ǰ�ڴ��е����ݿ��ļ��洢���ļ���
		����ÿ�δ洢���ܻ�����һ����ʱ��(<100ms)��������ִ����һϵ���޸Ĳ������ٵ�����������
		ע�⣺�ڳ����˳�ʱ���Զ�ִ���������䣬����Ϊ�˷�ֹ���⣬��ÿ�β������ݿ��󶼵��ô�����
	*/
	bool UpdateDatabaseFile();
private:
	std::string curDbFileName;
	char* gFileData;
	char* gStringPool;
	DatabaseHeader* gFileHeader;
	std::set<std::string> GlobalString;
	std::map<int, std::string> cacheAbstractString;
	std::map<int, CItemInfo> ItemInfoTable;
	std::map<std::string, CItemStorageInfo> ItemStorageTable_mp;
	std::vector<CItemStorageInfo> ItemStorageTable_vec;
	std::vector<CPurchaseItemRecord> PurchaseItemRecordTable_vec;
	std::map<int, CPurchaseOrderRecord> PurchaseOrderTable_mp;
	std::vector<CPurchaseOrderRecord> PurchaseOrderTable_vec;
	int maxPurchaseItemRecordID;
	int LatestOrderId;

	void GenerateEmptyDBFile(std::string dbFileName);
	std::string GetAbstractString(const AbstractString& as);
	void LoadBinaryDBFile(std::string dbFileName);
};
