//
// Created by joe on 19.10.23.
//

#ifndef SIAOD3_HASHBINARY_H
#define SIAOD3_HASHBINARY_H

#include "hashtable.h"
#include "binaryutils.h"
#include "HashTable.h"

int readInsert(const string& filename, int number, HashTable<int>* table);

int deleteRemove(const string& filename, int key, HashTable<int>* table);

int readGet(const string& filename, int key, HashTable<int>* table, Ticket*& toWrite);

void test();

#endif //SIAOD3_HASHBINARY_H
