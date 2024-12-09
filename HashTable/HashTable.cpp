#include <iostream>
#include "HashTable.h"

using namespace std;


int main() {

    HashTable<int>::testHashTable();

    // Пример использования хеш-таблицы с DJB2 хеш-функцией
    HashTable<int> hashTable(djb2Hash<int>);

    // Вставляем достаточно элементов, чтобы вызвать resize
    for (int i = 0; i < 10; ++i) {
        hashTable.insert(i);
        std::cout << "Max load factor: " << hashTable.get_maxLoadFactor() << ", Current load factor: " << hashTable.get_loadFactor() << std::endl;
        std::cout << "Inserted: " << i << ", Size: " << hashTable.size() << std::endl;
    }

    std::cout << "Final size: " << hashTable.size() << std::endl;

    // Проверка наличия элементов после вставки
    std::cout << "Contains 5: " << hashTable.contains(5) << std::endl;

    // Удаление элемента
    hashTable.remove(5);
    std::cout << "Contains 5 after removal: " << hashTable.contains(5) << std::endl;

    return 0;
}
