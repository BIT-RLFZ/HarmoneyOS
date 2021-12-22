#include <Windows.h>
#include "../HarmoneyOS/Database.h"
#include "../HarmoneyOS/ItemManager.h"
#include "../HarmoneyOS/CashierSystem.h"
#include <string>
#include <sstream>
#include <set>

using namespace std;

extern ItemManager* itemMgr;
extern CashierSystem* cashierSys;
extern Database* DB;




#define EXPORT extern "C" _declspec (dllexport)

string lastErrorInfo = "";

const string BuildTime = (string)__DATE__ + " " + __TIME__;

#pragma pack(4)
struct CItemInfoExport {
	const char* ItemName;
	double Cost;
	double Price;
	const char* ItemId;
	int ItemType;
};
#pragma pack(4)
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

EXPORT int __stdcall HOS_DB_InitDatabase(const char* dbFileName) {
	try {
		DB->InitDatabase(dbFileName);
		return 1;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = (string)__FUNCTION__ + ": " + ex.GetExceptionMessage();
		return 0;
	}
}

void CopyItemInfo(CItemInfoExport& res, CItemInfo& src);

EXPORT int __stdcall HOS_DB_QueryProduct(const char* ItemProcessedId, CItemInfoExport* ret) {
	try {
		string processedid = ItemProcessedId;
		string itemID = processedid.substr(0, 8);
		auto res = DB->QueryItemStorageInfo(itemID);
		CopyItemInfo(*ret, res.Item);
		return 1;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = (string)__FUNCTION__ + ": " + ex.GetExceptionMessage();
		return 0;
	}
}

EXPORT int __stdcall HOS_CS_AddItemToCart(const char* ItemProcessedId, int cnt) {
	try {
		cashierSys->AddItemToCart(ItemProcessedId, cnt);
		return 1;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = (string)__FUNCTION__ + ": " + ex.GetExceptionMessage();
		return 0;
	}
}

EXPORT int __stdcall HOS_CS_RemoveItemFromCart(const char* ItemProcessedId) {
	try {
		cashierSys->RemoveItemFromCart(ItemProcessedId);
		return 1;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = (string)__FUNCTION__ + ": " + ex.GetExceptionMessage();
		return 0;
	}
}

EXPORT int __stdcall HOS_CS_GetCurrentPrice(double* pResult) {
	try {
		*pResult = cashierSys->GetCurrentPrice();
		return 1;
	}
	catch (HarmoneyException ex) {
		*pResult = 0;
		lastErrorInfo = (string)__FUNCTION__ + ": " + ex.GetExceptionMessage();
		return 0;
	}
}

EXPORT int __stdcall HOS_CS_GetCurrentPurchaseListSize(int* pResult) {
	try {
		*pResult = cashierSys->GetCurrentPurchaseList().size();
		return 1;
	}
	catch (HarmoneyException ex) {
		*pResult = 0;
		lastErrorInfo = (string)__FUNCTION__ + ": " + ex.GetExceptionMessage();
		return 0;
	}
}

map<string, char*> mp;
int cnt = 0;

void CopyItemInfo(CItemInfoExport& res, CItemInfo& src) {
	if (!mp.count(src.ItemName)) {
		char* buf = new char[src.ItemName.size()];
		strcpy_s(buf, 999999, src.ItemName.c_str());
		mp[src.ItemName] = buf;
	}
	if (!mp.count(src.ItemId)) {
		char* buf = new char[src.ItemId.size()];
		strcpy_s(buf, 999999, src.ItemId.c_str());
		mp[src.ItemId] = buf;
	}
	res.ItemName = mp[src.ItemName];
	res.ItemId = mp[src.ItemId];
	res.Cost = src.Cost;
	res.Price = src.Price;
	res.ItemType = src.ItemType;
}

void CopyPurchaseItemRecord(CPurchaseItemRecordExport& res, CPurchaseItemRecord& src) {
	if (!mp.count(src.Item.ItemName)) {
		char* buf = new char[src.Item.ItemName.size()];
		strcpy_s(buf, 999999, src.Item.ItemName.c_str());
		mp[src.Item.ItemName] = buf;
	}
	if (!mp.count(src.Item.ItemId)) {
		char* buf = new char[src.Item.ItemId.size()];
		strcpy_s(buf, 999999, src.Item.ItemId.c_str());
		mp[src.Item.ItemId] = buf;
	}
	res.Item.ItemName = mp[src.Item.ItemName];
	res.Item.ItemId = mp[src.Item.ItemId];
	res.Item.Cost = src.Item.Cost;
	res.Item.Price = src.Item.Price;
	res.Item.ItemType = src.Item.ItemType;
	res.Count = src.Count;
	res.OrderId = src.OrderId;
	res.Timestamp = src.Timestamp;
	res.Weight = src.Weight;
}

EXPORT int __stdcall HOS_CS_GetCurrentPurchaseList(CPurchaseItemRecordExport* pList, int listSize) {
	// 必须保证pList内存空间充足，大于等于ListSize
	if (listSize < (int)cashierSys->GetCurrentPurchaseList().size()) {
		lastErrorInfo = __FUNCTION__": 给定的列表空间不足！";
		return 0;
	}
	char fuckyou[100] = { 0 };
	try {
		auto list = cashierSys->GetCurrentPurchaseList();
		for (int i = 0; i < list.size(); i++) {
			CPurchaseItemRecordExport cur = { 0 };
			CopyPurchaseItemRecord(cur, list[i]);
			*(pList + i) = cur;
			//sprintf_s(fuckyou, "name: addr=%d id=%d", cur.Item.ItemName,cur.Item.ItemId);
			//MessageBoxA(0, fuckyou, fuckyou, 0);
		}
		return 1;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = ex.GetExceptionMessage();
		return 0;
	}
}
vector<CPurchaseItemRecord> tmp;

EXPORT int __stdcall HOS_CS_Checkout() {
	try {
		cashierSys->Checkout(tmp);
		return 1;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = ex.GetExceptionMessage();
		return 0;
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

const int TYPE_CNTPRO = 0;
const int TYPE_WEIGHTPRO = 1;

void internal_addProduct(string name, string itemID, int itemType, double price, double cost, int cntRest, double weightRest) {
	CItemStorageInfo info;
	info.Item.ItemName = name;
	info.Item.Cost = cost;
	info.Item.ItemType = itemType;
	info.Item.Price = price;
	info.Item.ItemId = itemID;
	info.IsDelete = false;
	info.CountRest = cntRest;
	info.WeightRest = weightRest;
	info.Timestamp = time(0);
	DB->AddItemStorageInfo(info);
}

EXPORT void __stdcall HOS_DBG_AddExampleProducts() {
	internal_addProduct("个数商品1", "00000001", TYPE_CNTPRO, 10, 5, 100, 0);
	internal_addProduct("个数商品2", "00000002", TYPE_CNTPRO, 8, 2, 300, 0);
	internal_addProduct("个数商品3", "00000003", TYPE_CNTPRO, 15, 10, 250, 0);
	internal_addProduct("称重商品4", "00000004", TYPE_WEIGHTPRO, 5, 4, 0, 200);
	internal_addProduct("称重商品5", "00000005", TYPE_WEIGHTPRO, 10, 8, 0, 200);
	internal_addProduct("称重商品6", "00000006", TYPE_WEIGHTPRO, 20, 15, 0, 300);
}


EXPORT int __stdcall HOS_Mm_GetItemInfoStruSize() {
	return sizeof(CItemInfoExport);
}

EXPORT int __stdcall HOS_Mm_GetItemRecordInfoStruSize() {
	return sizeof(CPurchaseItemRecordExport);
}

EXPORT int __stdcall HOS_Mm_Allocate(int size) {
	return (int)(new char[size]);
}

EXPORT void __stdcall HOS_Mm_DeleteMem(void* address) {
	delete address;
}

EXPORT int __stdcall HOS_DBG_LoadExampleProducts(const char* productTxt) {
	stringstream ssm(productTxt);
	string productId, productName;
	double cost, price;
	int type;
	int cnt = 0;
	while (ssm >> productId >> productName >> cost >> price >> type) {
		cnt++;
		CItemStorageInfo cur;
		cur.Item.Cost = cost;
		cur.Item.ItemId = productId;
		cur.Item.ItemName = productName;
		cur.Item.ItemType = type;
		cur.Item.Price = price;
		cur.IsDelete = false;
		cur.CountRest = cur.Item.ItemType ? 0 : 100;
		cur.Timestamp = time(0);
		cur.WeightRest = cur.Item.ItemType ? 100 : 0;
		DB->AddItemStorageInfo(cur);
	}
	return cnt;
}

EXPORT int __stdcall HOS_DB_GetItemCountRest(const char* productId) {
	try {
		auto res = DB->QueryItemStorageInfo(productId);
		return res.CountRest;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = ex.GetExceptionMessage();
		return 0;
	}
}

EXPORT double __stdcall HOS_DB_GetItemWeightRest(const char* productId) {
	try {
		auto res = DB->QueryItemStorageInfo(productId);
		return res.WeightRest;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = ex.GetExceptionMessage();
		return 0;
	}
}

EXPORT int __stdcall HOS_DB_SetItemCountRest(const char* productId, int val) {
	try {
		auto res = DB->QueryItemStorageInfo(productId);
		res.CountRest = val;
		DB->ModifyItemStorageInfo(res);
		return true;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = ex.GetExceptionMessage();
		return 0;
	}
}

EXPORT int __stdcall HOS_DB_SetItemWeightRest(const char* productId,double val) {
	try {
		auto res = DB->QueryItemStorageInfo(productId);
		res.WeightRest = val;
		DB->ModifyItemStorageInfo(res);
		return true;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = ex.GetExceptionMessage();
		return 0;
	}
}

EXPORT int __stdcall HOS_DB_ModifyItem(const char* productId, const char* productName, int productType, double cost, double price,int countRest, double weightRest) {
	try {
		auto res = DB->QueryItemStorageInfo(productId);
		res.Item.ItemName = productName;
		res.Item.Cost = cost;
		res.Item.ItemType = productType;
		res.Item.Price = price;
		res.CountRest = countRest;
		res.WeightRest = weightRest;
		res.Timestamp = time(0);
		DB->ModifyItemStorageInfo(res);
		return true;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = ex.GetExceptionMessage();
		return 0;
	}
}

EXPORT int __stdcall HOS_DB_AddItem(const char* productId, const char* productName, int productType, double cost, double price, int countRest, double weightRest) {
	try {
		CItemStorageInfo cur;
		cur.IsDelete = false;
		cur.Item.ItemId = productId;
		cur.Item.ItemName = productName;
		cur.Item.Cost = cost;
		cur.Item.ItemType = productType;
		cur.Item.Price = price;
		cur.CountRest = countRest;
		cur.WeightRest = weightRest;
		cur.Timestamp = time(0);
		DB->AddItemStorageInfo(cur);
		return true;
	}
	catch (HarmoneyException ex) {
		lastErrorInfo = ex.GetExceptionMessage();
		return 0;
	}
}