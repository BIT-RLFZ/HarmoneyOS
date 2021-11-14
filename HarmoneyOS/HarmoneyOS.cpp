// HarmoneyOS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include "CItemStorageInfo.h"
#include "CItemInfo.h"
#include "GlobalVar.h"

using namespace std;

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
    DB->AddItemStorageInfo(info);
}

int main()
{
    std::cout << "Hello World1!\n";
    DB->InitDatabase("ForTest.osdb");
    auto storage = DB->GetAllItemStorageInfo();
    if (storage.size() > 0) {
        for (auto aaa : storage) {
            printf("[Item] Name = %s count = %d ID = %s\n", aaa.Item.ItemName.c_str(), aaa.CountRest, aaa.Item.ItemId.c_str());
        }
        return 0;
    }
    AddItem("name1",10,"ID1");
    AddItem("name2", 20, "ID2");
    AddItem("name3", 30, "ID3");
    AddItem("name4", 40, "ID4");
    AddItem("name5", 50, "ID5");
    AddItem("name6", 60, "ID6");
    AddItem("name7", 70, "ID7");
    AddItem("name8", 80, "ID8");
    AddItem("臭豆腐", 80, "ID99");
    DB->UpdateDatabaseFile();
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
