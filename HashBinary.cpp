#include "HashBinary.h"
#include <chrono>

int searchInsert(const string& filename, int key, HashTable<int>* table, Ticket*& toWrite)
{
    if (toWrite == nullptr)
    {
        toWrite = new Ticket();
    }
    int number;
    int r = searchRecordFromBin(filename, number, toWrite, number);

    bool success;
    if (r == 0)
    {
        //если success будет false, значит мы пытались вставить дубликат и ключ не уникален
        table->insert(key, number, success);
        if (!success)
        {
            return -5;
        }
    }
    return r;
}


int deleteRemove(const string& filename, int key,  HashTable<int>* table)
{
    bool success;

    //номер записи в файле
    int number = table->pop(key, success);
    if (success)
    {
        deleteRecordFromBinByNumber(filename, number);
        Ticket* lastTicket;

        //изменение порядкового номера записи в файле для последней записи (так как удаление путём замены на последнюю запись)
        int r = getRecordFromBin(filename, number, lastTicket);
        if (r != 0)
        {
            return -2;
        }
        table->update(lastTicket->key, number, success);
        if (success)
        {
            return 0;
        }
        return -1;
    }
    return -3;
}


int readGet(const string& filename, int key,  HashTable<int>* table, Ticket*& toWrite, bool& success)
{
    int index = table->get(key, success);
    if (success)
    {
        return getRecordFromBin(filename, index, toWrite);
    }
    return searchInsert(filename, key, table, toWrite);
}

void test() {
#define RECORDSINFILE 5
    string textfilename = "input.txt";
    string filename = "test.bin";

    convertTextToBin(textfilename, filename);

    HashTable<int>* table = new  HashTable<int>(RECORDSINFILE);
    int keys[RECORDSINFILE];

    Ticket* b;

    for (int i = 1; i < RECORDSINFILE + 1; i++)
    {
        searchInsert(filename, i, table, b);
        keys[i - 1] = b->key;
    }
    bool success;

    deleteRemove(filename, keys[2], table);
    Ticket *buffer = new Ticket();
    const char *value = "value for 1000000 test";
    strcpy(buffer->fio, value);
    value = "1234567890";
    strcpy(buffer->phoneNumber, value);

    for (int i = 0; i < 1000000; i++) {
        buffer->key = i;
        addRecordToBin(filename, buffer);
    }
    for (int i = 0; i < 1000000; i++)
    {
        searchInsert(filename, i, table, b);
    }
    PRINTEXECTIME(
            readGet(filename, 3, table, buffer, success);
    )
    PRINTEXECTIME(
            readGet(filename, 3, table, buffer, success);
    )
    PRINTEXECTIME(
            readGet(filename, 3, table, buffer, success);
    )
    delete table;
    delete buffer;
}