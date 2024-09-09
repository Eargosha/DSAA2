// Author: Eargosha

#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

/// @brief Классик TreeNodule есть узелок двусвязного списка
/// @tparam Type тип узла дерева
template <typename Type>
class TreeNodule
{

public:
    /// @brief Данные, что хранит в себе узелок дерева
    Type noduleData;

    /// @brief Указатель на следующий(правый) узел в списке
    TreeNodule<Type> *noduleRight;

    /// @brief Указатель на предыдущий(левый) узел в списке
    TreeNodule<Type> *noduleLeft;

    /// @brief Конструктор по умолчанию
    TreeNodule() : noduleData(Type()), noduleRight(nullptr), noduleLeft(nullptr) {}

    /// @brief Конструктор с данными и указателями
    /// @param data Данные узла
    /// @param right Указатель на следующий(правый) узел
    /// @param left Указатель на предыдущий(левый) узел
    TreeNodule(const Type &data, TreeNodule<Type> *right, TreeNodule<Type> *left) : noduleData(data), noduleRight(right), noduleLeft(left) {}

    /// @brief Конструктор только с данными
    /// @param data Данные узла
    TreeNodule(const Type &data) : noduleData(data), noduleRight(nullptr), noduleLeft(nullptr) {}

    /// @brief Деструктор
    ~TreeNodule()
    {
        // Проверяем, что указатели не являются nullptr, прежде чем удалять
        if (noduleLeft != nullptr)
            noduleLeft = nullptr;
        if (noduleRight != nullptr)
            noduleRight = nullptr;
    }

    /// @brief Конструктор копирования
    /// @param other Узел который копируем
    TreeNodule(const TreeNodule &other) : noduleData(other.noduleData), noduleRight(nullptr), noduleLeft(nullptr)
    {
        // Копируем указатели, если те есть
        // Копируем указатели, если те есть
        if (other.noduleRight != nullptr)
        {
            noduleRight = new TreeNodule(other.noduleRight->noduleData);
        }

        if (other.noduleLeft != nullptr)
        {
            noduleLeft = new TreeNodule(other.noduleLeft->noduleData);
        }

        other.noduleRight->noduleData;
        // Обращается к члену данных noduleData в узле, на который указывает other.noduleRight.
        // Таким образом, мы получаем значение данных, которое содержится в правом дочернем узле исходного объекта other.

        // Это часть процесса копирования правого дочернего узла исходного объекта в новый объект,
        // чтобы создать точную копию структуры дерева.
    };

    /// @brief Оператор присваивания копированием
    /// @param other Другой узел, что нужно скопировать
    /// @return Результат копирования
    TreeNodule &operator=(const TreeNodule &other)
    {
        // Равен ли себе самому
        if (this != &other)
        {
            // Копируем данные
            noduleData = other.noduleData;

            // Убираем текущие указатели на след. и пред. узлы
            delete noduleRight;
            delete noduleLeft;

            // (other.noduleRight) ? - условие, что проверяет является ли other.noduleRight не равным nullptr
            noduleRight = (other.noduleRight) ? new TreeNodule(other.noduleRight->noduleData) : nullptr;
            noduleLeft = (other.noduleLeft) ? new TreeNodule(other.noduleLeft->noduleData) : nullptr;
        }
        return *this;
    }

    /// @brief Конструктор перемещения
    /// @param other То, что нужно переместить
    TreeNodule(TreeNodule &&other) : noduleData(other.noduleData), noduleRight(other.noduleRight), noduleLeft(other.noduleLeft)
    {
        // Забываем о перемещенном
        other.noduleRight = nullptr;
        other.noduleLeft = nullptr;
    }

    /// @brief Оператор присваивания перемещением
    /// @param other То что перемещаем
    /// @return Результат перемещения
    TreeNodule &operator=(TreeNodule &&other)
    {
        // Равен ли сам себе
        if (this != &other)
        {
            // Освобождение ресурсов
            delete noduleRight;
            delete noduleLeft;

            // Перемещение данных и указателей из другого
            noduleData = other.noduleData;
            noduleRight = other.noduleRight;
            noduleLeft = other.noduleLeft;

            // Забываем о перемещенном
            other.noduleRight = nullptr;
            other.noduleLeft = nullptr;
        }
        return *this;
    }
};

/// @brief Классик TreeNodule есть узелок двусвязного списка
/// @tparam Type тип бинарного дерева
template <typename Type>
class BinaryTree
{

private:
    /// @brief Корень дерева
    TreeNodule<Type> *root;

    /// @brief Вспомогательная функция для удаления дерева
    /// @param node
    void deleteTree(TreeNodule<Type> *node)
    {
        if (node)
        {
            deleteTree(node->noduleLeft);
            deleteTree(node->noduleRight);
            delete node;
        }
    }

    /// @brief Вспомогательная рекурсивная функция для вставки
    /// @param node Узел, куда вставляем
    /// @param value Значение нового узла
    /// @return Готовый узел
    TreeNodule<Type> *insertRecursive(TreeNodule<Type> *node, const Type &value)
    {
        // Если текущий узел равен nullptr, создаем новый узел с заданным значением
        if (node == nullptr)
        {
            return new TreeNodule<Type>(value);
        }

        // Если значение меньше, чем значение в текущем узле, вставляем в левое поддерево
        if (value < node->noduleData)
        {
            node->noduleLeft = insertRecursive(node->noduleLeft, value);
        }
        // Если значение больше, чем значение в текущем узле, вставляем в правое поддерево
        else if (value > node->noduleData)
        {
            node->noduleRight = insertRecursive(node->noduleRight, value);
        }
        // Если значение равно значению в текущем узле, ничего не делаем
        else
        {
            return node;
        }

        return node;
    }

    /// @brief Вспомогательная функция для поиска узла по ключу
    /// @param node Текущий узел для рекурсивного поиска
    /// @param key Значение ключа для поиска
    /// @return Указатель на найденный узел, или nullptr, если не найден
    TreeNodule<Type> *getNode(TreeNodule<Type> *node, const Type &key) const
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (key == node->noduleData)
        {
            return node;
        }

        if (key < node->noduleData)
        {
            return getNode(node->noduleLeft, key);
        }
        else
        {
            return getNode(node->noduleRight, key);
        }
    }

    /// @brief Вспомогательная функция для инордерного обхода
    /// @param node Текущий узел
    void inorderRecursive(TreeNodule<Type> *node)
    {
        if (node != nullptr) // Конец дерева - чек
        {
            // Левый
            inorderRecursive(node->noduleLeft);
            cout << node->noduleData << " "; // выводим
            // Правый
            inorderRecursive(node->noduleRight);
        }
    }

    /// @brief Вспомогательная функция для рисования дерева в консоль
    /// @param node Текущий узел дерева
    /// @param level Уровень рисования
    void printTreeRecursive(TreeNodule<Type> *node, int level) const
    {
        if (node == nullptr)
        {
            return;
        }

        // Вывод правого поддерева
        printTreeRecursive(node->noduleRight, level + 1);

        // Вывод текущего узла
        cout << setw(4 * level) << "" << "┌─" << node->noduleData << endl;

        // Вывод левого поддерева
        printTreeRecursive(node->noduleLeft, level + 1);
    }

public:
    /// @brief Конструктор по умолчанию
    BinaryTree() : root(nullptr) {};

    /// @brief Конструктор с значением корня
    /// @param value Значение корня
    BinaryTree(Type value)
    {
        root = TreeNodule(value);
    }

    /// @brief Деструктор, производится при помощи вспомогательной функции
    ~BinaryTree()
    {
        deleteTree(root);
    }

    /// @brief Геттер корня дерева
    /// @return Возращает TreeNodule корня дерева
    TreeNodule<Type> *getRoot() const
    {
        return root;
    }

    /// @brief Функция вставки нового узла в дерево
    /// @param value Значение нового узла
    void insert(const Type &value)
    {
        root = insertRecursive(root, value);
    }

    /// @brief Функция для получения значения узла по заданному ключу
    /// @param key Значение ключа для поиска узла
    /// @return Возвращает найденный узел, если он найден, иначе выбрасывает исключение
    TreeNodule<Type> *getNodeByValue(const Type &key) const
    {
        TreeNodule<Type> *node = getNode(root, key);
        if (node == nullptr)
        {
            throw std::runtime_error("Узел с заданным ключом не найден");
        }
        return node;
    }

    /// @brief Выводит значение определенного узла
    /// @param node Узел, значение которого выведется
    void printNode(TreeNodule<Type> *node) const
    {
        if (node)
        {
            cout << node->noduleData << endl;
        }
        else
        {
            cout << "Пусто" << endl;
        }
    }

    /// @brief Метод инфиксного обхода дерева,
    /// при котором обрабатывается сначала Left -> Near -> Right
    void inorder()
    {
        inorderRecursive(root); // начиная с корня дерева
        cout << endl;
    }

    /// @brief Метод для рисования дерева в консольку, начиная с корня
    void printTree() const
    {
        printTreeRecursive(root, 0);
    }
};