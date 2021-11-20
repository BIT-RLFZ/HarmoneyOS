#include "CashierSystem.h"
#include "Database.h"
#include "CPurchaseItemRecord.h"
#include "CItemStorageInfo.h"
#include "CPurchaseOrderRecord.h"
#include "NoImplException.h"
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>

const int COUNT = 0;
const int WEIGHT = 1;
//������Ʒ������ID
void CashierSystem::generateOrderId()
{
	OrderId = DB->GetLatestOrderId() + 1;
}
//����ʱ���
void CashierSystem::generateTimestamp()
{
	Timestamp = (int)time(0);
}
/*
	ʵ��У�鹦�� 
	����ֵ bool: true����ͨ��У�飬��֮��ͨ��
*/
bool CashierSystem::CheckCode(const std::string& value, int code)
{
	int Result = 0;	//��ʼ��
	int Length = value.length();	//�õ��ַ�����
	for (int i = 0; i < Length; ++i) {
		if (value[i] < '0' || value[i] > '9') {
			throw HarmoneyException("��ƷID�д��ڷ������ַ���");
		}
		Result ^= value[i] - '0';	//��λ���
	}
	return Result == code;	//У��
}
/*
	������ƷID�õ���Ӧ����ƷԭʼID,��Ʒ���ͣ���Ʒ����
*/
std::string CashierSystem::processId(const std::string& ItemProcessedId, int &Type, double &Weight)
{
	// TODO: �ڴ˴����� return ���
	// ��λ��ƷID ��λ������ ��λУ��λ
	if (ItemProcessedId.length() != 15) {	//ID���Ȳ���
		throw HarmoneyException("��ƷID���ȴ���!");
	}
	size_t start = 0;	//��ȡ�Ӵ�ʹ��
	int TmpWeight = 0, code = 0;
	std::string ItemId;
	try {
		ItemId = ItemProcessedId.substr(start, 8);	//�ָ����ƷԭʼID
		TmpWeight = stoi(ItemProcessedId.substr(start + 8, 5));	//��ȡ����ʾ������λ
		code = stoi(ItemProcessedId.substr(start + 13, 2));	//��ȡ��У����
	}
	catch (...) {
		throw HarmoneyException("��ƷID�д��ڷ������ַ���");
	}
	if (!CheckCode(ItemProcessedId.substr(start, 13), code)) {	//���ǰ����У���벻ƥ�䣬���쳣
		throw HarmoneyException("��ƷУ�������");
	}
	if (TmpWeight == 0) {	//����λ��Ϊ0����ʾ����Ʒ����ΪCOUNT
		Type = COUNT;
		Weight = 0.00;
	}
	else {	//��������ΪWEIGHT
		Type = WEIGHT;
		Weight = 1.0 * TmpWeight / 100;	//�������Ʒ������
	}
	return ItemId;
}
/*
	���췽��
	����µĶ���ID��ʱ���
*/
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
	CItemStorageInfo ItemStorageInfo = DB->QueryItemStorageInfo(ItemId);
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
	CItemStorageInfo ItemStorageInfo = DB->QueryItemStorageInfo(ItemId);	//���ݿ��в�����Ʒ����Ϣ
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
		DB->AddPurchaseItemRecord(ItemRecord);	//�����Ʒ�����¼
		ItemStorageInfo = DB->QueryItemStorageInfo(ItemRecord.Item.ItemId);	//��ȡ�����Ϣ
		if (ItemRecord.Item.ItemType == COUNT) {	// ������Ʒ�����Ŀ��������Ϣ
			ItemStorageInfo.CountRest -= ItemRecord.Count;
		}
		else {
			ItemStorageInfo.WeightRest -= ItemRecord.Weight;
		}
		DB->ModifyItemStorageInfo(ItemStorageInfo);	//�޸Ŀ��
		Cart.pop_back();	//�����Ʒ
	}
	CPurchaseOrderRecord OrderRecord(OrderId, Timestamp);	//���ɶ���
	DB->AddPurchaseOrderRecord(OrderRecord);	//�����ݿ���Ӷ���
	return true;
}
