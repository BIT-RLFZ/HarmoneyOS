#pragma once
#include <string>
/*
	����һ�� HarmoneyOS �쳣�࣬������δʵ�ֵĴ��봦�׳�
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

