//
// Created by joe on 19.10.23.
//

#ifndef SIAOD3_HASHTABLE_H

#include "math.h"
#include <iomanip>
#include <iostream>

using namespace std;

#define DEBUG

#ifdef DEBUG
#include <vector>
#endif

#define DEBUG

#define A 0.6180339887498948482045868343656381177203091798057628621354486227

class HTable
{
private:
    struct TableItem
    {
        int key;
        int value;
        char flags;
#ifdef DEBUG
        vector<int> debugPrimaryIndexes;
#endif

        enum Flags
        {
            opened = 1,
            deleted = 2,
        };

        TableItem()
        {
            this->flags = Flags::opened;
        }

        TableItem(int key, int value)
        {
            this->key = key;
            this->value = value;
            this->flags = 0;
        }

        bool isOpened() const
        {
            return this->flags & Flags::opened;
        }

        bool isDeleted() const
        {
            return this->flags & Flags::deleted;
        }

        void remove()
        {
            this->flags = Flags::opened | Flags::deleted;
        }

        int getKey() const
        {
            if (!isOpened())
            {
                return this->key;
            }
            return -1;
        }

        void print()
        {
            int keyValue = this->getKey();
            cout << std::setw(12);
            if (this->isDeleted())
            {
                cout << this->key;
            }
            else if (keyValue == -1)
            {
                cout << "~";
            }
            else
            {
                cout << keyValue;
            }
            cout << setw(3) << this->isOpened() << setw(3) << this->isDeleted();
#ifdef DEBUG
            for (int i = 0; i < this->debugPrimaryIndexes.size(); i++)
            {
                cout << setw(3) << this->debugPrimaryIndexes[i];
            }
            cout << endl;
#endif
        }
    };

    int m;
    int n;
    int deleted;
    TableItem* items;

public:
    int getKeyHash(int key);

    int getBiasHash(int key);

    HTable();

    HTable(int size);

    ~HTable();

    int find(int key);

    int get(int index);

    void insert(int key, int value);

    bool needRehash();

    int pop(int key);

    static HTable* reHash(HTable* oldTable);

    friend void testHeshT();
};

class HashTable
{
private:
    HTable* table;

    void reHash();
public:
    HashTable();

    HashTable(int size);

    ~HashTable();

    void insert(int key, int value);

    int get(int key, bool& success);

    int remove(int key, bool& success);

    friend void testHeshT();
};

void testHeshT();

#define SIAOD3_HASHTABLE_H
#endif //SIAOD3_HASHTABLE_H
