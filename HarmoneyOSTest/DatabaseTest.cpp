/*
	测试数据库正确性的单元测试
	所属：任波翰
	请不要随意更改里面的内容！
*/
#include "pch.h"
#include "CppUnitTest.h"
#include "../HarmoneyOS/Database.h"
#include "../HarmoneyOS/NoImplException.h"
#include "../HarmoneyOS/HarmoneyException.h"
#include <map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
Database* database = new Database();

namespace HarmoneyOSTest
{
	TEST_CLASS(HarmoneyOS_Database_Test)
	{
	public:
		// 一定要以对象形式建立！！否则~构析函数会有问题！
		const int ItemsCount = 1000;
		HarmoneyOS_Database_Test() {
			Logger::WriteMessage("[Database_Test] Start to Unittest Database Module.\n");
			// 删除以前的测试数据库文件，保证数据库是全新的以保证测试的正确性
			remove("test.osdb");

			// 随机化种子，供后续随机加入商品使用
			srand((unsigned)time(0));
			// 随机生成一堆商品
			GenerateItems(ItemsCount);
		}
		/*
			数据库初始化测试
			数据库初始化时会读入数据库文件，解析数据库文件并返回
		*/

		TEST_METHOD(t1_DatabaseInitTest)
		{
			try {
				auto result = database->InitDatabase("test.osdb");
				if (result) Logger::WriteMessage("[Database_Test] Database Init Test OK!\n");
				else Assert::Fail(L"[Database_Test] Database Init Test Failed!\n");
			}
			catch (NoImplException ex) {
				Logger::WriteMessage((ex.GetFunctionName() + "() throw NoImplException! (ignore)\n").c_str());
			}
			catch (HarmoneyException ex) {
				Logger::WriteMessage("[Database_Test] Test Failed!\n");
				Logger::WriteMessage(("[Exception] " + ex.GetExceptionMessage()).c_str());
				Assert::Fail();
			}
		}

		/*
			获取一个独一无二的随机商品名
		*/
		std::map<std::string, bool> RdNameMp;
		std::string GetUniqueRandomName(int length) {
			std::string ret = "";
			for (int i = 0; i < length; i++) {
				auto rd = rand();
				if (rd % 3 == 0) ret += (char)(rand() % 26 + 'a');
				else if(rd % 3 == 1) ret += (char)(rand() % 26 + 'A');
				else ret += (char)(rand() % 10 + '0');
			}
			if (RdNameMp.count(ret)) return GetUniqueRandomName(length);
			RdNameMp[ret] = true;
			return ret;
		}
		std::vector<CItemInfo> items;
		void GenerateItems(int count) {
			for (int i = 0; i < count; i++) {
				static char num[10] = { 0 };
				sprintf_s(num, "%d", i);
				CItemInfo cur;
				cur.ItemName = GetUniqueRandomName(15);
				cur.Cost = rand() % 100 + 10;
				cur.Price = cur.Cost + rand() % 20 + 1;
				cur.ItemId = (std::string)"1145140" + num;
				cur.ItemType = rand() % 2;
				items.push_back(cur);
			}
		}

		/*
			随机生成一个商品库存信息
		*/
		CItemStorageInfo GenerateRandomItemStorageInfo() {
			auto rd = rand();
			CItemStorageInfo ret;
			
		}
		/*
			测试添加库存的功能，还要测试是否按要求合并正确
		*/
		TEST_METHOD(t2_AddItemStorageInfoTest)
		{
			try {
				for (int i = 0; i < items.size(); i++) {
					auto item = items[i];
					CItemStorageInfo ret;
					if (item.ItemType == 0) {
						// 按个数卖
						ret.CountRest = rand() % 100 + 10;
						ret.IsDelete = false;
						ret.Item = item;
						ret.Timestamp = time(0);
						ret.WeightRest = 0;
					}
					else if (item.ItemType == 1) {
						// 按斤
						ret.CountRest = 0;
						ret.IsDelete = false;
						ret.Item = item;
						ret.Timestamp = time(0);
						ret.WeightRest = (rand() + 200) / 100.0f + 10.0f;
					}
					auto status = database->AddItemStorageInfo(ret);
					if (!status) Assert::Fail();
					double newCost = ret.Item.Cost + 30;
					double newPrice = ret.Item.Price + 40;
					ret.Item.Cost = newCost;
					ret.Item.Price = newPrice;
					status = database->AddItemStorageInfo(ret);
					if (!status) Assert::Fail();
					auto dataRes = database->QueryItemStorageInfo(ret.Item.ItemId);
					if (dataRes.Item.ItemName != item.ItemName) Assert::Fail();
					if (dataRes.Item.Cost != newCost) Assert::Fail();
					if (dataRes.Item.Price != newPrice) Assert::Fail();
					if (dataRes.Item.ItemType == 0 && dataRes.CountRest != ret.CountRest * 2) Assert::Fail();
					if (dataRes.Item.ItemType == 1 && dataRes.WeightRest != ret.WeightRest * 2) Assert::Fail();
				}
				auto allStorageInfo = database->GetAllItemStorageInfo();
				if (allStorageInfo.size() != items.size()) Assert::Fail();
			}
			catch (NoImplException ex) {
				Logger::WriteMessage((ex.GetFunctionName() + "() throw NoImplException! (ignore)\n").c_str());
			}
			catch (HarmoneyException ex) {
				Logger::WriteMessage("[Database_Test] Test Failed!\n");
				Logger::WriteMessage(("[Exception] " + ex.GetExceptionMessage()).c_str());
				Assert::Fail();
			}
		}
		TEST_METHOD(t3_ModifyItemStorageInfoTest) {
			try {
				for (int i = 0; i < items.size(); i++) {
					auto item = items[i];
					auto itemInDB = database->QueryItemStorageInfo(item.ItemId);
					if (itemInDB.Item.ItemType == 0) {
						itemInDB.CountRest -= 3;
						itemInDB.Timestamp = time(0);
						auto sta = database->ModifyItemStorageInfo(itemInDB);
						if (!sta) Assert::Fail();
						auto nowItemInDB = database->QueryItemStorageInfo(item.ItemId);
						if (nowItemInDB.CountRest != itemInDB.CountRest) Assert::Fail();
						if (nowItemInDB.Timestamp != itemInDB.Timestamp) Assert::Fail();
					}
					else if (itemInDB.Item.ItemType == 1) {
						itemInDB.WeightRest -= 4;
						itemInDB.Timestamp = time(0);
						auto sta = database->ModifyItemStorageInfo(itemInDB);
						if (!sta) Assert::Fail();
						auto nowItemInDB = database->QueryItemStorageInfo(item.ItemId);
						if (nowItemInDB.WeightRest != itemInDB.WeightRest) Assert::Fail();
						if (nowItemInDB.Timestamp != itemInDB.Timestamp) Assert::Fail();
					}
				}
			}
			catch (NoImplException ex) {
				Logger::WriteMessage((ex.GetFunctionName() + "() throw NoImplException! (ignore)\n").c_str());
			}
			catch (HarmoneyException ex) {
				Logger::WriteMessage("[Database_Test] Test Failed!\n");
				Logger::WriteMessage(("[Exception] " + ex.GetExceptionMessage()).c_str());
				Assert::Fail();
			}
		}
		TEST_METHOD(t4_DeleteItemStorageInfoTest) {
			try {
				printf("dbsize: %d\n", database->GetAllItemStorageInfo().size());
				for (int i = 0; i < items.size(); i++) {
					auto item = items[i];
					database->DeleteItemStorageInfo(item.ItemId);
					auto itemInDB = database->QueryItemStorageInfo(item.ItemId);
					if (!itemInDB.IsDelete) Assert::Fail();
				}
			}
			catch (NoImplException ex) {
				Logger::WriteMessage((ex.GetFunctionName() + "() throw NoImplException! (ignore)\n").c_str());
			}
			catch (HarmoneyException ex) {
				Logger::WriteMessage("[Database_Test] Test Failed!\n");
				Logger::WriteMessage(("[Exception] " + ex.GetExceptionMessage()).c_str());
				Assert::Fail();
			}
		}

		/*
			测试数据库存储文件的正确性，以及读取文件的正确性
			第一部分：测试物品信息的读入存储
		*/
		TEST_METHOD(t5_DBStorageFileTest_1) {
			try {
				int previousSize = database->GetAllItemStorageInfo().size();
				database->UpdateDatabaseFile();
				database->InitDatabase("test.osdb");
				auto storage = database->GetAllItemStorageInfo();
				if(storage.size() != previousSize) Assert::Fail();

				CItemStorageInfo testInfo;
				testInfo.CountRest = 123;
				testInfo.IsDelete = false;
				testInfo.Timestamp = time(0);
				testInfo.WeightRest = 0;
				testInfo.Item.ItemName = "测试物品";
				testInfo.Item.Cost = 233;
				testInfo.Item.ItemId = "123456888";
				testInfo.Item.ItemType = 0;
				testInfo.Item.Price = 999;
				database->AddItemStorageInfo(testInfo);
				database->UpdateDatabaseFile();
				database->InitDatabase("test.osdb");
				auto result = database->QueryItemStorageInfo("123456888");
				if(result.Item.ItemName != "测试物品") Assert::Fail();
				if (result.Item.Cost != 233) Assert::Fail();
				if (result.Item.Price != 999) Assert::Fail();
				if (result.Item.ItemType != 0) Assert::Fail();
				if (result.CountRest != 123) Assert::Fail();
			}
			catch (NoImplException ex) {
				Logger::WriteMessage((ex.GetFunctionName() + "() throw NoImplException! (ignore)\n").c_str());
			}
			catch (HarmoneyException ex) {
				Logger::WriteMessage("[Database_Test] Test Failed!\n");
				Logger::WriteMessage(("[Exception] " + ex.GetExceptionMessage()).c_str());
				Assert::Fail();
			}
		}
		
		/*
			测试数据库存储文件的正确性，以及读取文件的正确性
			第二部分：测试物品购买记录的读入存储
		*/
		TEST_METHOD(t5_DBStorageFileTest_2) {
			try {
				database->InitDatabase("test.osdb");
				for (int i = 1; i <= 100; i++) {
					CPurchaseItemRecord pir;
					pir.Item = items[rand() % items.size()];
					pir.Count = 12;
					pir.OrderId = i;
					pir.Timestamp = time(0);
					pir.Weight = 0;
					database->AddPurchaseItemRecord(pir);
				}
				database->UpdateDatabaseFile();
				database->InitDatabase("test.osdb");
				auto all = database->GetAllPurchaseItemRecord();
				int sumCount = 0, sumOrderId = 0, sumWeight = 0;
				for (auto each : all) {
					sumCount += each.Count;
					sumOrderId += each.OrderId;
					sumWeight += each.Weight;
				}
				if (sumCount != 1200) Assert::Fail();
				if (sumOrderId != 5050) Assert::Fail();
				if (sumWeight != 0) Assert::Fail();
			}
			catch (NoImplException ex) {
				Logger::WriteMessage((ex.GetFunctionName() + "() throw NoImplException! (ignore)\n").c_str());
			}
			catch (HarmoneyException ex) {
				Logger::WriteMessage("[Database_Test] Test Failed!\n");
				Logger::WriteMessage(("[Exception] " + ex.GetExceptionMessage()).c_str());
				Assert::Fail();
			}
		}
	};
}