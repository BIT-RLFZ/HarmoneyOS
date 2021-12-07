#include "ItemManager.h"
#include <time.h>
#include <string.h>
#include <iostream>
ItemManager* itemMgr = new ItemManager(); // 给全局用
//时间戳（北京时间）
int stamp_to_standard(int stampTime)
{
	time_t tick = (time_t)stampTime;
	struct tm tm;
	char s[100];
	localtime_s(&tm,&tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);//其中s是一个字符串，表示着时间戳转换过来的真实时间
	int Y1 = 0, M1 = 0, D1 = 0;
	Y1 += (s[0] - '0') * 1000;
	Y1 += (s[1] - '0') * 100;
	Y1 += (s[2] - '0') * 10;
	Y1 += (s[3] - '0') * 1;
	M1 += (s[5] - '0') * 10;
	M1 += (s[6] - '0') * 1;
	int totMonth = (Y1 - 1970) * 12 + M1;//totMonth表示时间戳转换过来之后是1970年开始的有了几个月，其中1970年1月是第一个月。
	return totMonth;
}
int ItemManager::TimestampConvertToMonthNumber(const int& Timestamp)//时间戳转换成第几个月
{
	return stamp_to_standard(Timestamp);//这里调用了时间戳转换成真实日期，并返回是第几个月
}//今后如果想换成年度统计的话，可以直接把月份-1除以12然后加1变成第几年，第一年是第一年（不是第零年）
