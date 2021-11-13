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

		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			Logger::WriteMessage("[Database_Test] Start to Unittest Database Module.\n");
			//ɾ����ǰ�Ĳ������ݿ��ļ�����֤���ݿ���ȫ�µ��Ա�֤���Ե���ȷ��
			remove("test.osdb");
			//��������ӣ����������������Ʒʹ��
			srand((unsigned)time(0));
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
				Logger::WriteMessage(ex.GetMessage().c_str());
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