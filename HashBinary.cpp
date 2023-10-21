#include "HashBinary.h"
#include <chrono>

int searchInsert(const string& filename, int key, HashTable<int>* table, Ticket*& toWrite)
{
    if (toWrite == nullptr)
    {
        toWrite = new Ticket();
    }
    int number;
    int r = searchRecordFromBin(filename, key, toWrite, number);

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


int readGet(const string& filename, int key,  HashTable<int>* table, Ticket*& toWrite)
{
    bool success;
    int index = table->get(key, success);
    if (success)
    {
        return getRecordFromBin(filename, index, toWrite);
    }
    return searchInsert(filename, key, table, toWrite);
}

void test() {
#define RECORDSINFILE 2
    string textfilename = "input.txt";
    string filename = "test.bin";

    LINE()
    cout << "Конвертация входного файла txt -> bin" << endl;
    int created = convertTextToBin(textfilename, filename);
    TESTCODE(created)

    HashTable<int>* table = new  HashTable<int>(RECORDSINFILE);

    Ticket* b;
    LINE()
    cout << "Исходная хэш таблица" << endl;
    PRINTTABLE()

    LINE()
    cout << "Чтение файла и формирование таблицы" << endl;

    readGet(filename, 923, table, b);
    readGet(filename, 2123, table, b);
    readGet(filename, 3543, table, b);
    readGet(filename, 4, table, b);
    readGet(filename, 5412, table, b);

    cout << "Как можно заметить, потребовалось рехэширование и для ключа 4 возникла коллизия" << endl;
    PRINTTABLE()
    printFromBin(filename);

    bool success;

    LINE()
    cout << "Удаление записи по ключу 4" << endl;
    deleteRemove(filename, 4, table);

    PRINTTABLE()
    printFromBin(filename);

    LINE()
    cout << "Тестирование на особо крупном файле, 1000000 записей" << endl;

    delete table;
    //размер - 2^20 ~ 1000000
    table = new HashTable<int>(20);
    Ticket *buffer = new Ticket();
    const char *value = "value for 1000000 test";
    strcpy(buffer->fio, value);
    value = "1234567890";
    strcpy(buffer->phoneNumber, value);
    int records = 1000000;

    cout << "Создание файла..." << endl;
    for (int i = 0; i < records; i++)
    {
        buffer->key = i;
        addRecordToBin(filename, buffer);
    }

    cout << "Заполнение таблицы в тестовом режиме (знаем номера записей без чтения файлов)..." << endl;
    for (int i = 0; i < records; i++)
    {
        table->insert(i, i + 1, success);
    }

    cout << "Время чтения записи из файла" << endl;

    cout << "0-я запись" << endl;
    PRINTEXECTIME(
            readGet(filename, 0, table, buffer);
    )
    cout << records / 2 << "-я запись" << endl;
    PRINTEXECTIME(
            readGet(filename, records / 2, table, buffer);
    )
    cout << records - 1 << "-я запись" << endl;
    PRINTEXECTIME(
            readGet(filename, records - 1, table, buffer);
    )
    delete table;
    delete buffer;
    cout << "Тестирование модуля для работы с двоичным файлом и хэш таблицей завершено" << endl;
}