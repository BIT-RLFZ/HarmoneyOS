#pragma once
#include <string>
/*
	����һ�� HarmoneyOS �쳣�࣬������δʵ�ֵĴ��봦�׳�
	����: throw NoImplException(__FUNCTION__);
	�׳����쳣�ķ��������ܵ�Ԫ���ԵĲ������ƣ�Ҳ���ǵ�Ԫ���Ի�������ֺ���������Ϊ���Գ���
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

