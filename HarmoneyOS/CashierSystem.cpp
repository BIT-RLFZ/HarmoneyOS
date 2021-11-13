#include "CashierSystem.h"
#include "Database.h"
#include "CPurchaseItemRecord.h"
#include "CItemStorageInfo.h"
#include "CPurchaseOrderRecord.h"
#include <string>
#include <algorithm>
#include "NoImplException.h";
const int COUNT = 0;
const int WEIGHT = 1;
//未实现
void CashierSystem::generateOrderId()
{
	throw new NoImplException(__FUNCTION__);
}
//未实现
void CashierSystem::generateTimestamp()
{
	throw new NoImplException(__FUNCTION__);
}
/*
	该函数功能并未实现完，依赖于开发约定
*/
std::string CashierSystem::processId(std::string ItemProcessedId, int &Type, double &Weight)
{
	// TODO: 在此处插入 return 语句
	throw new NoImplException(__FUNCTION__);
	std::string ItemId = ItemProcessedId;
	return ItemId;
}

CashierSystem::CashierSystem()
{
	generateOrderId();
	generateTimestamp();
}
/*
*	该函数还未处理数据库可能抛出的异常！
	实现将商品添加到购物车的过程
	参数：ItemProcessedId string 处理过后的ID
		  cnt int 顾客购买商品的数量
				  如果是重量计价的商品，cnt参数就传递1，重量信息已经包含在第一个参数中
	返回值：bool 表示添加操作是否成功
*/
bool CashierSystem::AddItemToCart(const std::string& ItemProcessedId, int cnt)
{
	int Type = 0;
	double Weight = 0.0;
	// 将处理过的Id转换成原来的Id 并得到商品类型，商品重量(如果是按重量计价的商品的话)
	std::string ItemId = processId(ItemProcessedId,Type, Weight);
	// 按原Id找到指定的商品信息
	CItemStorageInfo ItemStorageInfo = DB.QueryItemStorageInfo(ItemId);
	//将商品，商品个数，时间戳和订单号包装成一条购物记录
	CPurchaseItemRecord Record(ItemStorageInfo.Item, Weight, cnt, Timestamp, OrderId);
	// 查找当前购物车中是否有该商品
	auto ItemRecordIter = find(Cart.begin(), Cart.end(), Record);
	if (ItemRecordIter != Cart.end()) {	// 如果购物车中已有就进行合并
		if (Type == COUNT) {	// 按个数计价的商品
			ItemRecordIter->Count += cnt;
		}
		else {	//按重量计价的商品
			ItemRecordIter->Weight += Weight;
		}
	}
	else {	//如果不在购物车中就放入购物车
		Cart.push_back(Record);
	}
	return true;	// 操作成功返回true
}
/*
*	该函数还未处理数据库可能抛出的异常！
	从购物车中删除商品
	参数：ItemProcessedId string 要删除的商品的处理过的ID
	返回值：bool 删除操作是否成功
			true 代表成功
			false 代表没有找到商品未成功
*/
bool CashierSystem::RemoveItemFromCart(const std::string& ItemProcessedId)
{
	int Type = 0;
	double Weight = 0.0;
	std::string ItemId = processId(ItemProcessedId, Type, Weight);	//转换成商品原ID
	CItemStorageInfo ItemStorageInfo = DB.QueryItemStorageInfo(ItemId);	//数据库中查找商品的信息
	auto ItemRecordIter = Cart.begin();	//遍历购物车
	while (ItemRecordIter != Cart.end()) {
		if (ItemRecordIter->Item == ItemStorageInfo.Item) {	//找到该商品就删掉
			Cart.erase(ItemRecordIter, ItemRecordIter + 1);
			return true;
		}
		ItemRecordIter++;
	}
	return false;	// 购物车中没有找到对应商品就返回false
}

/*
	返回值 double 当前购物车中货物的总价值
	遍历当前购物车中商品，分类别计算价值累加到总价值中去
*/
double CashierSystem::GetCurrentPrice()
{
	double CurrentPrice = 0.0;	//初始化
	auto ItemRecordIter = Cart.begin();	//vector的迭代器
	while (ItemRecordIter != Cart.end()) {	//遍历每一个元素
		if (ItemRecordIter->Item.ItemType == COUNT) {	// 判断类型 分别计算价值
			CurrentPrice += ItemRecordIter->Item.Price * ItemRecordIter->Count;
		}
		else {	// 按重量称重
			CurrentPrice += ItemRecordIter->Item.Price * ItemRecordIter->Weight;
		}
		ItemRecordIter++;
	}
	return CurrentPrice;
}
/*
	返回值 const std::vector<CPurchaseItemRecord>& 
		返回当前购物车列表
		const & 代表不希望调用时将购物车改动
		如果前端调用有问题时再做代码改动
*/
const std::vector<CPurchaseItemRecord>& CashierSystem::GetCurrentPurchaseList()
{
	// TODO: 在此处插入 return 语句
	return Cart;
}
/*
* 该函数还未处理数据库可能抛出的异常！
	结账功能
	首先将购物车列表传递给参数
	然后一边清空购物车一边结账
	对于每一条购买记录 
		先要将购买记录添加到数据库
		更改库存信息
	最后要把订单添加到数据库中
*/
bool CashierSystem::Checkout(std::vector<CPurchaseItemRecord>& CurrentPurchaseList)
{
	CurrentPurchaseList = Cart;	// 将购物车列表传递个参数
	CPurchaseItemRecord ItemRecord;
	CItemStorageInfo ItemStorageInfo;
	while (!Cart.empty()) {
		ItemRecord = Cart.back();
		DB.AddPurchaseItemRecord(ItemRecord);	//添加商品购买记录
		ItemStorageInfo = DB.QueryItemStorageInfo(ItemRecord.Item.ItemId);	//获取库存信息
		if (ItemRecord.Item.ItemType == COUNT) {	// 根据商品类别更改库存对象的信息
			ItemStorageInfo.CountRest -= ItemRecord.Count;
		}
		else {
			ItemStorageInfo.WeightRest -= ItemRecord.Weight;
		}
		DB.ModifyItemStorageInfo(ItemStorageInfo);	//修改库存
		Cart.pop_back();	//清楚商品
	}
	CPurchaseOrderRecord OrderRecord(OrderId, Timestamp);	//生成订单
	DB.AddPurchaseOrderRecord(OrderRecord);	//向数据库添加订单
	return true;
}
