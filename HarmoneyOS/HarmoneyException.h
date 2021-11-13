#pragma once
#include <string>
/*
	����һ�� HarmoneyOS �쳣�࣬�����ڳ����������Ĵ������׳�
	����: throw HarmoneyException("Some Information...");
	�׳����쳣�ķ���������Ԫ���Լ�⵽���������������
*/
class HarmoneyException
{
private:
	std::string ExceptionMessage;
public:
	HarmoneyException(const char* Message) {
		ExceptionMessage = Message;
	}
	std::string& GetMessage() {
		return ExceptionMessage;
	}
};

