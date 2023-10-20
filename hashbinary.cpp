#include "hashbinary.h"
#include "ReaderTicket.h"

int readInsert(const string& filename, int number, HashTable* table)
{
    Ticket* record = new Ticket();
    int r = getRecordFromBin(filename, number, record);
    if (r == 0)
    {
        table->insert(record->key, number);
    }
    return r;
}


int deleteRemove(const string& filename, int key, HashTable* table)
{
    int index = table->remove(key);
    if (index != -1)
    {
        return deleteRecordFromBinByKey(filename, key);
    }
    return -3;
}


Ticket* readGet(int key);