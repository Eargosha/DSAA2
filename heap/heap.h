// Author - EARGOSHA 

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

/// @brief Класс кучи - Max Heap, в качестве хранения элементов кучи используется std::vector
/// Это бинарное дерево. Элементы определены следующей зависимостью - дети имеют значение болше родителей. Дописать понятие
/// @tparam T - Тип кучи
template <typename T>
class MaxHeap
{
private:
    /// @brief Вектор, в котором хранится куча
    vector<T> heap;

    /// @brief Возвращает индекс родительского узла для заданного индекса.
    /// @param index Заданный индекс
    /// Сложность в среднем и худшем и лучшем - O(1)
    /// @return Результат
    int getParentIndex(int index) const
    {
        return (index - 1) / 2;
    }

    /// @brief Возвращает индекс левого дочернего узла для заданного индекса.
    /// @param index Заданный индекс
    /// Сложность в среднем и худшем и лучшем - O(1)
    /// @return Результат
    int getLeftChildIndex(int index) const
    {
        return 2 * index + 1;
    }

    /// @brief Возвращает индекс правого дочернего узла для заданного индекса.
    /// @param index Заданный индекс
    /// Сложность в среднем и худшем и лучшем - O(1)
    /// @return Результат
    int getRightChildIndex(int index) const
    {
        return 2 * index + 2;
    }

    /// @brief Просеивание вверх: переставляет элемент вверх по куче,
    /// Пока не получится свойство кучи
    /// Сложность в среднем и худшем - O(n) в лучшем - O(1)
    /// @param index Индекс, от которого начинается просеивание
    void heapifyUp(int index)
    {
        // Пока индекс не является корнем и текущий индекс больше родителя
        while (index > 0 && heap[index] > heap[getParentIndex(index)])
        {
            // Меняем местами элемент и его родителя
            swap(heap[index], heap[getParentIndex(index)]);
            // Переход к родителю
            index = getParentIndex(index);
        }
    }

    /// @brief Просеивание вниз: переставляет элемент вниз по куче,
    /// Сложность в среднем и худшем - O(n) в лучшем - O(1)
    /// @param index Индекс, от которого начинаем просеивание
    void heapifyDown(int index)
    {
        int largest = index;
        int leftChildIndex = getLeftChildIndex(index);
        int rightChildIndex = getRightChildIndex(index);

        // Ищем наибольшее значение из трех - левого, правого
        if (leftChildIndex < heap.size() && heap[leftChildIndex] > heap[largest])
        {
            largest = leftChildIndex;
        }

        // Ищем наибольшее значение из трех - левого, правого
        if (rightChildIndex < heap.size() && heap[rightChildIndex] > heap[largest])
        {
            largest = rightChildIndex;
        }

        // Если текущий не является наибольшим продолжаем рекурсию
        if (largest != index)
        {
            // Меняем местами
            swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }

    /// @brief Рекурсивная функция для поиска элемента в куче
    /// @param value Элемент что ищем
    /// @param index От какого места ищем, обычно это корень - 0
    /// Сложность в среднем и худшем - O(n) в лучшем - O(1)
    /// @return Возращает найденное значение, или -1, если элемент не был найден
    T findIt(const T &value, int index) const
    {
        // Out Of Bounce
        if (index >= heap.size())
        {
            return -1; // Элемент не найден
        }

        // Ой, нашли, возращаем элемент
        if (heap[index] == value)
        {
            return heap[index]; // Элемент найден
        }

        // Поиск в левом поддереве
        T leftResult = findIt(value, getLeftChildIndex(index));
        if (leftResult != -1)
        {
            return leftResult; // Элемент найден в левом поддереве
        }

        // Поиск в правом поддереве
        T rightResult = findIt(value, getRightChildIndex(index));
        if (rightResult != -1)
        {
            return rightResult; // Элемент найден в правом поддереве
        }

        return -1; // Элемент не найден
    }

    /// @brief Вспомогательная функция для вывода узла дерева
    /// @param index Индекс узла
    /// @param level Уровень
    void printNode(int index, int level)
    {
        // Вывод отступов для уровня дерева
        for (int i = 0; i < level; ++i)
        {
            cout << " ";
        }

        if (index < heap.size())
        {
            cout << heap[index] << endl; // Вывод значения узла

            // Рекурсивный вызов для левого и правого потомков
            printNode(getLeftChildIndex(index), level + 1);
            printNode(getRightChildIndex(index), level + 1);
        }
    }

public:
    /// @brief Конструктор по умолчанию
    MaxHeap() {}

    /// @brief Конструктор с значениями записанными в массив ветора
    /// @param values Сам вектор
    MaxHeap(const vector<T> &values)
    {
        heap = values;
        buildHeap();
    }

    /// @brief Вставляем элемент в кучу по свойствам кучи
    /// Сложность в среднем и худшем - O(log n) в лучшем - O(1)
    /// @param value Сам элемент
    void insert(T value)
    {
        // Поставим в конец
        heap.push_back(value);
        // Возвращаем свойство кучи
        heapifyUp(heap.size() - 1);
    }

    /// @brief Удаление элемента по индексу
    /// Сложность в среднем и худшем - O(log n) в лучшем - O(1)
    /// @param index Индекс, с которого нужного удалить элемент
    void deleteAtIndex(int index)
    {
        // Проверка на пустоту и на выпадения из границ кучи
        if (index < 0 || index >= heap.size())
        {
            throw out_of_range("Индекс вне диапазона или в куче пусто");
        }

        // Если удаляемый является последним элементом кучи, просто вон его
        if (index == heap.size() - 1)
        {
            heap.pop_back();
            return;
        }

        // Заменяем удаляемый элемент последним элементом кучи.
        heap[index] = heap.back();
        heap.pop_back();

        // Просеиваем элемент вниз или вверх, чтобы восстановить условие кучи.
        if (index == 0 || heap[index] > heap[getParentIndex(index)])
        {
            heapifyUp(index);
        }
        else
        {
            heapifyDown(index);
        }
    }

    /// @brief Поиск элемента в куче по его значению
    /// @param value Значение по которому производится поиск
    /// Сложность в среднем и худшем - O(n log n) в лучшем - O(1)
    /// @return Возращает найденное значение, либо -1 - в том случае, если ничего не нашли :(
    T search(const T &value) const
    {
        return findIt(value, 0);
    }

    /// @brief Перегрузка оператора = !!!!!!!!!!!!! нужен ли это оператор перегруенный
    /// @param other То, к чему приравниваем this
    /// @return Результат
    MaxHeap &operator=(const MaxHeap &other)
    {
        if (this != &other)
        {
            heap = other.heap;
            buildHeap();
        }
        return *this;
    }

    /// @brief Перегрузка оператора [] для доступа к элементам кучи
    /// @param index По какому индексу обращаемся
    /// @return Результат, которого хотели добиться
    T &operator[](size_t index)
    {
        if (index < 0 || index >= heap.size())
        {
            throw out_of_range("Индекс вне диапазона");
        }
        return heap[index];
    }

    /// @brief Перегрузка оператора [] для константных объектов
    /// @param index По какому индексу обращаемс
    /// @return Результат, которого хотели добиться
    const T &operator[](size_t index) const
    {
        if (index < 0 || index >= heap.size())
        {
            throw out_of_range("Индекс вне диапазона");
        }
        return heap[index];
    }

    /// @brief Возращает размер кучи
    /// @return Результат в виде size_t
    /// Сложность в среднем и худшем и лучшем - O(1)
    size_t size() const
    {
        return heap.size();
    }

    /// @brief Проверяет, является ли куча пустой.
    /// @return Логический результат в виде bool
    /// Сложность в среднем и худшем и лучшем - O(1)
    bool isEmpty() const
    {
        return heap.empty();
    }

    /// @brief Очищает кучу
    /// Сложность в среднем и худшем и лучшем - O(1)
    void clear()
    {
        heap.clear();
    }

    /// @brief Строит кучу из существующего вектора.
    /// Сложность в среднем, лучшем и худшем - O(n)
    void buildHeap()
    {
        for (int i = heap.size() / 2 - 1; i >= 0; --i)
        {
            heapifyDown(i);
        }
    }

    /// @brief Вывод кучи в виде бинарного дерева в консоль
    void printTree()
    {
        printNode(0, 0); // Начало с корневого узла
    }

    /// @brief Извлекает и возвращает максимальный элемент из кучи
    /// Сложность в среднем и худшем и лучшем - O(1)
    /// @return Максимальный элемент из кучи
    T extractMax()
    {
        if (isEmpty())
        {
            throw runtime_error("Куча пуста");
        }

        T max = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);

        return max;
    }

    /// @brief Возвращает максимальный элемент из кучи
    /// Сложность в среднем и худшем и лучшем - O(1)
    /// @return Максимальный элемент из кучи
    T getMax() const
    {
        if (isEmpty())
        {
            throw runtime_error("Куча пуста");
        }

        return heap[0];
    }

    /// @brief Сортировка по убыванию
    /// @param vector - то, что сортируем
    /// Сложность в среднем и худшем - O(n log n) в лучшем - O(1)
    static void heapSortDecrease(vector<T> &vec)
	{
		MaxHeap<T> heapForSort;
		for (const auto& item : vec)
		{
			heapForSort.insert(item);
		}
		vector<T> sortedVector;
		while (!heapForSort.isEmpty()) {
			sortedVector.push_back(heapForSort.extractMax());
		}
        vec = sortedVector;
	}

    /// @brief Сортировка по возрастанию
    /// Сложность в среднем и худшем - O(n log n) в лучшем - O(1)
    /// @param vector То, что сортируем
    static void heapSortIncrease(vector<T> &vector)
    {
        // Сортировка по убыванию
        heapSortDecrease(vector);

        // Сортировка по возрастанию. Просто перевернем массив
        reverse(vector.begin(), vector.end());
    }

    /// @brief А я не люблю тестирование, а Вы?)))
    static void testMaxHeap()
    {
        MaxHeap<int> heapHop;

        // Опа, пусто
        assert(heapHop.isEmpty());
        // Заполним
        heapHop.insert(10);
        heapHop.insert(20);
        heapHop.insert(15);
        heapHop.insert(5);
        heapHop.insert(25);
        // Во как здорово получилось то
        assert(heapHop.size() == 5);
        assert(heapHop.getMax() == 25);

        // Надоел он мне - вырвите его отсюда
        assert(heapHop.extractMax() == 25);
        assert(heapHop.size() == 4);
        assert(heapHop.getMax() == 20);

        // Ну и хватит с тебя
        heapHop.clear();
        assert(heapHop.size() == 0);

        // О, массивчик 💀  !!! Сделать на разных массивах сортировку
        vector<int> rawr = {4, 2, 16, 9, 11, 7, 21};
        // Проврим сортировку
        MaxHeap::heapSortDecrease(rawr);
        vector<int> rawrSortedDecrease = {21, 16, 11, 9, 7, 4, 2};
        vector<int> rawrSortedIncrease = {2, 4, 7, 9, 11, 16, 21};
        assert(rawr == rawrSortedDecrease);
        MaxHeap::heapSortIncrease(rawr);
        assert(rawr == rawrSortedIncrease);

        // Проверим конструктор с массивом
        MaxHeap<int> arrayHeap(rawr);
        assert(arrayHeap.getMax() == 21);
        

        // Тест перегруженного оператора присваивания
        MaxHeap<int> heapCopy = arrayHeap;
        assert(heapCopy.getMax() == 21);
        assert(heapCopy.size() == 7);

        // Опа delete !!!!!!!!!!!!!!!!!сделать удаление корня
        heapCopy.deleteAtIndex(1);
        assert(heapCopy.getMax() == 21);

        // А вот и серч
        cout<< heapCopy.search(2) << endl;
        assert(heapCopy.search(2) == 2);
        assert(heapCopy.search(22222) == -1);

        // Опустело
        MaxHeap<int> emptyHeap;
        assert(emptyHeap.isEmpty());
        assert(emptyHeap.size() == 0);
        assert(emptyHeap.search(52222) == -1);
        // Ловись рыбка большая и маленькая
        try
        {
            emptyHeap.getMax();
        }
        catch (const runtime_error &e)
        {
            cerr << "Ошибка перехвачена - " << e.what() << endl;
        }
        try
        {
            emptyHeap.extractMax();
        }
        catch (const runtime_error &e)
        {
            cerr << "Ошибка перехвачена - " << e.what() << std::endl;
        }
        try
        {
            emptyHeap.deleteAtIndex(0);
        }
        catch (const out_of_range &e)
        {
            // Перехват исключения out_of_range
            cerr << "Ошибка перехвачена - " << e.what() << std::endl;
        }

        cout << "Все, расслабьтесь, тесты прошли" << std::endl;
    }
};