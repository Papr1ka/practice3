#include "hashbinary.h"
#include <chrono>

//key must be greater then 0

int readInsert(const string& filename, int number, HashTable<int>* table, Ticket*& toWrite)
{
    if (toWrite == nullptr)
    {
        toWrite = new Ticket();
    }
    int r = getRecordFromBin(filename, number, toWrite);

    if (r == 0)
    {
        table->insert(toWrite->key, number);
    }
    return r;
}


int deleteRemove(const string& filename, int key,  HashTable<int>* table)
{
    bool success;
    int number = table->pop(key, success);
    if (success)
    {
        deleteRecordFromBinByNumber(filename, number);
        Ticket* lastTicket;
        int r = getRecordFromBin(filename, number, lastTicket);
        table->update(lastTicket->key, number, success);
        if (success)
        {
            return 0;
        }
        return -1;
        //добавить в таблицу метод update
        //заменить в таблице значение элемента с ключом последнего элемента в файле на номер index
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
    return 0;
}

void test() {
#define RECORDSINFILE 5
    int r;
    string textfilename = "input.txt";
    string filename = "test.bin";

    convertTextToBin(textfilename, filename);

    HashTable<int>* table = new  HashTable<int>(RECORDSINFILE);
    int keys[RECORDSINFILE];

    Ticket* b;

    for (int i = 1; i < RECORDSINFILE + 1; i++)
    {
        r = readInsert(filename, i, table, b);
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
        readInsert(filename, i, table, b);
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