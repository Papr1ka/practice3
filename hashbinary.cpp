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
    int number = table->remove(key, success);
    if (success)
    {
        deleteRecordFromBinByNumber(filename, number);
	Ticket* lastTicker;
	int r = getRecordFromBin(filename, number, lastTicket);
	table->update(lastTicket->key, number);
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

void test()
{
#define RECORDSINFILE 5
	int r;
	string textfilename = "input.txt";
	string filename = "test.bin";

	convertTextToBin(textfilename, filename);

	HashTable table = new HashTable(RECORDSINFILE);
	int keys[RECORDSINFILE];

	for (int i = 0; i < RECORDSINFILE; i++)
	{
		r = readInsert(filename, i, table);
		keys[i] = r;
	}

	deleteRemove(filename, keys[2]);
	PRINTEXECTIME(
	r = table->get(keys[3]);
	)
	
	Ticket* buffer = new Ticket();
	string value = "value for 1000000 test"
	strcpy(buffer->fio);
	value = "1234567890";
	strcpy(buffer->phoneNumber);

	for (int i = 0; i < 1000000; i++)
	{
		buffer->key = i;
		addRecordToBin(filename, buffer);
	}
	PRINTEXECTIME(
	r = table->get(3);
	)
	PRINTEXECTIME(
	r = table->get(450000);
	)
	PRINTEXECTIME(
	r = table->get(999000);
	)
}
