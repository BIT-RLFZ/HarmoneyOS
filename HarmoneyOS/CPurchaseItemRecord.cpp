#include "CPurchaseItemRecord.h"

bool operator==(const CPurchaseItemRecord& RecordA, const CPurchaseItemRecord& RecordB)
{
    return RecordA.Item == RecordB.Item;
}

CPurchaseItemRecord::CPurchaseItemRecord()
{
    Weight = 0.0;   
    Count = 0;
    Timestamp = 0;
    OrderId = 0;
}

CPurchaseItemRecord::CPurchaseItemRecord(const CItemInfo& Item, double Weight, int Count, int TimeStamp, int OrderId):Item(Item), Weight(Weight), Count(Count), Timestamp(TimeStamp), OrderId(OrderId)
{
}
