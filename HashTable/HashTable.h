﻿#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <stdexcept>
#include <cassert>
#include <algorithm>
#include <string>

using namespace std;

/// <summary> 
/// DJB2 — простая хеш-функция, разработанная Дэниелом Дж. Бернстайном.  
/// Часто используется для строк и обеспечивает хорошее распределение.  
/// Не предназначена для криптографических целей. 
///  Вероятность равномерного распределения низкая. Эта функция подвержена проблемам с кластеризацией. 
/// </summary> 
/// <typeparam name="Key">Тип ключа</typeparam> 
/// <param name="key">Ключ для хеширования</param> 
/// <returns>Хеш-значение для данного ключа</returns>
template <typename Key>
size_t djb2Hash(const Key& key) {
    unsigned int hash = 5381; // Начальное значение хеша
    // reinterpret_cast в C++ — это оператор приведения типов, который позволяет выполнять 
    // преобразование указателей и ссылок между несовместимыми типами. 
    // Он используется, когда необходимо интерпретировать данные одного типа 
    // как данные другого типа без проверки на безопасность преобразования.
    const char* str = reinterpret_cast<const char*>(&key); // Приводим ключ к строке
    int c;

    while ((c = *str++)) { // Пока не достигнут конец строки
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash; // Возвращаем окончательный хеш
}

/// <summary> 
/// FNV (англ. Fowler–Noll–Vo) — простая хеш-функция для общего применения,  
/// разработанная Гленом Фаулером, Лондоном Керт Нолом и Фогном Во.  
/// Не является криптографической функцией. Вероятность равномерного распределения средняя
/// </summary> 
/// <typeparam name="Key">Тип ключа</typeparam> 
/// <param name="key">Ключ для хеширования</param> 
/// <returns>Хеш-значение для данного ключа</returns>
template <typename Key>
size_t fnv1aHash(const Key& key) {
    size_t hash = 2166136261; // Начальное значение FNV-1a хеша
    auto key_hash = std::hash<Key>{}(key); // Получаем стандартный хеш для ключа
    hash = (hash ^ key_hash) * 16777619; // Применяем XOR и умножение для обновления хеша
    return hash; // Возвращаем окончательный хеш
}

/// <summary> 
/// MurmurHash — это не криптографическая хеш-функция,  
/// которая обеспечивает хорошую производительность и равномерное распределение. 
/// Подходит для использования в хеш-таблицах и других структурах данных. 
/// </summary> 
/// <typeparam name="Key">Тип ключа</typeparam> 
/// <param name="key">Ключ для хеширования</param> 
/// <returns>Хеш-значение для данного ключа</returns>
template <typename Key>
size_t murmurHash(const Key& key) {
    unsigned int seed = 0; // Сид для генерации хеша
    unsigned int hash = seed ^ (sizeof(Key) * 0x5bd1e995); // Начинаем с сидированного значения
    const unsigned char* data = reinterpret_cast<const unsigned char*>(&key);
    size_t len = sizeof(Key);

    for (size_t i = 0; i < len; i++) {
        hash ^= data[i]; // XOR с каждым байтом
        hash *= 0x5bd1e995; // Умножаем на константу
        hash ^= hash >> 15; // Сдвиг и XOR для разброса битов
    }

    return hash; // Возвращаем окончательный хеш
}

/// <summary>
/// Легкая функция для тестирования коллизий
/// </summary>
/// <typeparam name="Key">тип ключа</typeparam>
/// <param name="key">сам ключ</param>
/// <returns>хеш значение</returns>
template <typename Key>
size_t too_easy_hash(const Key& key) {
    return key % 10;
}

/// <summary>
/// Шаблонный класс Хеш таблицы. Хеш-табли́ца — структура данных, реализующая интерфейс ассоциативного массива, 
/// а именно, она позволяет хранить пары (ключ, значение) и выполнять три операции:
/// операцию добавления новой пары, 
/// операцию удаления и 
/// операцию поиска пары по ключу.
/// Возможно задание произвольной хеш-функции
/// </summary>
/// <typeparam name="Key">Тип хеш таблицы</typeparam>
template <typename Key>
class HashTable {
private:
    std::vector<std::list<Key>> table; // Вектор, в котором хранятся ключи
    // таблица представлена в виде вектора, где каждый элемент представляет собой связный список
    // Каждый связный список (или "ведро") будет хранить ключи, которые имеют одинаковый хеш-индекс (то есть произошло совпадение хешей).
    std::function<size_t(const Key&)> hashFunction; // Хеш-функция
    size_t _size; // Количество элементов в таблице
    double loadFactor; // Коэффициент заполнения
    double maxLoadFactor; // Максимальный коэффициент загрузки
    double minLoadFactor; // Минимальный коэффициент загрузки

    /// <summary> 
    /// Вычисляет индекс в таблице на основе хеш-значения ключа. 
    /// </summary> 
    /// <param name="key">Ключ, для которого необходимо вычислить индекс.</param> 
    /// <returns>Индекс в таблице, соответствующий данному ключу.</returns>
    size_t hashIndex(const Key& key) const {
        return hashFunction(key) % table.size();
    }


    // +0.5 елси будет ресайз на меньшую minFactor
    // колизия на такойто list юlistюlistюlistв table
    // в сет можно сделать операции над множествами их перегрузкой


    /// <summary> 
    /// Изменяет размер таблицы, увеличивая её емкость в два раза. 
    /// </summary> 
    /// <remarks> 
    /// BigO: Average - O(2n)
    /// Переносит все элементы из текущей таблицы в новую таблицу с увеличенной емкостью. 
    /// Это необходимо для оптимизации хранения элементов при превышении максимального коэффициента загрузки. 
    /// </remarks>
    void resizeUp() {
        size_t newCapacity = table.size() * 2;
        std::vector<std::list<Key>> newTable(newCapacity);

        // Переносим все элементы в новую таблицу
        for (const auto& bucket : table) {
            for (const auto& key : bucket) {
                size_t newIndex = hashFunction(key) % newCapacity;
                newTable[newIndex].push_back(key);
            }
        }

        table.swap(newTable); // Обновляем таблицу
    }


    /// <summary> 
    /// Изменяет размер таблицы, уменьшая её емкость в два раза. 
    /// </summary> 
    /// <remarks> 
    /// BigO: Average - O(n/2)
    /// Переносит все элементы из текущей таблицы в новую таблицу с уменьшенной емкостью. 
    /// Это необходимо для оптимизации хранения элементов при уменьшении минимального коэффициента загрузки. Минимальная емкость таблицы - 10 елементов
    /// </remarks>
    void resizeDown() {
        size_t newCapacity = table.size() / 2; // Уменьшаем емкость вдвое
        if (newCapacity < 10) {  // Ограничение минимальной емкости
            newCapacity = 10;    // Если новый размер становится меньше минимальной емкости - ничего не делаем
            return;
        }
        std::vector<std::list<Key>> newTable(newCapacity);

        // Переносим все элементы в новую таблицу
        for (const auto& bucket : table) {
            for (const auto& key : bucket) {
                size_t newIndex = hashFunction(key) % newCapacity;
                newTable[newIndex].push_back(key);
            }
        }

        table.swap(newTable); // Обновляем таблицу
    }

public:
    /// <summary> 
    /// Конструктор HashTable, инициализирует таблицу заданной емкостью и хеш-функцией. 
    /// </summary> 
    /// <param name="hashFunc">Функция, используемая для вычисления хеша ключей.</param> 
    /// <param name="capacity">Начальная емкость таблицы (по умолчанию 10). Это минимальная емкость таблицы - 10 елементов</param> 
    /// <param name="maxLoad">Максимальный коэффициент загрузки (по умолчанию 0.7).</param>
    /// <param name="maxLoad">Минимальный коэффициент загрузки (по умолчанию 0.3).</param>  
    HashTable(std::function<size_t(const Key&)> hashFunc, size_t capacity = 10, double maxLoad = 0.7, double minLoad = 0.3)
        : _size(0), maxLoadFactor(maxLoad), minLoadFactor(minLoad), hashFunction(hashFunc) {
        table.resize(capacity);
    }

    /// <summary> 
    /// Добавляет новый элемент в таблицу. 
    /// BigO: Average - O(1), Worst - O(2n)
    /// </summary> 
    /// <param name="key">Ключ, который необходимо добавить в таблицу.</param> 
    /// <remarks> 
    /// Если таблица заполнена до максимально допустимого коэффициента загрузки, 
    /// происходит изменение размера таблицы. 
    /// </remarks>
    void insert(const Key& key) {
        // Проверяем необходимость увеличения размера таблицы
        if (loadFactor >= maxLoadFactor) {
            resizeUp();
        }

        size_t index = hashIndex(key);
        table[index].push_back(key);
        _size++;
        loadFactor = static_cast<double>(_size) / table.size();

        // Проверяем необходимость уменьшения размера таблицы, у которой 10 - это минимальный размер по умолчанию
        if (loadFactor < minLoadFactor && table.size() > 10) {
            resizeDown();
        }
    }

    /// <summary> 
    /// Проверяет, существует ли указанный элемент в таблице. 
    /// BigO: Average - O(1), Worst - O(n)
    /// </summary> 
    /// <param name="key">Ключ, который необходимо проверить на наличие.</param> 
    /// <returns>Возвращает true, если элемент существует, в противном случае false.</returns>
    bool contains(const Key& key) const {
        size_t index = hashIndex(key);
        for (const auto& item : table[index]) {
            if (item == key) {
                return true;
            }
        }
        return false;
    }

    /// <summary> 
    /// Удаляет указанный элемент из таблицы. 
    /// BigO: Average - O(1), Worst - O(n)
    /// </summary> 
    /// <param name="key">Ключ, который необходимо удалить.</param> 
    /// <remarks> 
    /// Если элемент не найден, будет сгенерировано исключение runtime_error. 
    /// </remarks>
    void remove(const Key& key) {
        size_t index = hashIndex(key);
        auto& bucket = table[index];

        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (*it == key) {
                bucket.erase(it);
                _size--;
                loadFactor = static_cast<double>(_size) / table.size();

                // Проверяем необходимость уменьшения размера таблицы
                if (loadFactor < minLoadFactor && table.size() > 10) {
                    resizeDown();
                }

                return;
            }
        }
        throw std::runtime_error("Key not found");
    }

    /// <summary> 
    /// Возвращает текущее количество элементов в таблице. 
    /// </summary> 
    /// <returns>Количество элементов в таблице.</returns>
    size_t size() const {
        return _size;
    }

    /// <summary> 
    /// Возвращает текущее capacity таблицы. 
    /// </summary> 
    /// <returns>capacity таблицы.</returns>
    size_t capacity() const {
        return table.size();
    }

    /// <summary> 
    /// Возвращает текущий коэффициент заполнения таблицы. 
    /// </summary> 
    /// <returns>Коэффициент заполнения.</returns>
    double get_loadFactor() const {
        return loadFactor;
    }

    /// <summary> 
    /// Возвращает максимальный коэффициент загрузки таблицы. 
    /// </summary> 
    /// <returns>Максимальный коэффициент заполнения.</returns>
    double get_maxLoadFactor() const {
        return maxLoadFactor;
    }

    /// <summary> 
    /// Возвращает минимальный коэффициент загрузки таблицы. 
    /// </summary> 
    /// <returns>Минимальный коэффициент заполнения.</returns>
    double get_minLoadFactor() const {
        return minLoadFactor;
    }

    /// <summary> 
    /// Проверяет равенство двух ключей. 
    /// </summary> 
    /// <param name="key1">Первый ключ для сравнения.</param> 
    /// <param name="key2">Второй ключ для сравнения.</param> 
    /// <returns>Возвращает true, если ключи равны, иначе false.</returns>
    bool key_equality(const Key& key1, const Key& key2) const {
        return key1 == key2;
    }

    /// <summary>
    /// Метод очистки хеш таблицы
    /// </summary>
    void clear() {
        for (auto& bucket : table) {
            bucket.clear();  // Очищаем каждый "ведро"
        }
        _size = 0; // Сбрасываем количество элементов
        loadFactor = 0; // Сбрасываем коэффициент загрузки
    }

    class Iterator {
    private:
        HashTable& hashTable; // Ссылка на хеш-таблицу, к которой относится итератор 
        size_t bucketIndex; // Индекс текущего ведра
        typename std::list<Key>::iterator listIterator; // Итератор по ведру

        /// <summary> 
        /// Ищет следующий непустой ведро и обновляет индексы. 
        /// </summary> 
        /// <remarks> 
        /// Этот метод помогает убедиться, что итератор указывает на действительный элемент, 
        /// если текущий ведро пуст или итератор достиг конца текущего ведра. 
        /// </remarks>
        void findNext() {
            while (bucketIndex < hashTable.table.size() && (hashTable.table[bucketIndex].empty() || listIterator == hashTable.table[bucketIndex].end())) {
                bucketIndex++;
                if (bucketIndex < hashTable.table.size()) {
                    listIterator = hashTable.table[bucketIndex].begin();
                }
            }
        }

    public:
        /// <summary> 
        /// Конструктор итератора, инициализирует итератор для заданной хеш-таблицы и индекса ведра. 
        /// </summary> 
        /// <param name="ht">Ссылка на хеш-таблицу, которую нужно итератору обходить.</param> 
        /// <param name="index">Начальный индекс ведра, откуда начинается итерация.</param> 
        /// <remarks> 
        /// Конструктор сразу ищет первый элемент, чтобы установить начальное состояние итератора. 
        /// </remarks>
        Iterator(HashTable& ht, size_t index)
            : hashTable(ht), bucketIndex(index), listIterator(ht.table[index].begin()) {
            findNext(); // Ищем первый элемент
        }

        /// <summary> 
        /// Доступ к текущему элементу, на который указывает итератор. 
        /// </summary> 
        /// <returns>Текущий ключ, на который указывает итератор.</returns>
        Key operator*() const {
            return *listIterator;
        }

        /// <summary> 
        /// Перемещает итератор к следующему элементу. 
        /// </summary> 
        /// <returns>Ссылку на текущий итератор после перемещения.</returns> 
        /// <remarks> 
        /// Увеличивает итератор на следующую позицию в текущем ведре и ищет следующий непустой ведро. 
        /// </remarks>
        Iterator& operator++() {
            ++listIterator; // Переходим к следующему элементу в текущем ведре
            findNext();     // Находим следующее непустое ведро
            return *this;
        }

        /// <summary> 
        /// Проверяет не равны ли два итератора. 
        /// </summary> 
        /// <param name="other">Итератор для сравнения.</param> 
        /// <returns>Возвращает true, если итераторы не равны, иначе false.</returns>
        bool operator!=(const Iterator& other) const {
            return (bucketIndex != other.bucketIndex || listIterator != other.listIterator);
        }
    };

    /// <summary> 
    /// Возвращает итератор на начало хеш-таблицы. 
    /// </summary> 
    /// <returns>Итератор, указывающий на первый элемент хеш-таблицы.</returns>
    Iterator begin() {
        return Iterator(*this, 0);
    }

    /// <summary> 
    /// Возвращает итератор на конец хеш-таблицы. 
    /// </summary> 
    /// <returns>Итератор, указывающий на "конец" хеш-таблицы (позиция за последним элементом).</returns>
    Iterator end() {
        return Iterator(*this, table.size()-1);
    }

    /// <summary>
    /// Веселая функция тетсирования :::):)::_):):):):):)_%)_:):):):)::):_
    /// </summary>
    static void testHashTable() {
        // Тестирование с типом int и хеш-функцией DJB2
        HashTable<int> hashTableDJB2(djb2Hash<int>);
        hashTableDJB2.insert(1);
        hashTableDJB2.insert(2);
        hashTableDJB2.insert(3);

        // Проверка наличия элементов
        assert(hashTableDJB2.contains(1)); // 1 должен присутствовать в таблице
        assert(!hashTableDJB2.contains(4)); // 4 не должен присутствовать в таблице

        // Проверка коллизий
        hashTableDJB2.insert(11); // 11 может вызвать коллизию с 1
        assert(hashTableDJB2.contains(11)); // 11 должен присутствовать в таблице

        // Удаление элемента
        hashTableDJB2.remove(2);
        assert(!hashTableDJB2.contains(2)); // 2 не должен присутствовать в таблице после удаления

        // Проверка переполнения
        for (int i = 4; i < 11; ++i) {
            hashTableDJB2.insert(i);
        }

        assert(hashTableDJB2.size() == 10); // Проверка общего количества элементов после вставки

        // Тестирование с типом std::string и хеш-функцией FNV-1a
        HashTable<std::string> hashTableFNV(fnv1aHash<std::string>);
        hashTableFNV.insert("hello");
        hashTableFNV.insert("world");
        hashTableFNV.insert("foo");
        hashTableFNV.insert("bar");

        assert(hashTableFNV.contains("hello")); // "hello" должен присутствовать в таблице
        assert(!hashTableFNV.contains("baz")); // "baz" не должен присутствовать в таблице

        // Удаление элемента
        hashTableFNV.remove("foo");
        assert(!hashTableFNV.contains("foo")); // "foo" не должен присутствовать в таблице после удаления

        // Проверка переполнения
        hashTableFNV.insert("test1");
        hashTableFNV.insert("test2");
        hashTableFNV.insert("test3");

        assert(hashTableFNV.size() == 6); // Проверка общего количества элементов после вставки

        // Тестирование с пользовательским типом (например, Простая структура)
        struct CustomObject {
            int value;

            // Оператор равенства для сравнения
            bool operator==(const CustomObject& other) const {
                return value == other.value;
            }
        };

        HashTable<CustomObject> hashTableCustom(djb2Hash<CustomObject>);
        CustomObject obj1{ 10 }, obj2{ 20 }, obj3{ 30 };

        hashTableCustom.insert(obj1);
        hashTableCustom.insert(obj2);
        hashTableCustom.insert(obj3);

        assert(hashTableCustom.contains(obj1)); // obj1 должен присутствовать в таблице
        assert(hashTableCustom.contains(obj2)); // obj2 должен присутствовать в таблице
        assert(!hashTableCustom.contains(CustomObject{ 40 })); // Объект со значением 40 не должен присутствовать

        // Удаление пользовательского объекта
        hashTableCustom.remove(obj2);
        assert(!hashTableCustom.contains(obj2)); // obj2 не должен присутствовать в таблице после удаления

        cout << "Using Iterator To Walk In Int Digits" << endl;
        HashTable<int> hashTableIter(djb2Hash<int>);
        hashTableIter.insert(1);
        hashTableIter.insert(2);
        hashTableIter.insert(3);

        // Тесты итератора
        for (auto it = hashTableIter.begin(); it != hashTableIter.end(); ++it) {
            std::cout << *it << std::endl; // Выводит все элементы в хеш-таблице
        }

        cout << "Using Iterator To Walk In Strings" << endl;
        HashTable<string> hashTableIterSt(djb2Hash<string>);
        hashTableIterSt.insert("I");
        hashTableIterSt.insert("REALY Dont");
        hashTableIterSt.insert("Like");
        hashTableIterSt.insert("Backend");

        for (const auto& element : hashTableIterSt) {
            std::cout << element << std::endl;
        }

        // Участок кода для тестирования коллизий
        HashTable<int> hashTableEasy(too_easy_hash<int>, 10, 0.7, 0.5);
        // Вставляем множество значений, которые должны вызывать коллизии
        for (int i = 0; i < 30; ++i) {
            hashTableEasy.insert(i); // Это создано так, чтобы i % 10 выдавало значения от 0 до 9
        }

        // Проверка наличия значений
        for (int i = 0; i < 30; ++i) {
            assert(hashTableEasy.contains(i)); // Все числа от 0 до 29 должны присутствовать
        }
        // Проверка наличия только некоторых значений для коллизий
        for (int i = 0; i < 10; ++i) {
            assert(hashTableEasy.contains(i)); // значения от 0 до 9 должны присутствовать
        }
        // Проверка, что значение 30 не присутствует
        assert(!hashTableEasy.contains(30)); // 30 не должно присутствовать

        // Тестирование функции resizeDown
        for (int i = 0; i < 20; ++i) { // вставляем другие элементы
            hashTableEasy.insert(i + 30); // От 30 до 49
        }

        assert(hashTableEasy.size() == 50); // Размер должен быть 50 (30 + 20)

        // Удаляем несколько элементов, дойдя до точки, где может сработать resizeDown
        for (int i = 0; i < 30; ++i) {
            hashTableEasy.remove(i); // Удаляем элементы 0-29
        }

        // Теперь таблица должна инициировать resizeDown
        hashTableEasy.remove(30); // Удаляем еще один, чтобы случилась resizeDown
        assert(hashTableEasy.capacity() == 20); // Проверяем размер после уменьшения

        assert(!hashTableEasy.contains(29)); // Проверяем, что 29 отсутствует


        // Тестируем maxLoadFactor с resize
        HashTable<int> hashTableSize(djb2Hash<int>);

        // Вставляем достаточно элементов, чтобы вызвать resize
        for (int i = 0; i < 10; ++i) {
            hashTableSize.insert(i);
        }

        assert(hashTableSize.capacity() == 20); // Проверяем размер после увеличился
        for (int i = 0; i < 10; ++i) {
            hashTableSize.contains(i); // Проверяем что эжлементы есть
        }

        std::cout << "All HASH tests completed successfully.\n";
    }
};