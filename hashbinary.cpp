#include "hashbinary.h"
#include "ReaderTicket.h"
#include <chrono>

//key must be greater then 0

int readInsert(const string& filename, int number, HashTable<int>* table)
{
    Ticket* record = new Ticket();
    int r = getRecordFromBin(filename, number, record);

    if (r == 0)
    {
        table->insert(record->key, number);
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


int readGet(const string& filename, int key,  HashTable<int>* table, Ticket*& toWrite)
{
    bool success;
    int index = table->get(key, success);
    if (success)
    {
        return getRecordFromBin(filename, index, toWrite);
    }
    return -3;
}

void test() {
#define RECORDSINFILE 5
    int r;
    string textfilename = "input.txt";
    string filename = "test.bin";

    convertTextToBin(textfilename, filename);

    HashTable<int>* table = new  HashTable<int>(RECORDSINFILE);
    int keys[RECORDSINFILE];

    for (int i = 0; i < RECORDSINFILE; i++) {
        r = readInsert(filename, i, table);
        keys[i] = r;
    }
    bool success;

    deleteRemove(filename, keys[2], table);
    PRINTEXECTIME(
            r = table->get, keys[3], success
    )
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
        readInsert(filename, i, table);
    }
    PRINTEXECTIME(
            r = table->get, 3, success
    )
    PRINTEXECTIME(
            r = table->get, 450000, success
    )
    PRINTEXECTIME(
            r = table->get, 999000, success
    )
    delete table;
    delete buffer;
}