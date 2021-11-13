/*
	�������ݿ���ȷ�Եĵ�Ԫ����
	�������β���
	�벻Ҫ���������������ݣ�
*/
#include "pch.h"
#include "CppUnitTest.h"
#include "../HarmoneyOS/Database.h"
#include "../HarmoneyOS/NoImplException.h"
#include "../HarmoneyOS/HarmoneyException.h"
#include <map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HarmoneyOSTest
{
	TEST_CLASS(HarmoneyOS_Database_Test)
	{
	public:
		Database database;
		const int ItemsCount = 1000;
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			Logger::WriteMessage("[Database_Test] Start to Unittest Database Module.\n");
			// ɾ����ǰ�Ĳ������ݿ��ļ�����֤���ݿ���ȫ�µ��Ա�֤���Ե���ȷ��
			remove("test.osdb");
		}
		HarmoneyOS_Database_Test() {
			// ��������ӣ����������������Ʒʹ��
			srand((unsigned)time(0));
			// �������һ����Ʒ
			GenerateItems(ItemsCount);
		}
		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			Logger::WriteMessage("[Database_Test] Unittest Database Module Finished.\n");
		}

		/*
			���ݿ��ʼ������
			���ݿ��ʼ��ʱ��������ݿ��ļ����������ݿ��ļ�������
		*/
		TEST_METHOD(DatabaseInitTest)
		{
			try {
				auto result = database.InitDatabase("test.osdb");
				if (result) Logger::WriteMessage("[Database_Test] Database Init Test OK!\n");
				else Assert::Fail(L"[Database_Test] Database Init Test Failed!\n");
			}
			catch (NoImplException ex) {
				Logger::WriteMessage((ex.GetFunctionName() + "() throw NoImplException! (ignore)\n").c_str());
			}
			catch (HarmoneyException ex) {
				Logger::WriteMessage("[Database_Test] Test Failed!\n");
				Logger::WriteMessage(("[Exception] " + ex.GetMessage()).c_str());
				Assert::Fail();
			}
		}

		/*
			��ȡһ����һ�޶��������Ʒ��
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
				CItemInfo cur;
				cur.ItemName = GetUniqueRandomName(15);
				cur.Cost = rand() % 100 + 10;
				cur.Price = cur.Cost + rand() % 20 + 1;
				cur.ItemId = 1145140000 + i;
				cur.ItemType = rand() % 2;
				items.push_back(cur);
			}
		}

		/*
			�������һ����Ʒ�����Ϣ
		*/
		CItemStorageInfo GenerateRandomItemStorageInfo() {
			auto rd = rand();
			CItemStorageInfo ret;
			
		}
		/*
			������ӿ��Ĺ��ܣ���Ҫ�����Ƿ�Ҫ��ϲ���ȷ
		*/
		TEST_METHOD(AddItemStorageInfoTest)
		{
			try {
				for (int i = 0; i < items.size(); i++) {
					auto item = items[i];
					CItemStorageInfo ret;
					if (item.ItemType == 0) {
						// ��������
						ret.CountRest = rand() % 100 + 10;
						ret.IsDelete = false;
						ret.Item = item;
						ret.Timestamp = time(0);
						ret.WeightRest = 0;
					}
					else if (item.ItemType == 1) {
						// ����
						ret.CountRest = 0;
						ret.IsDelete = false;
						ret.Item = item;
						ret.Timestamp = time(0);
						ret.WeightRest = (rand() + 200) / 100.0f + 10.0f;
					}
					auto status = database.AddItemStorageInfo(ret);
					if (!status) Assert::Fail();
					double newCost = ret.Item.Cost + 30;
					double newPrice = ret.Item.Price + 40;
					ret.Item.Cost = newCost;
					ret.Item.Price = newPrice;
					status = database.AddItemStorageInfo(ret);
					if (!status) Assert::Fail();
					auto dataRes = database.QueryItemStorageInfo(ret.Item.ItemId);
					if (dataRes.Item.ItemName != item.ItemName) Assert::Fail();
					if (dataRes.Item.Cost != newCost) Assert::Fail();
					if (dataRes.Item.Price != newPrice) Assert::Fail();
					if (dataRes.Item.ItemType == 0 && dataRes.CountRest != ret.CountRest * 2) Assert::Fail();
					if (dataRes.Item.ItemType == 1 && dataRes.WeightRest != ret.WeightRest * 2) Assert::Fail();
				}
				auto allStorageInfo = database.GetAllItemStorageInfo();
				if (allStorageInfo.size() != items.size()) Assert::Fail();
			}
			catch (NoImplException ex) {
				Logger::WriteMessage((ex.GetFunctionName() + "() throw NoImplException! (ignore)\n").c_str());
			}
			catch (HarmoneyException ex) {
				Logger::WriteMessage("[Database_Test] Test Failed!\n");
				Logger::WriteMessage(("[Exception] " + ex.GetMessage()).c_str());
				Assert::Fail();
			}
		}

		TEST_METHOD(ModifyItemStorageInfoTest) {
			try {
				for (int i = 0; i < items.size(); i++) {
					auto item = items[i];
					auto itemInDB = database.QueryItemStorageInfo(item.ItemId);
					if (itemInDB.Item.ItemType == 0) {
						itemInDB.CountRest -= 3;
						itemInDB.Timestamp = time(0);
						auto sta = database.ModifyItemStorageInfo(itemInDB);
						if (!sta) Assert::Fail();
						auto nowItemInDB = database.QueryItemStorageInfo(item.ItemId);
						if (nowItemInDB.CountRest != itemInDB.CountRest) Assert::Fail();
						if (nowItemInDB.Timestamp != itemInDB.Timestamp) Assert::Fail();
					}
					else if (itemInDB.Item.ItemType == 1) {
						itemInDB.WeightRest -= 4;
						itemInDB.Timestamp = time(0);
						auto sta = database.ModifyItemStorageInfo(itemInDB);
						if (!sta) Assert::Fail();
						auto nowItemInDB = database.QueryItemStorageInfo(item.ItemId);
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
				Logger::WriteMessage(("[Exception] " + ex.GetMessage()).c_str());
				Assert::Fail();
			}
		}

		TEST_METHOD(DeleteItemStorageInfoTest) {
			try {
				for (int i = 0; i < items.size(); i++) {
					auto item = items[i];
					database.DeleteItemStorageInfo(item.ItemId);
					auto itemInDB = database.QueryItemStorageInfo(item.ItemId);
					if (!itemInDB.IsDelete) Assert::Fail();
				}
			}
			catch (NoImplException ex) {
				Logger::WriteMessage((ex.GetFunctionName() + "() throw NoImplException! (ignore)\n").c_str());
			}
			catch (HarmoneyException ex) {
				Logger::WriteMessage("[Database_Test] Test Failed!\n");
				Logger::WriteMessage(("[Exception] " + ex.GetMessage()).c_str());
				Assert::Fail();
			}
		}
	};
}