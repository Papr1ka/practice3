#include "binaryutils.h"
#include <filesystem>

int addRecordToBin(const string& filename, const Ticket* record)
{
    ofstream wNewStream;
    wNewStream.open(filename, (ios::ios_base::binary | ios::ios_base::app));
    CHECK(wNewStream, -1)

    wNewStream.write((char*)record, sizeof(Ticket));

    ENDSTREAM(wNewStream)
}


int convertTextToBin(const string& filename1, const string& filename2)
{
    ifstream rOldStream;
    ofstream wNewStream;
    rOldStream.open(filename1);
    wNewStream.open(filename2, ios::ios_base::binary);
    CHECK(rOldStream, -1)
    CHECK(wNewStream, -2)

    while (rOldStream.peek() != EOF)
    {
        Ticket buffer;
        rOldStream >> buffer.key;
        rOldStream >> buffer.fio;
        rOldStream >> buffer.phoneNumber;
        wNewStream.write((char*)&buffer, sizeof(Ticket));
        rOldStream.get();
    }

    rOldStream.close();
    wNewStream.close();
    return rOldStream.bad() || wNewStream.bad();
}

int printFromBin(const string& filename)
{
    ifstream rOldStream;
    rOldStream.open(filename, ios::ios_base::binary);
    CHECK(rOldStream, -1)

    Ticket buffer;

    while (rOldStream.peek() != EOF)
    {
        rOldStream.read((char*)&buffer, sizeof(buffer));
        buffer.print();
    }

    ENDSTREAM(rOldStream)
}

int getRecordFromBin(const string& filename, int number, Ticket*& toWrite)
{
    if (number < 0)
    {
        return -2;
    }

    ifstream rOldStream;
    rOldStream.open(filename, ios::ios_base::binary);
    CHECK(rOldStream, -1)


    rOldStream.seekg(0, std::ifstream::end);
    int fileLength = rOldStream.tellg();

    if (number * sizeof(Ticket) > fileLength)
    {
        rOldStream.close();
        return -2;
    }

    rOldStream.seekg(0, std::ifstream::beg);
    rOldStream.seekg((number - 1) * sizeof(Ticket));
    Ticket buffer;
    rOldStream.read((char*)&buffer, sizeof(Ticket));
    toWrite = new Ticket(buffer);

    ENDSTREAM(rOldStream)
}

int deleteRecordFromBinByKey(const string& filename, int key)
{
    fstream rOldStream;
    rOldStream.open(filename, ios::ios_base::binary | ios::ios_base::in | ios::ios_base::out);
    CHECK(rOldStream, -1)

    int offset = 0;
    bool isFind = false;
    int iteration = 0;
    int positionBuffer = 0;
    int iter = 0;
    Ticket buffer;

    rOldStream.seekg(0, std::fstream::end);
    int recordCount = rOldStream.tellg();
    recordCount = recordCount / sizeof(Ticket);
    rOldStream.seekg(0, std::fstream::beg);

    while (!(rOldStream.peek() == EOF || iter > recordCount))
    {
        rOldStream.read((char*)&buffer, sizeof(Ticket));

        if (isFind)
        {
            positionBuffer = rOldStream.tellg();
            rOldStream.seekg(offset + iteration * sizeof(Ticket));
            rOldStream.write((char*)&buffer, sizeof(Ticket));
            rOldStream.seekg(positionBuffer);
            iteration++;
        }
        else
        {
            if (buffer.key == key)
            {
                isFind = true;
                offset *= sizeof(Ticket);
            }
            else
            {
                offset++;
            }
        }
        iter++;
    }


    if (!isFind)
    {
        rOldStream.close();
        return -2;
    }

    rOldStream.close();
    filesystem::resize_file(filename, (recordCount - 1) * sizeof(Ticket));
    return rOldStream.bad();
}


void testBinF(const string& txtFilename)
{
    cout << "Начало тестирования" << "binary module" << endl;
    int result;
    string binFilename = "testbin.bin";
    LINE()
    cout << "Конвертация из " << txtFilename << " в " << binFilename << "..." << endl;
    result = convertTextToBin(txtFilename, binFilename);
    TESTCODE(result)

    LINE()
    cout << "Вывод содержимого из " << binFilename << endl;
    result = printFromBin(binFilename);
    TESTCODE(result)

    int recordNumber = 1;
    Ticket* record = nullptr;
    LINE()
    cout << "Получение записи под номером " << recordNumber << " из файла " << binFilename << "..." << endl;
    result = getRecordFromBin(binFilename, recordNumber, record);
    TESTCODE(result)
    if (result == 0)
    {
        if (record == nullptr)
        {
            cout << "Ошибка, функция вернула успех, но запись nullptr" << endl;
        }
        else
        {
            record->print();
            CLEAR(record)
        }
    }

    recordNumber = 923;
    LINE()
    cout << "Удаление записи с ключом " << recordNumber << " из файла " << binFilename << "..." << endl;
    cout << "Файл до: " << endl;
    printFromBin(binFilename);
    cout << "Начало операции" << endl;
    result = deleteRecordFromBinByKey(binFilename, recordNumber);
    cout << endl << "Файл после:" << endl;
    printFromBin(binFilename);
    TESTCODE(result)

    LINE()
    cout << "Добавление записи:" << endl;
    printFromBin(binFilename);

    cout << "Запись:" << endl;
    record = new Ticket();
    record->key = 1235;
    strcpy(record->fio, "Павлов К.С.");
    strcpy(record->phoneNumber, "+923545341123");
    record->print();
    result = addRecordToBin(binFilename, record);
    cout << endl << "Файл после добавления" << endl;
    printFromBin(binFilename);
    TESTCODE(result)

    cout << "Тестирование завершено" << endl;
}