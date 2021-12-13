#pragma once
#include "CItemInfo.h"
#include "CashierSystem.h"
#include "CItemStorageInfo.h"
#include "CPurchaseItemRecord.h"
#include "CPurchaseOrderRecord.h"

#include "Database.h"
#include "NoImplException.h"

#include <string>
#include <vector>
#include <utility>

extern Database* DB;
struct Timeacc//结构体的Timeacc类型返回了对应时间戳的年份加季度
{
    int year;
    int season;
};
struct SingleOrder {//每一份订单里有价值的信息
    double OrderProfit;//订单总利润
    double OrderCost;//订单总价格
    int WhichMonth ;
    int OrderId ;
} ;

struct OrderYouWannaKnow {//每个月的财务报表，记录当月订单里有价值的信息
    SingleOrder MaxProfit;//利润
    SingleOrder MaxCost;//最大金额的订单
    int OrderAmount;//订单数目
    double CostAmount ;
    double ProfitAmount ;
};

class ItemManager{
private:
    int OrderCount ;
    int TotalMonthCount; //到现在为止几个月了
    std::vector <CPurchaseItemRecord> ItemList ; //物品列表
    std::pair <double, double> MonthlyRate[5050] ; // costrate, profitrate
    OrderYouWannaKnow MonthlyList[5050] ; //保存每个月的订单中的有价值的信息
    std::vector <SingleOrder> Orders; //存储全部订单ZZ
    std::vector <CItemStorageInfo> Goods; //存储全部货物YY
    int TimestampConvertToMonthNumber(const int& Timestamp);//时间戳转换，学pair<int, int> a;
    Timeacc TimestampConvertToYearAndSeason(const int& Timestamp);//时间转换成年份和对应的季度
public:
    CItemStorageInfo ReceiveAction();

    bool AddItem(CItemStorageInfo NewItem);//返回商品类
    bool ModifyItem(CItemStorageInfo NewItem);//修改的是类
    bool DeleteItem(std::string ItemId);//输入商品编号删除//YY获取仓库里所有的商品信息, 对接rbh
    //以上是对商品的增删改查

    //以下是对订单信息的处理
    /*
      思路：从数据库获得全部的物品销售信息(1)
      -> 通过订单号，将单个物品的销售信息合并到单个订单信息里(2)
      -> 根据订单信息按照时间戳分组，获得每个月的财务信息，并保留有价值的(3)
      -> 将有价值的信息综合一下(4)
    */
    void Prework() ;
    void GetRate() ;
    bool ShowPurchaseRecord() ;
    bool ShowAllItemRecord() ;
    OrderYouWannaKnow MonthCheck(int x) ;
    bool CreateMonthlyFinancialStatement(); //(4)
    //创建一个月度财务报表，统计从 0 开始，到现在为止，以月为单位统计利润信息
    //收集每个月的利润信息 绘制出图表
    //收集每个月的消费信息 绘制出图表

   // bool AddItem(std::string ItemId, std::string ItemName, double WeightRest, int CountRest,);
};
