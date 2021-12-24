#include "ItemManager.h"
#include "CItemInfo.h"
#include "CashierSystem.h"
#include "CItemStorageInfo.h"
#include "CPurchaseItemRecord.h"
#include "CPurchaseOrderRecord.h"

#include <time.h>
#include <vector>
#include <cstring>
#include <iostream>
#include <algorithm>

ItemManager* itemMgr = new ItemManager(); // 给全局用
//时间戳（北京时间）

using namespace std;

int stamp_to_standard(int stampTime) {
	time_t tick = (time_t)stampTime;
	struct tm tm;
	char s[100];
	localtime_s(&tm, &tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);//其中s是一个字符串，表示着时间戳转换过来的真实时间
	//	printf("%s\n",  s);
	int Y1 = 0, M1 = 0, D1 = 0;
	Y1 += (s[0] - '0') * 1000;
	Y1 += (s[1] - '0') * 100;
	Y1 += (s[2] - '0') * 10;
	Y1 += (s[3] - '0') * 1;
	M1 += (s[5] - '0') * 10;
	M1 += (s[6] - '0') * 1;
	int totMonth = (Y1 - 1970) * 12 + M1;//totMonth表示时间戳转换过来之后是1970年开始的有了几个月，其中1970年1月是第一个月。
	return totMonth;
}

int ItemManager::TimestampConvertToMonthNumber(const int& Timestamp) {//时间戳转换成第几个月
	return stamp_to_standard(Timestamp);//这里调用了时间戳转换成真实日期，并返回是第几个月
}//今后如果想换成年度统计的话，可以直接把月份-1除以12然后加1变成第几年，第一年是第一年（不是第零年）
Timeacc ItemManager::TimestampConvertToYearAndSeason(const int& Timestamp) {//结构体的Timeacc类型返回了对应时间戳的年份加季度
	int totmonth = stamp_to_standard(Timestamp);
	int year = (totmonth - 1) / 12;
	int season = ((totmonth - year * 12) - 1) / 3;
	Timeacc acc;
	acc.year = year + 1970; acc.season = season + 1;//返回的是真实的对应年份和季度，比如123月是1季度；
	return acc;
}
bool ItemManager::AddItem(CItemStorageInfo NewItem) {
	return DB->AddItemStorageInfo(NewItem);
}
bool ItemManager::ModifyItem(CItemStorageInfo NewItem) {
	return DB->ModifyItemStorageInfo(NewItem);
}
bool ItemManager::DeleteItem(std::string ItemId) {
	return DB->DeleteItemStorageInfo(ItemId);
}

int MaxId = 0;
int MaxMonth = 0;

bool comp_cost(const SingleOrder& a, const SingleOrder& b) {
	return a.OrderCost < b.OrderCost;
}  // a < b 1 ; a >= b 0
bool comp_profit(const SingleOrder& a, const SingleOrder& b) {
	return a.OrderProfit < b.OrderProfit;
}
bool comp_timeorder(const SingleOrder& a, const SingleOrder& b) {
	return a.WhichMonth == b.WhichMonth ? a.OrderId < b.OrderId : a.WhichMonth < b.WhichMonth;
}
SingleOrder NullOrder;
void ItemManager::Prework() {
	MaxId = 0; MaxMonth = 0;
	ItemList = DB->GetAllPurchaseItemRecord();
	Goods = DB->GetAllItemStorageInfo();
	for (auto t : ItemList)
		MaxId = max(MaxId, t.OrderId);
	for (int i = 0; i <= MaxId; ++i)
		Orders.push_back(NullOrder);
	for (CPurchaseItemRecord t : ItemList) {
		int i = t.OrderId;
		if (t.Item.ItemType == 1) {
			Orders[i].OrderCost = t.Item.Price * t.Weight;
			Orders[i].OrderProfit = -t.Item.Cost * t.Weight;
		}
		else {
			Orders[i].OrderCost = t.Item.Price * 1.0 * t.Count;
			Orders[i].OrderProfit = -t.Item.Cost * 1.0 * t.Count;
		}
		Orders[i].OrderId = i;
		Orders[i].WhichMonth = TimestampConvertToMonthNumber(t.Timestamp);
		MaxMonth = max(ItemManager::TotalMonthCount, Orders[i].WhichMonth);
	}
	for (int m, i = 1; i <= MaxId; ++i) {
		m = Orders[i].WhichMonth;
		SingleOrder& T1 = MonthlyList[m].MaxProfit;
		SingleOrder& T2 = MonthlyList[m].MaxCost;
		Orders[i].OrderProfit += Orders[i].OrderCost;
		MonthlyList[m].OrderAmount++;
		MonthlyList[m].CostAmount += Orders[i].OrderCost;
		MonthlyList[m].ProfitAmount += Orders[i].OrderProfit;
		if (comp_profit(T1, Orders[i])) T1 = Orders[i];
		if (comp_cost(T2, Orders[i])) T2 = Orders[i];
	}
	return;
}

bool ItemManager::ShowPurchaseRecord() {
	Prework();
	sort(Orders.begin() + 1, Orders.end(), comp_timeorder);
	printf("Below is the information of all orders so far\n");
	for (int i = 1; i <= MaxId; ++i) {
		printf("%d 月 订单编号： %d 该订单共消费： %lf 该订单共牟利： %lf \n",
			Orders[i].WhichMonth, Orders[i].OrderId,
			Orders[i].OrderCost, Orders[i].OrderProfit);
	}
	return 0;
}
bool ItemManager::ShowAllItemRecord() {
	int n = Goods.size(); //鸽了鸽了
	for (int i = 0; i < n; ++i) {
		if (Goods[i].IsDelete) continue;
		printf("物品编号： "); cout << Goods[i].Item.ItemId;
		printf(" 物品名称： "); cout << Goods[i].Item.ItemName;
		printf("该物品进价为 %lf 元，售价为 %lf 元，净利润为 %lf 元 ",
			Goods[i].Item.Cost, Goods[i].Item.Price, Goods[i].Item.Price - Goods[i].Item.Cost);
		if (Goods[i].Item.ItemType) printf("该物品以单个形式售卖, 剩余 %d 个\n", Goods[i].CountRest);
		else printf("该物品以散装称重形式售卖, 剩余 %lf 斤\n", Goods[i].WeightRest);
	}
	return 0;
}
OrderYouWannaKnow ItemManager::MonthCheck(int x) { return MonthlyList[x]; }
void ItemManager::GetRate() {
	MonthlyRate[1] = make_pair(1.0, 1.0);
	for (int i = 2; i <= MaxMonth; ++i) {
		MonthlyRate[i].first = (MonthlyList[i].MaxCost.OrderCost
			/ MonthlyList[i - 1].MaxCost.OrderCost) - 1.0;
		MonthlyRate[i].second = (MonthlyList[i].MaxProfit.OrderProfit
			/ MonthlyList[i - 1].MaxProfit.OrderProfit) - 1.0;
	}
	return;
}
bool ItemManager::CreateMonthlyFinancialStatement() {
	Prework();
	GetRate();
	for (int i = 2; i <= MaxMonth; ++i) {
		printf("---------------------------------------------------------------");
		printf("&这是人类纪元的第 %d 个月，以下为本月的相关统计信息\n", i);
		printf(" 本月订单总数.为 %d，总销售额为 %lf 元，总利润为 %lf 元\n", MonthlyList[i].OrderAmount, MonthlyList[i].CostAmount, MonthlyList[i].ProfitAmount);
		printf(" 本月最大利润的订单为编号 %d 的订单，最大利润为 %lf 元，上月同比增长 %lf %%\n", MonthlyList[i].MaxProfit.OrderId, MonthlyList[i].MaxProfit.OrderProfit, MonthlyRate[i].second);
		printf(" 本月最大消费的订单为编号 %d 的订单，最大消费为 %lf 元，上月同比增长 %lf %%\n", MonthlyList[i].MaxCost.OrderId, MonthlyList[i].MaxCost.OrderCost, MonthlyRate[i].first);
	}
	return 0;
}