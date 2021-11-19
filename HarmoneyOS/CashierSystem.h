#pragma once
#include "CPurchaseItemRecord.h"
#include "Database.h"
#include <string>
#include <vector>
#include "GlobalVar.h"
/*
	�ڹ˿�ÿ�ν���ǰ��Ҫ�½�һ��CashierSystem����
	ͨ��ִ�й��췽�������µĶ���ID��ʱ���
*/
class CashierSystem
{
private:
	std::vector<CPurchaseItemRecord> Cart;	// ���ﳵ����vector��¼
	int OrderId;	// ��ǰ�����ı��
	int Timestamp;	//��ǰ������ʱ���
	void generateOrderId(); //���ɵ�ǰ�����Ķ������
	void generateTimestamp();	// ���ɵ�ǰ������ʱ���
	bool CheckCode(const std::string& value, int code);	//����ƷID����У��
	std::string processId(const std::string& ItemProcessedId, int &Type, double &Weight);
public:
	/*
		��ʼ�������ı�ź���ʱ����ȳ�ʼ��Ϣ
	*/
	CashierSystem();
	/*
		�����Ʒ����ǰ���ﳵ
		����: ItemProcessedId string Ҫ�����Ʒ�ı��(�������)
			  cnt int ������Ʒ������
		����ֵ�� bool �������Ӳ����Ƿ�ɹ�ִ��
	*/
	bool AddItemToCart(const std::string& ItemProcessedId,int cnt);
	/*
		�ӵ�ǰ���ﳵ��ɾ����Ʒ
		������ItemProcessedId string Ҫɾ������Ʒ�ı��(�������)
		����ֵ��bool ����ɾ�������Ƿ�ɹ�ִ��
	*/
	bool RemoveItemFromCart(const std::string& ItemProcessedId);
	/* 
	*	��õ�ǰ���ﳵ�л�����ܼ�ֵ
		����ֵ: double ���ﳵ�е�ǰ������ܼ�ֵ
	*/
	double GetCurrentPrice();
	/*
		��ȡ��ǰ���ﳵ�е���Ʒ��¼��Ϣ
		����ֵ ��vector<CPurchaseItemRecord> ���ص�vector�а������ﳵ��������Ʒ��Ϣ
	*/
	const std::vector<CPurchaseItemRecord>& GetCurrentPurchaseList();
	/*
		�����ˣ���չ��ﳵ
		������CurrentPurchaseList vector<CPurchaseItemRecord>& ����һ��vector�Ա��ӡСƱ
		����ֵ��������ز����Ƿ�ɹ�
	*/
	bool Checkout(std::vector<CPurchaseItemRecord>& CurrentPurchaseList);
};

