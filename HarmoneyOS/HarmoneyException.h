#pragma once
#include <string>
/*
	这是一个 HarmoneyOS 异常类，用于在出现意外错误的代码中抛出
	方法: throw HarmoneyException("Some Information...");
	抛出此异常的方法将被单元测试检测到，并视作代码错误！
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

