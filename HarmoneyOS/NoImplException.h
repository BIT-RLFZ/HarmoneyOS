#pragma once
#include <string>
/*
	这是一个 HarmoneyOS 异常类，用于在未实现的代码处抛出
	方法: throw NoImplException(__FUNCTION__);
	抛出此异常的方法将不受单元测试的测试限制，也就是单元测试会忽视这种函数，不记为测试出错
*/
class NoImplException
{
private:
	std::string expFunctionName;
public:
	NoImplException(const char* FunctionName) {
		expFunctionName = FunctionName;
	}
	std::string& GetFunctionName() {
		return expFunctionName;
	}
};

