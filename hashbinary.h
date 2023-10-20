//
// Created by joe on 19.10.23.
//

#ifndef SIAOD3_HASHBINARY_H
#define SIAOD3_HASHBINARY_H

#include "hashtable.h"
#include "binaryutils.h"

int readInsert(const string& filename, int number, HashTable* table);

int deleteRemove(const string& filename, int key, HashTable* table);

int readGet(const string& filename, int key, HashTable* table, Ticket*& toWrite);

void test();

#endif //SIAOD3_HASHBINARY_H
