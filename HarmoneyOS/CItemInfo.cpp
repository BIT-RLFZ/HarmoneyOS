#include "CItemInfo.h"
#include "NoImplException.h"
bool operator==(const CItemInfo& item1, const CItemInfo& item2)
{
    throw NoImplException(__FUNCTION__);
    return false;
}

CItemInfo::CItemInfo()
{
    Cost = 0.0;
    Price = 0.0;
    ItemType = 0;
}

CItemInfo::CItemInfo(const std::string& ItemName, double Cost, double Price, const std::string& ItemId, int ItemType):ItemName(ItemName), Cost(Cost), Price(Price), ItemId(ItemId), ItemType(ItemType)
{
}
