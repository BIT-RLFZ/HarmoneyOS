#include "CashierSystem.h"
#include "Database.h"
#include "CPurchaseItemRecord.h"
#include "CItemStorageInfo.h"
#include "CPurchaseOrderRecord.h"
#include "NoImplException.h"
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>

const int COUNT = 0;
const int WEIGHT = 1;
//生成商品订单的ID
void CashierSystem::generateOrderId()
{
	OrderId = DB->GetLatestOrderId() + 1;
}
//生成时间戳
void CashierSystem::generateTimestamp()
{
	Timestamp = (int)time(0);
}
/*
	实现校验功能 
	返回值 bool: true代表通过校验，反之不通过
*/
bool CashierSystem::CheckCode(const std::string& value, int code)
{
	int Result = 0;	//初始化
	int Length = value.length();	//得到字符串长
	for (int i = 0; i < Length; ++i) {
		if (value[i] < '0' || value[i] > '9') {
			throw HarmoneyException("商品ID中存在非数字字符！");
		}
		Result ^= value[i] - '0';	//逐位异或
	}
	return Result == code;	//校验
}
/*
	处理商品ID得到对应的商品原始ID,商品类型，商品重量
*/
std::string CashierSystem::processId(const std::string& ItemProcessedId, int &Type, double &Weight)
{
	// TODO: 在此处插入 return 语句
	// 八位商品ID 五位的重量 两位校验位
	if (ItemProcessedId.length() != 15) {	//ID长度不符
		throw HarmoneyException("商品ID长度错误!");
	}
	size_t start = 0;	//截取子串使用
	int TmpWeight = 0, code = 0;
	std::string ItemId;
	try {
		ItemId = ItemProcessedId.substr(start, 8);	//分割出商品原始ID
		TmpWeight = stoi(ItemProcessedId.substr(start + 8, 5));	//截取出表示重量的位
		code = stoi(ItemProcessedId.substr(start + 13, 2));	//截取出校验码
	}
	catch (...) {
		throw HarmoneyException("商品ID中存在非数字字符！");
	}
	if (!CheckCode(ItemProcessedId.substr(start, 13), code)) {	//如果前面与校验码不匹配，抛异常
		throw HarmoneyException("商品校验码错误");
	}
	if (TmpWeight == 0) {	//重量位均为0，表示该商品类型为COUNT
		Type = COUNT;
		Weight = 0.00;
	}
	else {	//否则类型为WEIGHT
		Type = WEIGHT;
		Weight = 1.0 * TmpWeight / 100;	//计算该商品的重量
	}
	return ItemId;
}
/*
	构造方法
	获得新的订单ID与时间戳
*/
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
	CItemStorageInfo ItemStorageInfo = DB->QueryItemStorageInfo(ItemId);
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
	CItemStorageInfo ItemStorageInfo = DB->QueryItemStorageInfo(ItemId);	//数据库中查找商品的信息
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
		DB->AddPurchaseItemRecord(ItemRecord);	//添加商品购买记录
		ItemStorageInfo = DB->QueryItemStorageInfo(ItemRecord.Item.ItemId);	//获取库存信息
		if (ItemRecord.Item.ItemType == COUNT) {	// 根据商品类别更改库存对象的信息
			ItemStorageInfo.CountRest -= ItemRecord.Count;
		}
		else {
			ItemStorageInfo.WeightRest -= ItemRecord.Weight;
		}
		DB->ModifyItemStorageInfo(ItemStorageInfo);	//修改库存
		Cart.pop_back();	//清除商品
	}
	CPurchaseOrderRecord OrderRecord(OrderId, Timestamp);	//生成订单
	DB->AddPurchaseOrderRecord(OrderRecord);	//向数据库添加订单
	return true;
}
