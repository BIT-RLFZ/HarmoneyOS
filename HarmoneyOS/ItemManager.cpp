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

#include <Windows.h>

ItemManager* itemMgr = new ItemManager(); // ��ȫ����
//ʱ���������ʱ�䣩

using namespace std;

int stamp_to_standard(int stampTime) {
	time_t tick = (time_t)stampTime;
	struct tm tm;
	char s[100];
	localtime_s(&tm, &tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);//����s��һ���ַ�������ʾ��ʱ���ת����������ʵʱ��
	//	printf("%s\n",  s);
	int Y1 = 0, M1 = 0, D1 = 0;
	Y1 += (s[0] - '0') * 1000;
	Y1 += (s[1] - '0') * 100;
	Y1 += (s[2] - '0') * 10;
	Y1 += (s[3] - '0') * 1;
	M1 += (s[5] - '0') * 10;
	M1 += (s[6] - '0') * 1;
	int totMonth = (Y1 - 1970) * 12 + M1;//totMonth��ʾʱ���ת������֮����1970�꿪ʼ�����˼����£�����1970��1���ǵ�һ���¡�
	return totMonth;
}

int ItemManager::TimestampConvertToMonthNumber(const int& Timestamp) {//ʱ���ת���ɵڼ�����
	return stamp_to_standard(Timestamp);//���������ʱ���ת������ʵ���ڣ��������ǵڼ�����
}//�������뻻�����ͳ�ƵĻ�������ֱ�Ӱ��·�-1����12Ȼ���1��ɵڼ��꣬��һ���ǵ�һ�꣨���ǵ����꣩
Timeacc ItemManager::TimestampConvertToYearAndSeason(const int& Timestamp) {//�ṹ���Timeacc���ͷ����˶�Ӧʱ�������ݼӼ���
	int totmonth = stamp_to_standard(Timestamp);
	int year = (totmonth - 1) / 12;
	int season = ((totmonth - year * 12) - 1) / 3;
	Timeacc acc;
	acc.year = year + 1970; acc.season = season + 1;//���ص�����ʵ�Ķ�Ӧ��ݺͼ��ȣ�����123����1���ȣ�
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
	int mv = 998244353;
	MaxId = 0; MaxMonth = 0;
	ItemList = DB->GetAllPurchaseItemRecord();
	Goods = DB->GetAllItemStorageInfo();
	for (auto t : ItemList)
		MaxId = max(MaxId, t.OrderId);
	Orders.clear();
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
		mv = min(mv, Orders[i].WhichMonth);
	}
	mv--;
	for (int i = 1; i <= MaxId; ++i) {
		Orders[i].WhichMonth -= mv; 
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

string buff = "";

void clearBuf() {
	buff.clear();
}

string getBuff() {
	return buff;
}

ULONG myprintf(const char* fmt, ...) {
	ULONG cnt = 0;
	static char buf[3000] = { 0 };
	va_list argptr;
	va_start(argptr, fmt);
	cnt = vsprintf_s(buf, 3000, fmt, argptr);
	va_end(argptr);
	buff += buf;
	return cnt;
}

// ������Ϣ
bool ItemManager::ShowPurchaseRecord() {
	Prework();
	sort(Orders.begin() + 1, Orders.end(), comp_timeorder);
	myprintf("Below is the information of all orders so far\n");
	for (int i = 1; i <= MaxId; ++i) {
		myprintf("%d �� ������ţ� %d �ö��������ѣ� %lf �ö�����Ĳ���� %lf \n",
			Orders[i].WhichMonth, Orders[i].OrderId,
			Orders[i].OrderCost, Orders[i].OrderProfit);
	}
	return 0;
}
// ������Ʒ��Ϣ
bool ItemManager::ShowAllItemRecord() {
	Prework();
	int n = Goods.size(); //���˸���
	for (int i = 0; i < n; ++i) {
		if (Goods[i].IsDelete) continue;
		myprintf("��Ʒ��ţ� %s  ", Goods[i].Item.ItemId.c_str());
		myprintf(" ��Ʒ���ƣ� %s  ", Goods[i].Item.ItemName.c_str());
		myprintf("����Ʒ����Ϊ %lf Ԫ���ۼ�Ϊ %lf Ԫ��������Ϊ %lf Ԫ ",
			Goods[i].Item.Cost, Goods[i].Item.Price, Goods[i].Item.Price - Goods[i].Item.Cost);
		if (Goods[i].Item.ItemType) myprintf("����Ʒ�Ե�����ʽ����, ʣ�� %d ��\n", Goods[i].CountRest);
		else myprintf("����Ʒ��ɢװ������ʽ����, ʣ�� %lf ��\n", Goods[i].WeightRest);
	}
	return 0;
}
OrderYouWannaKnow ItemManager::MonthCheck(int x) { return MonthlyList[x]; }
void ItemManager::GetRate() {
	MonthlyRate[1] = make_pair(1.0, 1.0);
	for (int i = 2; i <= MaxMonth; ++i) {
		if (MonthlyList[i - 1].MaxCost.OrderCost == 0.0) continue;
		if (MonthlyList[i - 1].MaxProfit.OrderProfit == 0.0) continue;
		MonthlyRate[i].first = (MonthlyList[i].MaxCost.OrderCost
			/ MonthlyList[i - 1].MaxCost.OrderCost) - 1.0;
		MonthlyRate[i].second = (MonthlyList[i].MaxProfit.OrderProfit
			/ MonthlyList[i - 1].MaxProfit.OrderProfit) - 1.0;
	}
	return;
}
// ����Ϊ��λ��������Ϣͳ��
bool ItemManager::CreateMonthlyFinancialStatement() {
	Prework();
	GetRate();
	for (int i = 2; i <= MaxMonth; ++i) {
		myprintf("---------------------------------------------------------------");
		myprintf("&���������Ԫ�ĵ� %d ���£�����Ϊ���µ����ͳ����Ϣ\n", i);
		myprintf(" ���¶�������.Ϊ %d�������۶�Ϊ %lf Ԫ��������Ϊ %lf Ԫ\n", MonthlyList[i].OrderAmount, MonthlyList[i].CostAmount, MonthlyList[i].ProfitAmount);
		myprintf(" �����������Ķ���Ϊ��� %d �Ķ������������Ϊ %lf Ԫ������ͬ������ %lf %%\n", MonthlyList[i].MaxProfit.OrderId, MonthlyList[i].MaxProfit.OrderProfit, MonthlyRate[i].second);
		myprintf(" ����������ѵĶ���Ϊ��� %d �Ķ������������Ϊ %lf Ԫ������ͬ������ %lf %%\n", MonthlyList[i].MaxCost.OrderId, MonthlyList[i].MaxCost.OrderCost, MonthlyRate[i].first);
	}
	return 0;
}