#include "hashbinary.h"
#include "ReaderTicket.h"

//key must be greater then 0

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
    bool success;
    int index = table->remove(key, success);
    if (success)
    {
        return deleteRecordFromBinByKey(filename, key);
        deleteRecordFromBinByNumber(filename, index);
        //добавить в таблицу метод update
        //заменить в таблице значение элемента с ключом последнего элемента в файле на номер index
    }
    return -3;
}


int readGet(const string& filename, int key, HashTable* table, Ticket*& toWrite)
{
    bool success;
    int index = table->get(key, success);
    if (success)
    {
        return getRecordFromBin(filename, index, toWrite);
    }
    return -3;
}