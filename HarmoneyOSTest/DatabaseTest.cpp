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
				auto result = database.InitDatabase();
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
	};
}