#pragma once
#include "CPurchaseItemRecord.h"
#include <string>
#include <vector>
class CashierSystem
{
private:
	std::vector<CPurchaseItemRecord> Cart;	// ���ﳵ����vector��¼
	int OrderId;	// ��ǰ�����ı��
	int Timestamp;	//��ǰ������ʱ���
	void generateOrderId(); //���ɵ�ǰ�����Ķ������
	void generateTimestamp();	// ���ɵ�ǰ������ʱ���
public:
	/*
		�����Ʒ����ǰ���ﳵ
		����: ItemProcessedId string Ҫ�����Ʒ�ı��(�������)
			  cnt int ������Ʒ������
		����ֵ�� bool �������Ӳ����Ƿ�ɹ�ִ��
	*/
	bool AddItemToCart(std::string ItemProcessedId,int cnt);
	/*
		�ӵ�ǰ���ﳵ��ɾ����Ʒ
		������ItemProcessedId string Ҫɾ������Ʒ�ı��(�������)
		����ֵ��bool ����ɾ�������Ƿ�ɹ�ִ��
	*/
	bool RemoveItemFromCart(std::string ItemProcessedId);
	/* 
	*	��õ�ǰ���ﳵ�л�����ܼ�ֵ
		����ֵ: double ���ﳵ�е�ǰ������ܼ�ֵ
	*/
	double GetCurrentPrice();
	/*
		��ȡ��ǰ���ﳵ�е���Ʒ��¼��Ϣ
		����ֵ ��vector<CPurchaseItemRecord> ���ص�vector�а������ﳵ��������Ʒ��Ϣ
	*/
	std::vector<CPurchaseItemRecord>& GetCurrentPurchaseList();
	/*
		�����ˣ���չ��ﳵ
		������CurrentPurchaseList vector<CPurchaseItemRecord>& ����һ��vector�Ա��ӡСƱ
		����ֵ��������ز����Ƿ�ɹ�
	*/
	bool Checkout(std::vector<CPurchaseItemRecord>& CurrentPurchaseList);
};

