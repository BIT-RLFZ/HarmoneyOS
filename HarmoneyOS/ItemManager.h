#pragma once
#include "CItemStorageInfo.h"
#include "CPurchaseItemRecord.h"
#include "CPurchaseOrderRecord.h"
#include "Database.h"
#include <string>
#include <vector>
extern Database* DB;
struct SingleOrder {//ÿһ�ݶ������м�ֵ����Ϣ
    double OrderProfit;//����������
    double OrderCost;//�����ܼ۸�
};
struct OrderYouWannaKnow {//ÿ���µĲ��񱨱���¼���¶������м�ֵ����Ϣ
    SingleOrder MaxProfit;//����
    SingleOrder MinProfit;
    SingleOrder MaxCost;//�����Ķ���
    SingleOrder MinCost;
    int OrderAmount;//������Ŀ
    OrderYouWannaKnow(const SingleOrder& a, const SingleOrder& b,
        const SingleOrder& c, const SingleOrder& d, const int& e) {
        MaxProfit = a;
        MinProfit = b;
        MaxCost = c;
        MinCost = d;
        OrderAmount = e;
    }
};

class ItemManager
{
private:
    int TotalMonthCount; //������Ϊֹ��������
    std::vector <OrderYouWannaKnow> FinancialList; //����ÿ���µĶ����е��м�ֵ����Ϣ
    std::vector <SingleOrder> Orders; //�洢ȫ������ZZ
    std::vector <CItemStorageInfo> Goods; //�洢ȫ������YY
    int TimestampConvertToMonthNumber(const int& Timestamp);//ʱ���ת����ѧpair<int, int> a;

public:
    CItemStorageInfo ReceiveAction();

    bool AddItem(const CItemStorageInfo& NewItem);//������Ʒ��
    bool ModifyItem(const CItemStorageInfo& NewItem);//�޸ĵ�����
    bool DeleteItem(std::string ItemId);//������Ʒ���ɾ��
    std::vector<CItemStorageInfo>& GetAllItemStorageInfo();//YY��ȡ�ֿ������е���Ʒ��Ϣ, �Խ�rbh
    //�����Ƕ���Ʒ����ɾ�Ĳ�

    //�����ǶԶ�����Ϣ�Ĵ���
    /*
      ˼·�������ݿ���ȫ������Ʒ������Ϣ(1)
      -> ͨ�������ţ���������Ʒ��������Ϣ�ϲ�������������Ϣ��(2)
      -> ���ݶ�����Ϣ����ʱ������飬���ÿ���µĲ�����Ϣ���������м�ֵ��(3)
      -> ���м�ֵ����Ϣ�ۺ�һ��(4)
    */
    std::vector<CPurchaseOrderRecord>& GetAllPurchaseOrderRecord();//(1)ZZ

    bool GetProfit(const std::vector<CItemStorageInfo>& ItemList);//(2)

    OrderYouWannaKnow GetOrders(const std::vector<SingleOrder>& OrderLists); //(3)
    bool CreateNewMonthlyRecord(const int& Timestamp, const double& Profit); //(3)
    //���һ���¶ȵ��������¼

    bool CreateMonthlyFinancialStatement(const std::vector<OrderYouWannaKnow>& List); //(4)
    //����һ���¶Ȳ��񱨱�ͳ�ƴ� 0 ��ʼ��������Ϊֹ������Ϊ��λͳ��������Ϣ
    //�ռ�ÿ���µ�������Ϣ ���Ƴ�ͼ��
    //�ռ�ÿ���µ�������Ϣ ���Ƴ�ͼ��

   // bool AddItem(std::string ItemId, std::string ItemName, double WeightRest, int CountRest,);
};