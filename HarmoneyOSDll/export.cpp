#include <Windows.h>
#include "../HarmoneyOS/Database.h"
#include "../HarmoneyOS/ItemManager.h"
#include "../HarmoneyOS/CashierSystem.h"
#include <string>

using namespace std;

extern ItemManager* itemMgr;
extern CashierSystem* cashierSys;
extern Database* DB;




#define EXPORT extern "C" _declspec (dllexport)

string lastErrorInfo = "";

const string BuildTime = (string)__DATE__ + " " + __TIME__;

struct CItemInfoExport {
	const char* ItemName;
	double Cost;
	double Price;
	const char* ItemId;
	int ItemType;
};

struct CPurchaseItemRecordExport {
	CItemInfoExport Item;
	double Weight;
	int Count;
	int Timestamp;
	int OrderId;
};

void InitHarmoneyDllContext() {
	// 初始化HOS的上下文，保证所有类的构建完毕


}

EXPORT const char* __stdcall HOS_GetBuildTime() {
	return BuildTime.c_str();
}

EXPORT void __stdcall HOS_SaveAndExit() {
	// 保存并退出，主要是完成数据库的存储等
	DB->UpdateDatabaseFile();
}

EXPORT void __stdcall HOS_clear_lastError() {
	lastErrorInfo = "";
}

EXPORT const char* __stdcall HOS_get_lastError() {
	return lastErrorInfo.c_str();
}

EXPORT bool __stdcall HOS_DB_InitDatabase(const char* dbFileName) {
	try {
		DB->InitDatabase(dbFileName);
		return true;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = (string)__FUNCTION__ + ": " + ex.GetExceptionMessage();
		return false;
	}
}

EXPORT bool __stdcall HOS_CS_AddItemToCart(const char* ItemProcessedId, int cnt) {
	try {
		cashierSys->AddItemToCart(ItemProcessedId, cnt);
		return true;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = (string)__FUNCTION__ + ": " + ex.GetExceptionMessage();
		return false;
	}
}

EXPORT bool __stdcall HOS_CS_RemoveItemFromCart(const char* ItemProcessedId) {
	try {
		cashierSys->RemoveItemFromCart(ItemProcessedId);
		return true;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = (string)__FUNCTION__ + ": " + ex.GetExceptionMessage();
		return false;
	}
}

EXPORT bool __stdcall HOS_CS_GetCurrentPrice(double* pResult) {
	try {
		*pResult = cashierSys->GetCurrentPrice();
		return true;
	}
	catch (HarmoneyException ex) {
		*pResult = 0;
		lastErrorInfo = (string)__FUNCTION__ + ": " + ex.GetExceptionMessage();
		return false;
	}
}

EXPORT bool __stdcall HOS_CS_GetCurrentPurchaseListSize(int* pResult) {
	try {
		*pResult = cashierSys->GetCurrentPurchaseList().size();
		return true;
	}
	catch (HarmoneyException ex) {
		*pResult = 0;
		lastErrorInfo = (string)__FUNCTION__ + ": " + ex.GetExceptionMessage();
		return false;
	}
}

void CopyPurchaseItemRecord(CPurchaseItemRecordExport& res, CPurchaseItemRecord& src) {
	res.Item.ItemName = src.Item.ItemName.c_str();
	res.Item.ItemId = src.Item.ItemId.c_str();
	res.Item.Cost = src.Item.Cost;
	res.Item.Price = src.Item.Price;
	res.Item.ItemType = src.Item.ItemType;
	res.Count = src.Count;
	res.OrderId = src.OrderId;
	res.Timestamp = src.Timestamp;
	res.Weight = src.Weight;
}

EXPORT bool __stdcall HOS_CS_GetCurrentPurchaseList(CPurchaseItemRecordExport* pList, int listSize) {
	// 必须保证pList内存空间充足，大于等于ListSize
	if (listSize < cashierSys->GetCurrentPurchaseList().size()) {
		lastErrorInfo = __FUNCTION__": 给定的列表空间不足！";
		return false;
	}
	try {
		auto list = cashierSys->GetCurrentPurchaseList();
		for (int i = 0; i < list.size(); i++) {
			CPurchaseItemRecordExport cur = { 0 };
			CopyPurchaseItemRecord(cur, list[i]);
			*(pList + i) = cur;
		}
		return true;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = ex.GetExceptionMessage();
		return false;
	}
}
vector<CPurchaseItemRecord> tmp;

EXPORT bool __stdcall HOS_CS_Checkout() {
	try {
		cashierSys->Checkout(tmp);
		return true;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = ex.GetExceptionMessage();
		return false;
	}
}

string testString = "中文!!!";

EXPORT void __stdcall HOS_test(CItemInfoExport& ret) {
	ret.Cost = 100;
	ret.ItemId = testString.c_str();
	ret.ItemName = testString.c_str();
	ret.ItemType = 123;
	ret.Price = 1234;
}