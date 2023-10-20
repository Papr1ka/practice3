//
// Created by joe on 19.10.23.
//
#include "hashtable.h"
#include "debugtools.h"

void testHeshT()
{

    HashTable* table = new HashTable(4);

    LINE()
    cout << "Проверка формирования хешей для ключей..." << endl;
    cout << "key | index | bias" << endl;
    int key = 111001;
    cout << key << " | " << table->table->getKeyHash(key) << " | " << table->table->getBiasHash(key) << endl;
    key = 112001;
    cout << key << " | " << table->table->getKeyHash(key) << " | " << table->table->getBiasHash(key) << endl;
    key = 212001;
    cout << key << " | " << table->table->getKeyHash(key) << " | " << table->table->getBiasHash(key) << endl;
    key = 303002;
    cout << key << " | " << table->table->getKeyHash(key) << " | " << table->table->getBiasHash(key) << endl;
    key = 304002;
    cout << key << " | " << table->table->getKeyHash(key) << " | " << table->table->getBiasHash(key) << endl;
    key = 111007;
    cout << key << " | " << table->table->getKeyHash(key) << " | " << table->table->getBiasHash(key) << endl;
    key = 305002;
    cout << key << " | " << table->table->getKeyHash(key) << " | " << table->table->getBiasHash(key) << endl;
    key = 111001;
    cout << key << " | " << table->table->getKeyHash(key) << " | " << table->table->getBiasHash(key) << endl;
    key = 303010;
    cout << key << " | " << table->table->getKeyHash(key) << " | " << table->table->getBiasHash(key) << endl;

    LINE()
    cout << "Тестирование вставки" << endl;
    int test;
    test = 111001;
    cout << "Вставка без коллизии " << test << endl;
    table->insert(test, 0);
    PRINTTABLE()

    LINE()
    test = 112001;
    cout << "Вставка с коллизией " << test << endl;
    table->insert(test, 0);
    PRINTTABLE()

    LINE()
    test = 212001;
    cout << "Вставка с рехэшированием " << test << " и " << " 303002" << endl;
    table->insert(test, 0);
    test = 303002;
    table->insert(test, 0);
    PRINTTABLE()

    bool success;

    LINE()
    test = 111001;
    cout << "Удаление ключа " << test << " из таблицы" << endl;
    table->remove(test, success);
    if (success)
    {
        cout << "Ключ удалён" << endl;
    }
    else
    {
        cout << "Ошибка, ключ не найден" << endl;
    }
    PRINTTABLE()

    LINE()
    test = 112001;
    cout << "Поиск ключа " << test << " в таблице" << endl;
    int value = table->get(test, success);
    if (success)
    {
        cout << "Найден элемент " << test << endl;
    }
    else
    {
        cout << "Ошибка, элемент не найден" << endl;
    }
    PRINTTABLE()
    delete table;

    LINE()
    cout << "Поиск элемента, размещённого после удалённого с одним значением хэша ключей элементов" << endl;
    table = new HashTable(4);
    table->insert(111001, 0);
    table->insert(112001, 0);
    table->insert(303010, 0);
    table->remove(112001, success);
    PRINTTABLE()
    value = table->get(303010, success);

    if (success)
    {
        cout << "Найден элемент " << 303010 << endl;
    }
    else
    {
        cout << "Ошибка, элемент не найден" << endl;
    }
}

int HTable::getKeyHash(int key)
{
    double b = key * A;
    double c = fmod(b, 1);
    c *= this->m;
    int index = floor(c);
    return index;
}

int HTable::getBiasHash(int key)
{
    int result = (key / this->m) % this->m;
    if (result == 0)
    {
        result = 1;
    }
    return result;
}

HTable::HTable()
{
    this->m = 16;
    this->n = 0;
    this->deleted = 0;
    this->items = new TableItem[16];
    for (int i = 0; i < 16; ++i)
    {
        this->items[i] = TableItem();
    }
}

HTable::HTable(int size)
{
    this->m = size;
    this->n = 0;
    this->deleted = 0;
    this->items = new TableItem[size];
    for (int i = 0; i < size; ++i)
    {
        this->items[i] = TableItem();
    }
}

HTable::~HTable()
{
    delete[] this->items;
}

int HTable::find(int key)
{
    int index = this->getKeyHash(key);
    int bias = this->getBiasHash(key);
    TableItem tmp = this->items[index];
    while ((!tmp.isOpened() || tmp.isDeleted()) && tmp.getKey() != key)
    {
        index = (index + bias) % this->m;
        tmp = this->items[index];
    }
    if (tmp.isOpened())
    {
        return -1;
    }
    return index;
}

int HTable::get(int index)
{
    return this->items[index].value;
}

void HTable::insert(int key, int value)
{
    TableItem item(key, value);
    int index = this->getKeyHash(item.getKey());
#ifdef DEBUG
    item.debugPrimaryIndexes.push_back(index);
#endif
    int bias = this->getBiasHash(item.getKey());
    TableItem buffer = this->items[index];
    while (!(buffer.isOpened() && !buffer.isDeleted()))
    {
        index = (index + bias) % this->m;
#ifdef DEBUG
        item.debugPrimaryIndexes.push_back(index);
#endif
        buffer = this->items[index];
    }
    this->items[index] = item;
    this->n++;
}

bool HTable::needRehash()
{
    if (((this->n + this->deleted) * 100 / this->m) > 75)
    {
        return true;
    }
    return false;
}

int HTable::pop(int key)
{
    int index = this->find(key);
    if (index == -1)
    {
        return -1;
    }
    this->items[index].remove();
    this->deleted++;
    return index;
}

HTable* HTable::reHash(HTable* oldTable)
{
    HTable* newTable = new HTable(oldTable->m * 2);

    for (int i = 0; i < oldTable->m; i++)
    {
        TableItem buffer = oldTable->items[i];
        if (!buffer.isOpened())
        {
            newTable->insert(buffer.key, buffer.value);
        }
    }
    delete oldTable;
    return newTable;
}



void HashTable::reHash()
{
    this->table = HTable::reHash(this->table);
}

HashTable::HashTable()
{
    this->table = new HTable();
}

HashTable::HashTable(int size)
{
    this->table = new HTable(size);
}

HashTable::~HashTable()
{
    delete this->table;
}

void HashTable::insert(int key, int value)
{
    this->table->insert(key, value);
    if (this->table->needRehash())
    {
        this->reHash();
    }
}

int HashTable::get(int key, bool& success)
{
    int index = this->table->find(key);
    if (index == -1)
    {
        success = false;
        return 0;
    }
    success = true;
    return this->table->get(index);
}

int HashTable::remove(int key, bool& success)
{
    int index = this->table->pop(key);
    if (index == -1)
    {
        success = false;
        return 0;
    }
    success = true;
    return this->table->pop(key);
}
