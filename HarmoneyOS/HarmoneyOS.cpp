// HarmoneyOS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <vector>
#include "CItemStorageInfo.h"
#include "CItemInfo.h"
#include "Database.h"
extern Database* DB;

using namespace std;

vector<CItemStorageInfo> Itemvec;

void AddItem(string itemName, int count, string itemID) {
    CItemStorageInfo info;
    info.Item.ItemType = 0;
    info.Item.Cost = 123;
    info.Item.ItemId = itemID;
    info.Item.ItemName = itemName;
    info.CountRest = count;
    info.IsDelete = false;
    info.Timestamp = time(0);
    info.WeightRest = 0;
    info.Item.Price = 888;
    Itemvec.push_back(info);
    DB->AddItemStorageInfo(info);
}

int main()
{
    srand(time(0));
    std::cout << "Hello World1!\n";
    /*
    DB->InitDatabase("ForTest1.osdb");
    for (int i = 0; i < 100000; i++) {
        static char tmp[100] = { 0 };
        sprintf_s(tmp,"aaa_%d",i);
        AddItem(tmp,i, tmp);
        if(i % 10000 == 0) printf("%d\n",i);
    }*/
    remove("ForTest2.osdb");
    DB->InitDatabase("ForTest2.osdb");
    for (int i = 0; i < 20; i++) {
        static char tmp[100] = { 0 };
        sprintf_s(tmp, "aaa_%d", i);
        AddItem(tmp, i, tmp);
        if (i % 10000 == 0) printf("%d\n", i);
    }
    for (int i = 0; i < 20; i++) {
        CPurchaseItemRecord pir;
        pir.Item = Itemvec[rand() % Itemvec.size()].Item;
        pir.Count = 12;
        pir.OrderId = i;
        pir.Timestamp = time(0);
        pir.Weight = 0;
        DB->AddPurchaseItemRecord(pir);
    }
    DB->UpdateDatabaseFile();
    DB->InitDatabase("ForTest2.osdb");
    auto all = DB->GetAllPurchaseItemRecord();
    for (auto each : all) {
        printf("[!] orderid: %d timestamp: %d name: %s \n",each.OrderId,each.Timestamp,each.Item.ItemName.c_str());
    }
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
