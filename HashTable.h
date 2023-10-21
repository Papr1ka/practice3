//
// Created by joe on 20.10.23.
//

#ifndef SIAOD3_HASHTABLE_H
#define SIAOD3_HASHTABLE_H

#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

#define DEBUG

#ifdef DEBUG
#include <vector>
#endif

#define A 0.6180339887498948482045868343656381177203091798057628621354486227

template<typename T>
class HashTable;

template<typename T>
class TableItem {
protected:
    int key;
    T value;
    char flags;
public:

#ifdef DEBUG
    vector<int> debugPrimaryIndexes;
#endif

    enum Flags {
        opened = 1,
        deleted = 2,
    };

    TableItem();

    TableItem(int key, T value);

    bool isOpened();

    bool isDeleted();

    void remove();

    int getKey(bool &success);

    T getValue(bool &success);

    void setValue(T value);

    void clearDeleted();

#ifdef DEBUG
    void print();
    friend void testHeshT();
    friend void test();
#endif
};

template<typename T>
class HashTable
{
private:
    int m;
    int n;
    int deleted;
    TableItem<T>* items;

    int getKeyHash(int key);

    int getBiasHash(int key);

    //возвращает индекс элемента в массиве таблицы
    int findItem(int key, bool& success);

    //возвращает индекс элемента в массиве таблицы
    int popItem(int key, bool& success);

public:
    HashTable();

    //size - степень числа 2
    HashTable(unsigned int size);

    ~HashTable();

    //возвращает текущий размер таблицы
    int getSize();

    //возвращает количество занятых и неудалённых ячеек
    int getFilled();

    //возвращает количество удалённых ячеек
    int getDeleted();

    //возвращает отношение количества занятых к размеру таблицы в процентах (0-100)
    int getFilledPercent();

    bool needRehash();

    void reHash();

    void insert(int key, T value, bool& success);

    T get(int key, bool& success);
    T get(int key);

    void update(int key, T value, bool& success);
    void update(int key, T value);

    T pop(int key, bool& success);
    T pop(int key);

#ifdef DEBUG
    friend void testHeshT();
    friend void test();
#endif
};

#ifdef DEBUG
void testHeshT();
#endif

#endif //SIAOD3_HASHTABLE_H
