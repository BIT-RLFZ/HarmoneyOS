#pragma once
#include "CPurchaseItemRecord.h"
#include "Database.h"
#include <string>
#include <vector>
#include "GlobalVar.h"
/*
	在顾客每次结账前都要新建一个CashierSystem对象
	通过执行构造方法生成新的订单ID与时间戳
*/
class CashierSystem
{
private:
	std::vector<CPurchaseItemRecord> Cart;	// 购物车，用vector记录
	int OrderId;	// 当前订单的编号
	int Timestamp;	//当前订单的时间戳
	void generateOrderId(); //生成当前订单的订单编号
	void generateTimestamp();	// 生成当前订单的时间戳
	bool CheckCode(const std::string& value, int code);	//对商品ID进行校验
	std::string processId(const std::string& ItemProcessedId, int &Type, double &Weight);
public:
	/*
		初始化订单的编号和与时间戳等初始信息
	*/
	CashierSystem();
	/*
		添加商品到当前购物车
		参数: ItemProcessedId string 要添加商品的编号(处理过的)
			  cnt int 该种商品的数量
		返回值： bool 代表增加操作是否成功执行
	*/
	bool AddItemToCart(const std::string& ItemProcessedId,int cnt);
	/*
		从当前购物车中删除商品
		参数：ItemProcessedId string 要删除的商品的编号(处理过的)
		返回值：bool 代表删除操作是否成功执行
	*/
	bool RemoveItemFromCart(const std::string& ItemProcessedId);
	/* 
	*	获得当前购物车中货物的总价值
		返回值: double 购物车中当前货物的总价值
	*/
	double GetCurrentPrice();
	/*
		获取当前购物车中的商品记录信息
		返回值 ：vector<CPurchaseItemRecord> 返回的vector中包含购物车中所有商品信息
	*/
	const std::vector<CPurchaseItemRecord>& GetCurrentPurchaseList();
	/*
		最后结账，清空购物车
		参数：CurrentPurchaseList vector<CPurchaseItemRecord>& 传入一个vector以便打印小票
		返回值：结账相关操作是否成功
	*/
	bool Checkout(std::vector<CPurchaseItemRecord>& CurrentPurchaseList);
};

