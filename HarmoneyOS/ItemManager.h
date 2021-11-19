#pragma once
#include "CItemStorageInfo.h"
#include "CPurchaseItemRecord.h"
#include "CPurchaseOrderRecord.h"
#include <string>
#include <vector>
struct SingleOrder {//每一份订单里有价值的信息
    double OrderProfit;//订单总利润
    double OrderCost;//订单总价格
};
struct OrderYouWannaKnow {//每个月的财务报表，记录当月订单里有价值的信息
    SingleOrder MaxProfit;//利润
    SingleOrder MinProfit;
    SingleOrder MaxCost;//最大金额的订单
    SingleOrder MinCost;
    int OrderAmount;//订单数目
    OrderYouWannaKnow(const SingleOrder& a, const SingleOrder& b,
        const SingleOrder& c, const SingleOrder& d, const int& e) {
        MaxProfit = a;
        MinProfit = b;
        MaxCost = c;
        MinCost = d;
        OrderAmount = e;
    }
};

class ItemManager
{
private:
    int TotalMonthCount; //到现在为止几个月了
    std::vector <OrderYouWannaKnow> FinancialList; //保存每个月的订单中的有价值的信息
    std::vector <SingleOrder> Orders; //存储全部订单
    int TimestampConvertToMonthNumber(const int& Timestamp);//时间戳转换

public:
    CItemStorageInfo ReceiveAction();

    bool AddItem(const CItemStorageInfo& NewItem);//返回商品类
    bool ModifyItem(const CItemStorageInfo& NewItem);//修改的是类
    bool DeleteItem(std::string ItemId);//输入商品编号删除
    std::vector<CItemStorageInfo>& GetAllItemStorageInfo();//获取仓库里所有的商品信息
    //以上是对商品的增删改查

    //以下是对订单信息的处理
    /*
      思路：从数据库获得全部的物品销售信息(1)
      -> 通过订单号，将单个物品的销售信息合并到单个订单信息里(2)
      -> 根据订单信息按照时间戳分组，获得每个月的财务信息，并保留有价值的(3)
      -> 将有价值的信息综合一下(4)
    */
    std::vector<CPurchaseOrderRecord>& GetAllPurchaseOrderRecord();//(1)

    bool GetProfit(const std::vector<CItemStorageInfo>& ItemList);//(2)

    OrderYouWannaKnow GetOrders(const std::vector<SingleOrder>& OrderLists); //(3)
    bool CreateNewMonthlyRecord(const int& Timestamp, const double& Profit); //(3)
    //添加一个月度的总利润记录

    bool CreateMonthlyFinancialStatement(const std::vector<OrderYouWannaKnow>& List); //(4)
    //创建一个月度财务报表，统计从 0 开始，到现在为止，以月为单位统计利润信息
    //收集每个月的利润信息 绘制出图表
    //收集每个月的消费信息 绘制出图表

   // bool AddItem(std::string ItemId, std::string ItemName, double WeightRest, int CountRest,);
};