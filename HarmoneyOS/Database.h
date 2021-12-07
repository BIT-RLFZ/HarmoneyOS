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
	int StringPoolLength; //字符串池的长度
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

struct AbstractPurchaseItemRecord {
	int Item; // 查ItemInfo表，找到ItemDatabaseID为Item的项目即为此项
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
		初始化数据库，读入数据库文件数据并解析
		文件不存在时会自动初始化一个新的数据库文件
	*/
	bool InitDatabase(std::string dbFileName="Harmoney.osdb");
	/*
		查询指定ItemId商品的商品信息，返回类CItemStorageInfo
		可能异常：HarmoneyException
			 在：无法找到指定商品ID时
	*/
	CItemStorageInfo QueryItemStorageInfo(std::string ItemId);
	/*
		修改指定ItemId商品的商品信息，返回bool
		可能异常：HarmoneyException
			 在：无法找到指定商品ID时
	*/
	bool ModifyItemStorageInfo(CItemStorageInfo& StorageInfo);
	/*
		将指定商品加入数据库，已经存在的商品会被合并，合并方法是库存直接相加，而新的对象覆盖进价售价数据
	*/
	bool AddItemStorageInfo(CItemStorageInfo& StorageInfo);
	/*
		查询数据库中所有的商品库存信息，返回商品信息的列表
		注意！！！，已经下架(删除)的物品依然会返回，注意判断 IsDelete
	*/
	std::vector<CItemStorageInfo>& GetAllItemStorageInfo();
	/*
		将指定商品的Delete状态设置成true，表示商品已经下架
		可能异常：HarmoneyException
			 在：无法找到指定商品ID时
	*/
	bool DeleteItemStorageInfo(std::string ItemId);

	/*
		添加一个商品的购买记录，注意同一个订单中可能会生成多个商品购买记录，因为可能有多个商品被购买
	*/
	bool AddPurchaseItemRecord(CPurchaseItemRecord& PurchaseItemRecord);
	/*
		获取数据库中所有商品购买记录
	*/
	std::vector<CPurchaseItemRecord>& GetAllPurchaseItemRecord();

	/*
		根据订单的编号查询指定订单的消费时间
	*/
	CPurchaseOrderRecord QueryPurchaseOrderRecord(int OrderId);
	/*
		向数据库加入一条订单记录
	*/
	bool AddPurchaseOrderRecord(CPurchaseOrderRecord& PurchaseRecord);
	/*
		获取最后一条订单的编号，用于新的订单生成时订单编号，直接用这个函数返回值+1即可
		没有订单时返回0
	*/
	int GetLatestOrderId();
	/*
		查询数据库中所有的订单记录
	*/
	std::vector<CPurchaseOrderRecord>& GetAllPurchaseOrderRecord();
	/*
		把当前内存中的数据库文件存储到文件中
		由于每次存储可能会消耗一定的时间(<100ms)，建议在执行完一系列修改操作后再调用这条语句
		注意：在程序退出时会自动执行这条语句，但是为了防止意外，请每次操作数据库后都调用此语句
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

