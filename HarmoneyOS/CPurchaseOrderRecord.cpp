#include "CPurchaseOrderRecord.h"

CPurchaseOrderRecord::CPurchaseOrderRecord()
{
	OrderId = 0;
	Timestamp = 0;
}

CPurchaseOrderRecord::CPurchaseOrderRecord(int OrderId, int Timestamp):OrderId(OrderId),Timestamp(Timestamp)
{
}
