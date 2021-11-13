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
			���ݿ��ʼ������
			���ݿ��ʼ��ʱ��������ݿ��ļ����������ݿ��ļ�������
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