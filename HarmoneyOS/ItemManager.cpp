#include "ItemManager.h"
#include <time.h>
#include <string.h>
#include <iostream>
ItemManager* itemMgr = new ItemManager(); // ��ȫ����
//ʱ���������ʱ�䣩
int stamp_to_standard(int stampTime)
{
	time_t tick = (time_t)stampTime;
	struct tm tm;
	char s[100];
	localtime_s(&tm,&tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);//����s��һ���ַ�������ʾ��ʱ���ת����������ʵʱ��
	int Y1 = 0, M1 = 0, D1 = 0;
	Y1 += (s[0] - '0') * 1000;
	Y1 += (s[1] - '0') * 100;
	Y1 += (s[2] - '0') * 10;
	Y1 += (s[3] - '0') * 1;
	M1 += (s[5] - '0') * 10;
	M1 += (s[6] - '0') * 1;
	int totMonth = (Y1 - 1970) * 12 + M1;//totMonth��ʾʱ���ת������֮����1970�꿪ʼ�����˼����£�����1970��1���ǵ�һ���¡�
	return totMonth;
}
int ItemManager::TimestampConvertToMonthNumber(const int& Timestamp)//ʱ���ת���ɵڼ�����
{
	return stamp_to_standard(Timestamp);//���������ʱ���ת������ʵ���ڣ��������ǵڼ�����
}//�������뻻�����ͳ�ƵĻ�������ֱ�Ӱ��·�-1����12Ȼ���1��ɵڼ��꣬��һ���ǵ�һ�꣨���ǵ����꣩
