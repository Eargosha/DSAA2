#include <vector>
#include <list>
#include <functional>
#include <stdexcept>
#include <utility>
#include "HashTable.h"

/// <summary>
/// Словарь — это тип данных, представляющий собой коллекцию пар «ключ-значение».
/// Реализован на основе хеш таблицы
/// </summary>
/// <typeparam name="Key">Тип ключа</typeparam>
/// <typeparam name="Value">Тип значения</typeparam>
template <typename Key, typename Value>
class Dictionary {
private:
    // Хеш-таблица, где ключ - это Key, а значение - это Value 
    HashTable<std::pair<Key, Value>> hashTable;

    /// <summary> 
    /// Создание хеш-функции для пар (ключ, значение).  
    /// </summary> 
    /// <param name="pair">Пара (ключ, значение) для хеширования.</param>
    /// <returns>Хеш-код для данной пары.</returns>
    /// <BigO>O(1)</BigO> 
    static size_t hashFunction(const std::pair<Key, Value>& pair) {
        return fnv1aHash<Key>(pair.first);
    }

public:
    /// <summary> 
    /// Конструктор Dictionary. 
    /// </summary> 
    /// <param name="capacity">Начальная ёмкость хеш-таблицы.</param> 
    /// <param name="maxLoad">Максимальная загрузка хеш-таблицы.</param> 
    /// <BigO>O(1)</BigO>
    Dictionary(size_t capacity = 10, double maxLoad = 0.7)
        : hashTable(hashFunction, capacity, maxLoad) {}

    /// <summary> 
    /// Вставка пары (ключ, значение) в словарь. 
    /// Если ключ уже существует, обновляет значение.  
    /// </summary> 
    /// <param name="key">Ключ для вставки или обновления.</param>  
    /// <param name="value">Значение, соответствующее ключу.</param>  
    /// <BigO>Среднее : O(n)</BigO> 
    void put(const Key& key, const Value& value) {
        // Ищем существующий ключ
        for (auto pair : hashTable) {
            if (pair.first == key) {
                hashTable.remove(pair);
                hashTable.insert(std::make_pair(key, value));
                // Обновляем значение
                return;
            }
        }
        // Если ключ не найден, вставляем новую пару
        hashTable.insert(std::make_pair(key, value));
    }

    /// <summary> 
    /// Получение значения по ключу.  
    /// </summary>  
    /// <param name="key">Ключ для поиска значения.</param>  
    /// <returns>Значение, соответствующее ключу.</returns>  
    /// <BigO>Среднее : O(n)</BigO> 
    Value get(const Key& key) {
        for (const auto& pair : hashTable) {
            if (pair.first == key) {
                return pair.second;
            }
        }
        throw std::runtime_error("Key not found");
    }

    /// <summary> 
    /// Удаление пары (ключ, значение) по ключу.  
    /// </summary>  
    /// <param name="key">Ключ для удаления.</param>  
    /// <BigO>Среднее : O(n)</BigO>
    void remove(const Key& key) {
        for (const auto& pair : hashTable) {
            if (pair.first == key) {
                hashTable.remove(pair);
                return;
            }
        }
        throw std::runtime_error("Key not found");
    }

    /// <summary> 
    /// Возвращает текущее количество пар в словаре.  
    /// </summary>  
    /// <returns>Количество пар в словаре.</returns>  
    /// <BigO>O(1)</BigO>
    size_t size() const {
        return hashTable.size();
    }

    /// <summary>
    /// Метод очистки словаря
    /// </summary>
    void clear() {
        hashTable.clear();
    }

    // Итератор для Dictionary
    class Iterator {
    private:
        //Определение типа по возращаемому значению переданному в него
        decltype(hashTable.begin()) current; // Текущий итератор хеш-таблицы
        decltype(hashTable.end()) end; // Конечный итератор

    public:
        // Конструктор
        Iterator(decltype(hashTable.begin()) start, decltype(hashTable.end()) end)
            : current(start), end(end) {}

        // Оператор разыменования
        std::pair<Key, Value> operator*() {
            return *current; // Возвращает текущую пару
        }

        // Оператор инкремента
        Iterator& operator++() {
            ++current; // Переход к следующему элементу
            return *this;
        }

        // Оператор сравнения
        bool operator!=(const Iterator& other) const {
            return current != other.current; // Сравнение для итераторов
        }
    };

    // Метод для получения начала итератора
    Iterator begin() {
        return Iterator(hashTable.begin(), hashTable.end());
    }

    // Метод для получения конца итератора
    Iterator end() {
        return Iterator(hashTable.end(), hashTable.end());
    }

    static void testDictionary() {
        // Тест 1: Словарь со строковыми ключами и целочисленными значениями
        Dictionary<std::string, int> stringIntDict;
        stringIntDict.put("apple", 2);
        stringIntDict.put("banana", 5);
        stringIntDict.put("orange", 3);

        assert(stringIntDict.get("apple") == 2);
        assert(stringIntDict.get("banana") == 5);
        assert(stringIntDict.get("orange") == 3);

        // Обновить значение
        stringIntDict.put("orange", 4);
        assert(stringIntDict.get("orange") == 4);

        // Попытка получить несуществующий ключ
        try {
            stringIntDict.get("grape");
            assert(false);
        }
        catch (const std::runtime_error& e) {
            assert(true);
        }

        // Тест 2: Пустой словарь
        Dictionary<std::string, int> emptyDict;

        assert(emptyDict.size() == 0); // Проверяем, что размер пустого словаря равен 0

        // Еще раз проверить получение значения
        try {
            emptyDict.get("nonexistent");
            assert(false);
        }
        catch (const std::runtime_error& e) {
            assert(true);
        }

        // Тест 3: Словарь с целочисленными ключами и строковыми значениями
        Dictionary<int, std::string> intStringDict;
        intStringDict.put(1, "one");
        intStringDict.put(2, "two");
        intStringDict.put(3, "three");

        assert(intStringDict.get(1) == "one");
        assert(intStringDict.get(2) == "two");


        // Тест 4: Проверка размера и удаления элементов
        assert(intStringDict.size() == 3);
        intStringDict.remove(2);
        assert(intStringDict.size() == 2); // После удаления должен остаться размер 2

        // Попытка получить удаленный ключ
        try {
            intStringDict.get(2);
            assert(false);
        }
        catch (const std::runtime_error& e) {
            assert(true);
        }

        Dictionary<std::string, int> dict;
        dict.put("apple", 1);
        dict.put("banana", 2);
        dict.put("orange", 3);

        for (const auto& pair : dict) {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }

        for (const auto& element : dict) {
            std::cout << element.first << ": " << element.second << std::endl;
        }

        // Общий тест завершен
        std::cout << "All DIC tests passed successfully!" << std::endl;
    }
};