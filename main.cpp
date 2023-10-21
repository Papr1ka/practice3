#include <iostream>
#include "HashBinary.h"
#include "HashTable.h"

class Test
{
    Test()
    {

    }
};

int main() {
    testBinF("input.txt");
    testHeshT();

    HashTable<Test*> table;
    table.get(0);
    test();
    cout << "Общее тестирование завершено" << endl;
    return 0;
}