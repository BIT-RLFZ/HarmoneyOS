#pragma once
class CPurchaseOrderRecord
{
public:
	int OrderId;	//�������
	int Timestamp;	//ʱ���
	/*
		CPurchaseOrderRecord���޲ι���
	*/
	CPurchaseOrderRecord();	
	/*
		�вι���
		����Ϊ������Ա����ֵ
	*/
	CPurchaseOrderRecord(int OrderId, int Timestamp);
};

