#pragma once
class CPurchaseOrderRecord
{
public:
	int OrderId;	//订单编号
	int Timestamp;	//时间戳
	/*
		CPurchaseOrderRecord的无参构造
	*/
	CPurchaseOrderRecord();	
	/*
		有参构造
		方便为两个成员赋初值
	*/
	CPurchaseOrderRecord(int OrderId, int Timestamp);
};

