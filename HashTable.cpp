//
// Created by joe on 20.10.23.
//

#include "HashTable.h"
#include "debugtools.h"

template<typename T>
TableItem<T>::TableItem()
{
    this->flags = Flags::opened;
}

template<typename T>
TableItem<T>::TableItem(int key, T value)
{
    this->key = key;
    this->value = value;
    this->flags = 0;
}

template<typename T>
bool TableItem<T>::isOpened()
{
    return this->flags & Flags::opened;
}

template<typename T>
bool TableItem<T>::isDeleted()
{
    return this->flags & Flags::deleted;
}

template<typename T>
void TableItem<T>::remove()
{
    this->flags = Flags::opened | Flags::deleted;
}

template<typename T>
int TableItem<T>::getKey(bool& success)
{
    if (!isOpened())
    {
        success = true;
        return this->key;
    }
    success = false;
    return 0;
}

template<typename T>
T TableItem<T>::getValue(bool& success)
{
    if (!isOpened())
    {
        success = true;
        return this->value;
    }
    success = false;
    return 0;
}

template<typename T>
void TableItem<T>::setValue(T newValue)
{
    this->value = newValue;
    this->flags = 0;
}

template<typename T>
void TableItem<T>::clearDeleted()
{
    this->flags = 0;
}

#ifdef DEBUG
template<typename T>
void TableItem<T>::print()
{
    bool success;
    int keyValue = this->getKey(success);
    cout << std::setw(12);
    if (this->isDeleted())
    {
        cout << this->key;
    }
    else if (!success)
    {
        cout << "~";
    }
    else
    {
        cout << keyValue;
    }
    cout << setw(3) << this->isOpened() << setw(3) << this->isDeleted();
    for (int i = 0; i < this->debugPrimaryIndexes.size(); i++)
    {
        cout << setw(3) << this->debugPrimaryIndexes[i];
    }
    cout << endl;
}
#endif

template<typename T>
HashTable<T>::HashTable()
{
    this->m = 16;
    this->n = 0;
    this->deleted = 0;
    this->items = new TableItem<T>[16];
    for (int i = 0; i < 16; ++i)
    {
        this->items[i] = TableItem<T>();
    }
}

template<typename T>
HashTable<T>::HashTable(unsigned int size)
{
    this->m = 1 << size;
    if (this->m <= 0)
    {
        this->m = 16;
    }
    this->n = 0;
    this->deleted = 0;
    this->items = new TableItem<T>[this->m];
    for (int i = 0; i < this->m; ++i)
    {
        this->items[i] = TableItem<T>();
    }
}

template<typename T>
HashTable<T>::~HashTable()
{
    delete[] this->items;
}

template<typename T>
int HashTable<T>::getKeyHash(int key)
{
    double b = key * A;
    double c = fmod(b, 1);
    c *= this->m;
    int index = floor(c);
    return index;
}

template<typename T>
int HashTable<T>::getBiasHash(int key)
{
    int result = (key / this->m) % this->m;
    if (result == 0)
    {
        result = 1;
    }
    return result;
}

template<typename T>
int HashTable<T>::findItem(int key, bool& success)
{
    int index = this->getKeyHash(key);
    int bias = this->getBiasHash(key);
    TableItem<T> tmp = this->items[index];
    bool unused;
    while ((!tmp.isOpened() || tmp.isDeleted()) && tmp.getKey(unused) != key)
    {
        index = (index + bias) % this->m;
        tmp = this->items[index];
    }
    if (tmp.isOpened())
    {
        success = false;
        return 0;
    }
    success = true;
    return index;
}

template<typename T>
int HashTable<T>::popItem(int key, bool& success)
{
    int index = this->findItem(key, success);
    if (!success)
    {
        return 0;
    }
    this->items[index].remove();
    this->deleted++;
    return index;
}

template<typename T>
bool HashTable<T>::needRehash()
{
    if (((this->n + this->deleted) * 100 / this->m) > 75)
    {
        return true;
    }
    return false;
}

template<typename T>
void HashTable<T>::reHash()
{
    int oldSize = this->m;
    this->m *= 2;
    this->deleted = 0;
    this->n = 0;
    TableItem<T>* oldItems = this->items;
    this->items = new TableItem<T>[this->m];
    bool unused;

    for (int i = 0; i < oldSize; i++)
    {
        TableItem<T> buffer = oldItems[i];
        if (!buffer.isOpened())
        {
            buffer.clearDeleted();
            this->insert(buffer.getKey(unused), buffer.getValue(unused));
        }
    }

    delete[] oldItems;
}

template<typename T>
void HashTable<T>::insert(int key, T value)
{
    bool unused;
    TableItem<T> item(key, value);
    int index = this->getKeyHash(item.getKey(unused));
#ifdef DEBUG
    item.debugPrimaryIndexes.push_back(index);
#endif
    int bias = this->getBiasHash(item.getKey(unused));
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

    if (this->needRehash())
    {
        this->reHash();
    }
}

template<typename T>
T HashTable<T>::get(int key, bool& success)
{
    int index = this->findItem(key, success);
    if (!success)
    {
        return 0;
    }
    return this->items[index].getValue(success);
}

template<typename T>
T HashTable<T>::get(int key)
{
    bool success;
    int index = this->findItem(key, success);
    if (!success)
    {
        return 0;
    }
    return this->items[index];
}

template<typename T>
void HashTable<T>::update(int key, T value, bool& success)
{
    int index = this->findItem(key, success);
    if (success)
    {
        this->items[index].setValue(value);
    }
}

template<typename T>
void HashTable<T>::update(int key, T value)
{
    bool success;
    int index = this->findItem(key, success);
    if (success)
    {
        this->items[index].setValue(value);
    }
}

template<typename T>
T HashTable<T>::pop(int key, bool& success)
{
    int index = this->popItem(key, success);
    if (success)
    {
        return this->items[index].getValue(success);
    }
    return 0;
}

template<typename T>
T HashTable<T>::pop(int key)
{
    bool success;
    int index = this->popItem(key, success);
    if (success)
    {
        return this->items[index];
    }
    return 0;
}

#ifdef DEBUG
void testHeshT()
{

    HashTable<int>* table = new HashTable<int>(2);

    LINE()
    cout << "Проверка формирования хешей для ключей..." << endl;
    cout << "key | index | bias" << endl;
    int key = 111001;
    cout << key << " | " << table->getKeyHash(key) << " | " << table->getBiasHash(key) << endl;
    key = 112001;
    cout << key << " | " << table->getKeyHash(key) << " | " << table->getBiasHash(key) << endl;
    key = 212001;
    cout << key << " | " << table->getKeyHash(key) << " | " << table->getBiasHash(key) << endl;
    key = 303002;
    cout << key << " | " << table->getKeyHash(key) << " | " << table->getBiasHash(key) << endl;
    key = 304002;
    cout << key << " | " << table->getKeyHash(key) << " | " << table->getBiasHash(key) << endl;
    key = 111007;
    cout << key << " | " << table->getKeyHash(key) << " | " << table->getBiasHash(key) << endl;
    key = 305002;
    cout << key << " | " << table->getKeyHash(key) << " | " << table->getBiasHash(key) << endl;
    key = 111001;
    cout << key << " | " << table->getKeyHash(key) << " | " << table->getBiasHash(key) << endl;
    key = 303010;
    cout << key << " | " << table->getKeyHash(key) << " | " << table->getBiasHash(key) << endl;

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
    table->pop(test, success);
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
    table = new HashTable<int>(2);
    table->insert(111001, 0);
    table->insert(112001, 0);
    table->insert(303010, 0);
    table->pop(112001, success);
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

    LINE()
    cout << "Проверка замены значения элемента, метод udpate" << endl;
    key = 111001;
    cout << "Ключ " << key << " значение " << table->get(111001, success) << endl;
    int newValue = 58;
    cout << "Изменение значения на " << newValue << endl;
    table->update(key, newValue, success);

    if (success)
    {
        cout << "Значение изменено" << endl;
    }
    else
    {
        cout << "Ошибка, не вышло" << endl;
    }
    cout << "Ключ " << key << " значение " << table->get(111001, success) << endl;
    cout << "Тестирование завершено" << endl;
}
#endif
