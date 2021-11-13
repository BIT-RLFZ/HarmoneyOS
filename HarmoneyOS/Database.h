#pragma once
#include "CItemStorageInfo.h"
#include "CPurchaseItemRecord.h"
#include "CPurchaseOrderRecord.h"
#include <string>
#include <vector>
class Database
{
public:
	/*
		��ʼ�����ݿ⣬�������ݿ��ļ����ݲ�����
		�ļ�������ʱ���Զ���ʼ��һ���µ����ݿ��ļ�
	*/
	bool InitDatabase(std::string dbFileName="Harmoney.osdb");
	/*
		��ѯָ��ItemId��Ʒ����Ʒ��Ϣ��������CItemStorageInfo
		�����쳣��HarmoneyException
			 �ڣ��޷��ҵ�ָ����ƷIDʱ
	*/
	CItemStorageInfo QueryItemStorageInfo(std::string ItemId);
	/*
		�޸�ָ��ItemId��Ʒ����Ʒ��Ϣ������bool
		�����쳣��HarmoneyException
			 �ڣ��޷��ҵ�ָ����ƷIDʱ
	*/
	bool ModifyItemStorageInfo(CItemStorageInfo& StorageInfo);
	/*
		��ָ����Ʒ�������ݿ⣬�Ѿ����ڵ���Ʒ�ᱻ�ϲ����ϲ������ǿ��ֱ����ӣ����µĶ��󸲸ǽ����ۼ�����
	*/
	bool AddItemStorageInfo(CItemStorageInfo& StorageInfo);
	/*
		��ѯ���ݿ������е���Ʒ�����Ϣ��������Ʒ��Ϣ���б�
		ע�⣡�������Ѿ��¼�(ɾ��)����Ʒ��Ȼ�᷵�أ�ע���ж� IsDelete
	*/
	std::vector<CItemStorageInfo>& GetAllItemStorageInfo();
	/*
		��ָ����Ʒ��Delete״̬���ó�true����ʾ��Ʒ�Ѿ��¼�
		�����쳣��HarmoneyException
			 �ڣ��޷��ҵ�ָ����ƷIDʱ
	*/
	bool DeleteItemStorageInfo(std::string ItemId);

	/*
		���һ����Ʒ�Ĺ����¼��ע��ͬһ�������п��ܻ����ɶ����Ʒ�����¼����Ϊ�����ж����Ʒ������
	*/
	bool AddPurchaseItemRecord(CPurchaseItemRecord& PurchaseItemRecord);
	/*
		��ȡ���ݿ���������Ʒ�����¼
	*/
	std::vector<CPurchaseItemRecord>& GetAllPurchaseItemRecord();

	/*
		���ݶ����ı�Ų�ѯָ������������ʱ��
	*/
	CPurchaseOrderRecord QueryPurchaseOrderRecord(int OrderId);
	/*
		�����ݿ����һ��������¼
	*/
	bool AddPurchaseOrderRecord(CPurchaseOrderRecord& PurchaseRecord);
	/*
		��ȡ���һ�������ı�ţ������µĶ�������ʱ������ţ�ֱ���������������ֵ+1����
		û�ж���ʱ����0
	*/
	int GetLatestOrderId();
	/*
		��ѯ���ݿ������еĶ�����¼
	*/
	std::vector<CPurchaseOrderRecord>& GetAllPurchaseOrderRecord();
private:

};

