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

namespace HarmoneyOSTest
{
	TEST_CLASS(HarmoneyOS_Database_Test)
	{
	public:
		Database database;

		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			Logger::WriteMessage("[Database_Test] Start to Unittest Database Module.\n");
			//删除以前的测试数据库文件，保证数据库是全新的以保证测试的正确性
			remove("test.osdb");
			//随机化种子，供后续随机加入商品使用
			srand((unsigned)time(0));
		}
		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			Logger::WriteMessage("[Database_Test] Unittest Database Module Finished.\n");
		}

		/*
			数据库初始化测试
			数据库初始化时会读入数据库文件，解析数据库文件并返回
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
				Logger::WriteMessage(ex.GetMessage().c_str());
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
		TEST_METHOD(AddItemStorageInfoTest)
		{
			try {
				
			}
			catch (NoImplException ex) {
				Logger::WriteMessage((ex.GetFunctionName() + "() throw NoImplException! (ignore)\n").c_str());
			}
			catch (HarmoneyException ex) {
				Logger::WriteMessage("[Database_Test] Test Failed!\n");
				Logger::WriteMessage(ex.GetMessage().c_str());
				Assert::Fail();
			}
		}
	};
}