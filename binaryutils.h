//
// Created by joe on 19.10.23.
//

#ifndef SIAOD3_BINARYUTILS_H
#define SIAOD3_BINARYUTILS_H

#pragma once

#include <string>
#include <fstream>
#include "debugtools.h"
#include "ReaderTicket.h"

#define CHECK(filestream, error_code) \
{                      \
    if (!filestream.good()) \
    {                     \
        return error_code;                      \
    }\
};


#define ENDSTREAM(fstream) \
{                       \
    fstream.close(); \
    return fstream.bad(); \
};

using namespace std;

//конвертация из текстового формата в двоичный
int convertTextToBin(const string& filename1, const string& filename2);
//получение записи по порядковому номеру
int getRecordFromBin(const string& filename, int number, Ticket*& toWrite);

//обязательная функция по требованиям работы 3, пункт 4 "Удалить запись с заданным ключом из файла"
//в рамках работы с двоичным файлом, мы не знаем порядковый номер исходя из ключа,
// и по заданию не удалось распознать, что подразумевается работа с хеш таблицей в рамках этого заголовочного файла
// следовательно функция реализовывать линейный поиск
int deleteRecordFromBinByKey(const string& filename, int key);

//удаление записи по порядковому номеру
int deleteRecordFromBinByNumber(const string& filename, int number);
//добавление записи в конец файла
int addRecordToBin(const string& filename, const Ticket* record);
//вывод файла
int printFromBin(const string& filename1);

//тестирование модуля
void testBinF(const string& filename);

#endif //SIAOD3_BINARYUTILS_H
