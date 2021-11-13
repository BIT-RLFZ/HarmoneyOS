#pragma once
#include "CItemStorageInfo.h"
#include "CPurchaseItemRecord.h"
#include "CPurchaseOrderRecord.h"
#include <string>
#include <vector>
class Database
{
public:
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
private:

};

