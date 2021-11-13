#include "CItemStorageInfo.h"

CItemStorageInfo::CItemStorageInfo()
{
	WeightRest = 0.0;
	CountRest = 0;
	Timestamp = 0;
	IsDelete = false;
}

CItemStorageInfo::CItemStorageInfo(const CItemInfo& Item, double WeightRest, int CountRest, int Timestamp, bool IsDelete):Item(Item), WeightRest(WeightRest), CountRest(CountRest), Timestamp(Timestamp), IsDelete(IsDelete)
{
}
