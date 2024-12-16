#pragma once
#include <vector>
#include <list>
#include <functional>
#include <stdexcept>
#include <utility>
#include "HashTable.h"

/// <summary>
/// Класс, представляющий множество уникальных элементов на основе хеш-таблицы.
/// Позволяет выполнять операции вставки, проверки наличия, удаления и очистки элементов.
/// </summary>
/// <BigO> 
/// O(1) в среднем для вставки, проверки и удаления. 
/// O(n) в худшем случае для операций из-за коллизионных условий.
/// </BigO>
template <typename Value>
class Set {
private:
    HashTable<Value> hashTable; // Хеш-таблица для хранения элементов множества

public:
    /// <summary>
    /// Конструктор класса Set.
    /// Инициализирует хеш-таблицу с заданной вместимостью и максимальной загрузкой.
    /// </summary>
    /// <param name="capacity">Начальная ёмкость хеш-таблицы.</param>
    /// <param name="maxLoad">Максимальная допустимая нагрузка хеш-таблицы перед её увеличением.</param>
    /// <returns>Конструктор не возвращает значения.</returns>
    Set(size_t capacity = 10, double maxLoad = 0.7)
        : hashTable([](const Value& key) { return fnv1aHash<Value>(key); }, capacity, maxLoad) {}

    /// <summary>
    /// Вставка элемента в множество с проверкой на дубликат.
    /// Если элемент уже существует, повторная вставка не происходит.
    /// </summary>
    /// <param name="value">Элемент, который необходимо вставить в множество.</param>
    /// <returns>Нет возвращаемого значения.</returns>
    void insert(const Value& value) {
        if (!hashTable.contains(value)) { // Проверка на существование элемента 
            hashTable.insert(value); // Вставка, только если элемента нет 
        }
    }

    /// <summary>
    /// Проверка на наличие элемента в множестве.
    /// </summary>
    /// <param name="value">Элемент, наличие которого нужно проверить.</param>
    /// <returns>Возвращает true, если элемент присутствует, иначе false.</returns>
    bool contains(const Value& value) const {
        return hashTable.contains(value);
    }

    /// <summary>
    /// Удаление элемента из множества.
    /// Если элемент существует, он будет удалён.
    /// </summary>
    /// <param name="value">Элемент, который нужно удалить из множества.</param>
    /// <returns>Нет возвращаемого значения.</returns>
    void remove(const Value& value) {
        hashTable.remove(value);
    }

    /// <summary>
    /// Очистка множества. 
    /// Удаляет все элементы из множества.
    /// </summary>
    /// <returns>Нет возвращаемого значения.</returns>
    void clear() {
        hashTable.clear();
    }

    /// <summary>
    /// Получение количества элементов в множестве.
    /// </summary>
    /// <returns>Количество элементов в множестве.</returns>
    size_t size() const {
        return hashTable.size();
    }

    /// <summary>
    /// Итератор множества. Можно унаследовать
    /// </summary>
    class Iterator {
    private:
        typename HashTable<Value>::Iterator hashTableIterator; // Внутренний итератор HashTable

    public:
        Iterator(typename HashTable<Value>::Iterator it) : hashTableIterator(it) {}

        Value& operator*() {
            return *hashTableIterator;
        }

        Iterator& operator++() {
            ++hashTableIterator;
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return hashTableIterator == other.hashTableIterator;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    // Методы для получения итераторов
    Iterator begin() {
        return Iterator(hashTable.begin()); // Возвращаем итератор на первый элемент
    }

    Iterator end() {
        return Iterator(hashTable.end()); // Возвращаем итератор на элемент после последнего
    }

    /// <summary>
    /// Функция для тестирования класса Set
    /// </summary>
    static void testSet() {
        // Тестирование для типа int
        Set<int> intSet;

        // Проверка пустоты
        assert(intSet.size() == 0); // Проверяем, что множество пустое

        // Вставка элементов
        intSet.insert(1);
        intSet.insert(2);
        intSet.insert(3);
        intSet.insert(2); // Дубликат

        // Проверка содержимого
        assert(intSet.size() == 3); // Проверяем размер после вставок
        assert(intSet.contains(1) == true); // Проверяем наличие элемента
        assert(intSet.contains(2) == true);
        assert(intSet.contains(3) == true);
        assert(intSet.contains(4) == false); // Элемента нет

        // Удаление элемента
        intSet.remove(2);
        assert(intSet.size() == 2); // Проверяем размер после удаления
        assert(intSet.contains(2) == false); // Проверяем, что элемент удалён

        // Очистка множества
        intSet.clear();
        assert(intSet.size() == 0); // Проверяем, что множество пустое после очистки

        // Тестирование для типа std::string
        Set<std::string> strSet;

        // Вставка строк
        strSet.insert("apple");
        strSet.insert("banana");
        strSet.insert("orange");
        strSet.insert("banana"); // Дубликат

        // Проверка содержимого
        assert(strSet.size() == 3); // Проверяем размер после вставок
        assert(strSet.contains("apple") == true); // Проверяем наличие элемента
        assert(strSet.contains("banana") == true);
        assert(strSet.contains("grape") == false); // Элемента нет

        // Удаление элемента
        strSet.remove("banana");
        assert(strSet.size() == 2); // Проверяем размер после удаления

        // Очистка множества
        strSet.clear();
        assert(strSet.size() == 0); // Проверяем, что множество пустое после очистки

        // Если все проверки прошли успешно
        std::cout << "All SET tests passed!" << std::endl;
    }
};
