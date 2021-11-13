#include "CashierSystem.h"
#include "Database.h"
#include "CPurchaseItemRecord.h"
#include "CItemStorageInfo.h"
#include "CPurchaseOrderRecord.h"
#include <string>
#include <algorithm>
#include "NoImplException.h";
const int COUNT = 0;
const int WEIGHT = 1;
//δʵ��
void CashierSystem::generateOrderId()
{
	throw new NoImplException(__FUNCTION__);
}
//δʵ��
void CashierSystem::generateTimestamp()
{
	throw new NoImplException(__FUNCTION__);
}
/*
	�ú������ܲ�δʵ���꣬�����ڿ���Լ��
*/
std::string CashierSystem::processId(std::string ItemProcessedId, int &Type, double &Weight)
{
	// TODO: �ڴ˴����� return ���
	throw new NoImplException(__FUNCTION__);
	std::string ItemId = ItemProcessedId;
	return ItemId;
}

CashierSystem::CashierSystem()
{
	generateOrderId();
	generateTimestamp();
}
/*
*	�ú�����δ�������ݿ�����׳����쳣��
	ʵ�ֽ���Ʒ��ӵ����ﳵ�Ĺ���
	������ItemProcessedId string ��������ID
		  cnt int �˿͹�����Ʒ������
				  ����������Ƽ۵���Ʒ��cnt�����ʹ���1��������Ϣ�Ѿ������ڵ�һ��������
	����ֵ��bool ��ʾ��Ӳ����Ƿ�ɹ�
*/
bool CashierSystem::AddItemToCart(const std::string& ItemProcessedId, int cnt)
{
	int Type = 0;
	double Weight = 0.0;
	// ���������Idת����ԭ����Id ���õ���Ʒ���ͣ���Ʒ����(����ǰ������Ƽ۵���Ʒ�Ļ�)
	std::string ItemId = processId(ItemProcessedId,Type, Weight);
	// ��ԭId�ҵ�ָ������Ʒ��Ϣ
	CItemStorageInfo ItemStorageInfo = DB.QueryItemStorageInfo(ItemId);
	//����Ʒ����Ʒ������ʱ����Ͷ����Ű�װ��һ�������¼
	CPurchaseItemRecord Record(ItemStorageInfo.Item, Weight, cnt, Timestamp, OrderId);
	// ���ҵ�ǰ���ﳵ���Ƿ��и���Ʒ
	auto ItemRecordIter = find(Cart.begin(), Cart.end(), Record);
	if (ItemRecordIter != Cart.end()) {	// ������ﳵ�����оͽ��кϲ�
		if (Type == COUNT) {	// �������Ƽ۵���Ʒ
			ItemRecordIter->Count += cnt;
		}
		else {	//�������Ƽ۵���Ʒ
			ItemRecordIter->Weight += Weight;
		}
	}
	else {	//������ڹ��ﳵ�оͷ��빺�ﳵ
		Cart.push_back(Record);
	}
	return true;	// �����ɹ�����true
}
/*
*	�ú�����δ�������ݿ�����׳����쳣��
	�ӹ��ﳵ��ɾ����Ʒ
	������ItemProcessedId string Ҫɾ������Ʒ�Ĵ������ID
	����ֵ��bool ɾ�������Ƿ�ɹ�
			true ����ɹ�
			false ����û���ҵ���Ʒδ�ɹ�
*/
bool CashierSystem::RemoveItemFromCart(const std::string& ItemProcessedId)
{
	int Type = 0;
	double Weight = 0.0;
	std::string ItemId = processId(ItemProcessedId, Type, Weight);	//ת������ƷԭID
	CItemStorageInfo ItemStorageInfo = DB.QueryItemStorageInfo(ItemId);	//���ݿ��в�����Ʒ����Ϣ
	auto ItemRecordIter = Cart.begin();	//�������ﳵ
	while (ItemRecordIter != Cart.end()) {
		if (ItemRecordIter->Item == ItemStorageInfo.Item) {	//�ҵ�����Ʒ��ɾ��
			Cart.erase(ItemRecordIter, ItemRecordIter + 1);
			return true;
		}
		ItemRecordIter++;
	}
	return false;	// ���ﳵ��û���ҵ���Ӧ��Ʒ�ͷ���false
}

/*
	����ֵ double ��ǰ���ﳵ�л�����ܼ�ֵ
	������ǰ���ﳵ����Ʒ�����������ֵ�ۼӵ��ܼ�ֵ��ȥ
*/
double CashierSystem::GetCurrentPrice()
{
	double CurrentPrice = 0.0;	//��ʼ��
	auto ItemRecordIter = Cart.begin();	//vector�ĵ�����
	while (ItemRecordIter != Cart.end()) {	//����ÿһ��Ԫ��
		if (ItemRecordIter->Item.ItemType == COUNT) {	// �ж����� �ֱ�����ֵ
			CurrentPrice += ItemRecordIter->Item.Price * ItemRecordIter->Count;
		}
		else {	// ����������
			CurrentPrice += ItemRecordIter->Item.Price * ItemRecordIter->Weight;
		}
		ItemRecordIter++;
	}
	return CurrentPrice;
}
/*
	����ֵ const std::vector<CPurchaseItemRecord>& 
		���ص�ǰ���ﳵ�б�
		const & ����ϣ������ʱ�����ﳵ�Ķ�
		���ǰ�˵���������ʱ��������Ķ�
*/
const std::vector<CPurchaseItemRecord>& CashierSystem::GetCurrentPurchaseList()
{
	// TODO: �ڴ˴����� return ���
	return Cart;
}
/*
* �ú�����δ�������ݿ�����׳����쳣��
	���˹���
	���Ƚ����ﳵ�б��ݸ�����
	Ȼ��һ����չ��ﳵһ�߽���
	����ÿһ�������¼ 
		��Ҫ�������¼��ӵ����ݿ�
		���Ŀ����Ϣ
	���Ҫ�Ѷ�����ӵ����ݿ���
*/
bool CashierSystem::Checkout(std::vector<CPurchaseItemRecord>& CurrentPurchaseList)
{
	CurrentPurchaseList = Cart;	// �����ﳵ�б��ݸ�����
	CPurchaseItemRecord ItemRecord;
	CItemStorageInfo ItemStorageInfo;
	while (!Cart.empty()) {
		ItemRecord = Cart.back();
		DB.AddPurchaseItemRecord(ItemRecord);	//�����Ʒ�����¼
		ItemStorageInfo = DB.QueryItemStorageInfo(ItemRecord.Item.ItemId);	//��ȡ�����Ϣ
		if (ItemRecord.Item.ItemType == COUNT) {	// ������Ʒ�����Ŀ��������Ϣ
			ItemStorageInfo.CountRest -= ItemRecord.Count;
		}
		else {
			ItemStorageInfo.WeightRest -= ItemRecord.Weight;
		}
		DB.ModifyItemStorageInfo(ItemStorageInfo);	//�޸Ŀ��
		Cart.pop_back();	//�����Ʒ
	}
	CPurchaseOrderRecord OrderRecord(OrderId, Timestamp);	//���ɶ���
	DB.AddPurchaseOrderRecord(OrderRecord);	//�����ݿ���Ӷ���
	return true;
}
