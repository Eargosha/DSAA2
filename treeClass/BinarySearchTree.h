// Author: Eargosha

#include <iostream>
#include <iomanip>
#include <vector>
#include <functional>
#include <cassert>
#include <stack>

using namespace std;

/// @brief Класc TreeNodule есть узел дерева
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
};

/// @brief Классик BinarySearchTree есть само бинарное дерево
/// @tparam Type тип бинарного дерева
template <typename Type>
class BinarySearchTree
{

private:
    /// @brief Корень дерева
    TreeNodule<Type> *root;

public:
    /// @brief Конструктор по умолчанию
    BinarySearchTree() : root(nullptr) {};

    /// @brief Конструктор с значением корня
    /// @param value Значение корня
    BinarySearchTree(Type value)
    {
        root = TreeNodule<Type>(value);
    }

    /// @brief Деструктор, производится при помощи вспомогательной функции
    ~BinarySearchTree()
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
    /// @brief Сложность O(log(n)) - Так как проверяем  при этом на каждом шаге мы проверяем, в какую сторону
    /// @brief (левую или правую) необходимо двигаться. Это позволяет найти нужное место для вставки за логарифмическое время
    /// @param value Значение нового узла
    void insert(const Type &value)
    {
        root = insertRecursive(root, value);
    }

    /// @brief Функция для получения значения узла по заданному ключу
    /// @param key Значение ключа для поиска узла
    /// @brief Сложность O(log(n)) - Так как проверяем  при этом на каждом шаге мы проверяем, в какую сторону
    /// @brief (левую или правую) необходимо двигаться. Это позволяет найти нужное место для вставки за логарифмическое время
    /// @return Возвращает найденный узел, если он найден, иначе выбрасывает исключение
    TreeNodule<Type> *getNodeByValue(const Type &key) const
    {
        TreeNodule<Type> *node = getNode(root, key);
        if (node == nullptr)
        {
            throw runtime_error("Узел с заданным ключом не найден");
        }
        return node;
    }

    /// @brief Выводит значение определенного узла. O(1)
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

    /// @brief Метод симметричного обхода дерева - LNR. BigO(n). Сложность O(n) - посещаем каждый узел 1 раз
    /// @return Массив типа vector<Type>
    vector<Type> inorder()
    {
        vector<Type> result;
        inorderRecursive(root, result); // начиная с корня дерева
        return result;
    }

    /// @brief Метод прямого обхода дерева - NRL. BigO(n). Сложность O(n) - посещаем каждый узел 1 раз
    /// @return Массив типа vector<Type>
    vector<Type> preorder()
    {
        vector<Type> result;
        preorderRecursive(root, result); // начиная с корня дерева
        return result;
    }

    /// @brief Метод обратного обхода дерева - RLN. BigO(n). Сложность O(n) - посещаем каждый узел 1 раз
    /// @return Массив типа vector<Type>
    vector<Type> postorder()
    {
        vector<Type> result;
        postorderRecursive(root, result); // начиная с корня дерева
        return result;
    }

    /// @brief Метод для рисования дерева в консольку под +90 градусов, начиная с корня. Сложность O(n) - посещаем каждый узел 1 раз
    void printTree() const
    {
        printTreeRecursive(root, 0);
    }

    /// @brief Метод вывода обхода дерева LNR в консоль. Сложность O(n) - посещаем каждый узел 1 раз
    void printBypassInorderLNR()
    {
        vector<Type> temp;
        temp = inorder();
        cout << "<========== Симметричный обхоД LNR ==========>" << endl;
        for (int i = 0; i < temp.size(); i++)
        {
            cout << temp[i] << ", ";
        }
        cout << endl;
    }

    /// @brief Метод вывода обхода дерева NRL в консоль. Сложность O(n) - посещаем каждый узел 1 раз
    void printBypassPreorderNRL()
    {
        vector<Type> temp;
        temp = preorder();
        cout << "<========== Симметричный обхоД NRL ==========>" << endl;
        for (int i = 0; i < temp.size(); i++)
        {
            cout << temp[i] << ", ";
        }
        cout << endl;
    }

    /// @brief Метод вывода обхода дерева RLN в консоль. Сложность O(n) - посещаем каждый узел 1 раз
    void printBypassPostorderRLN()
    {
        vector<Type> temp;
        temp = postorder();
        cout << "<========== Симметричный обхоД RLN ==========>" << endl;
        for (int i = 0; i < temp.size(); i++)
        {
            cout << temp[i] << ", ";
        }
        cout << endl;
    }

    /// @brief Функция расчета глубины дерева. Сложность O(n) - посещаем каждый узел 1 раз
    /// @return Возращает число int - глубину дерева
    int getTreeDepth() const
    {
        return depthRecursive(root);
    }

    // Чуть чуть помятки
    // симметрического обхода LNR
    // прямого обхода NRL
    // обратного обхода RLN
    /// @brief Функция симметричного(LNR) применения функции к каждому узлу дерева. Сложность O(n) - посещаем каждый узел 1 раз
    /// @param func Сама функция, что применяется к узлу
    void applyInorder(const function<void(Type &)> &func)
    {
        inorderApply(root, func);
    }

    /// @brief Функция прямого(NRL) применения функции к каждому узлу дерева. Сложность O(n) - посещаем каждый узел 1 раз
    /// @param func Сама функция, что применяется к узлу
    void applyPostorder(const function<void(Type &)> &func)
    {
        postorderApply(root, func);
    }

    /// @brief Функция обратного(RLN) применения функции к каждому узлу дерева. Сложность O(n) - посещаем каждый узел 1 раз
    /// @param func Сама функция, что применяется к узлу
    void applyPreorder(const function<void(Type &)> &func)
    {
        preorderApply(root, func);
    }

    /// @brief Функция копирования дерева. Сложность O(n) - посещаем каждый узел 1 раз
    /// @return Возращает скопированное дерево
    BinarySearchTree copy()
    {
        BinarySearchTree tree;
        copyTreeRecursive(root, tree);
        return tree;
    }

    /// @brief Посчитать кол-во узлов в дереве. Сложность O(n) - посещаем каждый узел 1 раз
    /// @return size_t - кол-во узлов
    size_t getTreeNodesCount() const
    {
        return countRecursive(root);
    }

    /// @brief Удаляет узел с даннными data. Сложность: O(log n), в худшем O(n).
    /// @param data Указывает на узел с данными data
    void remove(const Type &data)
    {
        removeNodeRecoursive(root, data);
    }

    /// @brief Функция для удаления дерев. Сложность O(n) - посещаем каждый узел 1 раз
    void deleteTreeNow()
    {
        deleteTree(root);
        root = nullptr;
    }

    ///////////////////Итератор

    /// @brief Класс итератора для BST. Инордерный обход дерева - RNL.
    /// @details Обеспечивает доступ к элементам дерева,
    /// поддерживает операции итерации (перемещение по дереву).
    class Iterator
    {
    private:
        /// Указатель на текущий узел дерева.
        TreeNodule<Type> *currentNode;
        /// Стек для хранения
        stack<TreeNodule<Type> *> nodeStack;

        void pushLeft(TreeNodule<Type> *node)
        {
            while (node != nullptr)
            {
                nodeStack.push(node);
                node = node->noduleLeft;
            }
        }

    public:
        /// Конструктор, инициализирует итератор начальным узлом.
        /// @param root Указатель на узел, с которого начинать итерацию._
        Iterator(TreeNodule<Type> *root) : currentNode(root)
        {
            // Идём в самый левый узел дерева и заполняем стек
            pushLeft(root);
            // Устанавливаем currentNode на верхний элемент стека
            if (root)
            {
                currentNode = nodeStack.top();
                nodeStack.pop();
            }
        }

        /// Возвращает ссылку на данные текущего узла.
        /// @return Ссылка на данные текущего узла._
        Type &operator*() const
        {
            return currentNode->noduleData;
        }

        /// @brief Оператор проверки на неравенство
        /// @param other То с чем сравниваем
        /// @return логическое значене bool_
        bool operator!=(const Iterator &other) const
        {
            return !nodeStack.empty() || !other.nodeStack.empty();
        }

        /// Проверяет, находится ли итератор в конце списка (nullptr).
        /// @return true, если итератор указывает на nullptr, иначе false._
        bool hasNext() const
        {
            return !nodeStack.empty();
        }

        /// Переход к следующему узлу в дереве (по индексу). Инордер - RNL
        /// @return Ссылка на текущий объект итератора._
        Iterator &operator++()
        {
            return next();
        }

        /// @brief Вернуть позицию итератора в начало!_
        void reset()
        {
            nodeStack.empty();
            currentNode = root;
            pushLeft(root);
        }

        /// @brief Переход к следующему элементу
        /// @return Итератор, что будет следущим_
        Iterator &next()
        {
            if (!hasNext())
            {
                throw out_of_range("Итератор вернул значение null");
            }
            currentNode = nodeStack.top();
            nodeStack.pop();
            pushLeft(currentNode->noduleRight);
            return *this;
        }

        /// Возвращает ссылку на данные текущего узла.
        /// @return Ссылка на данные текущего узла._
        Type &data()
        {
            if (currentNode == nullptr)
                throw runtime_error("Итератор вернул значение null");
            return currentNode->noduleData;
        }

        /// Проверяет, совпадают ли два итератора.
        /// @param other Итератор, с которым сравнивается текущий.
        /// @return false, если итераторы указывают на разные узлы, иначе true.
        bool operator==(const Iterator &other) const
        {
            return currentNode == other.currentNode;
        }
    };

    /// Возвращает итератор, указывающий на начало дерева (минимальное значение).
    /// @return Итератор, указывающий на начало дерева.
    Iterator begin() const
    {
        return Iterator(root);
    }

    /// Возвращает итератор, указывающий на конец дерева (nullptr).
    /// @return Итератор, указывающий на конец дерева.
    Iterator end() const { return Iterator(nullptr); }

    /// @brief Тестирование
    static void assertIterator()
    {
        // Создаем простое бинарное дерево
        BinarySearchTree<int> tree;
        tree.insert(4);
        tree.insert(2);
        tree.insert(6);
        tree.insert(1);
        tree.insert(3);
        tree.insert(5);
        tree.insert(7);

        auto it = tree.begin();

        // Проверяем порядок обхода
        assert(it.hasNext());
        assert(*it == 1);
        assert(it != tree.end());
        // Проверка ==
        assert(it == tree.begin());
        ++it; // 2
        assert(it.hasNext());
        assert(*it == 2);
        ++it; // 3
        assert(it.hasNext());
        assert(*it == 3);
        // Проверка data
        assert(it.data() == 3);
        ++it; // 4
        assert(it.hasNext());
        assert(*it == 4);
        ++it; // 5
              // Проверка оператора !=
        assert(it != tree.begin());
        assert(tree.begin() != tree.end());

        assert(it.hasNext());
        assert(*it == 5);
        ++it; // 6
        assert(it.hasNext());
        assert(it != tree.end());
        assert(*it == 6);
        ++it; // 7
        assert(*it == 7);
        assert(!it.hasNext());

        try
        {
            ++it;          // Это должно выбросить исключение
            assert(false); // Если исключение не выброшено, тест провален
        }
        catch (const out_of_range &)
        {
            assert(true); // Исключение выброшено, тест пройден
        }

        // for_each: Выводит все элементы на экран
        for_each(tree.begin(), tree.end(), [](int &element)
                 { cout << (element > 3) << " "; });

        cout << endl;
        // For по коллекции
        for (int value : tree)
        {
            cout << (value < 3) << ' ' << endl;
        }

        // For по коллекции
        for (auto it = tree.begin(); it != tree.end(); ++it)
        {
            cout << (*it == 3) << ' ' << endl;
        }

        cout << endl;
    }

    /// @brief Тестить-тестить
    static void doTest()
    {
        // Тест для обычного дерева
        BinarySearchTree<int> myTree;
        myTree.insert(9);
        myTree.insert(6);
        myTree.insert(14);
        myTree.insert(6);
        myTree.insert(11);
        myTree.insert(19);

        cout << succ(myTree.root);
        assert(myTree.getTreeNodesCount() == 5);
        assert(countRecursive(myTree.getRoot()) == 5);
        assert(myTree.getTreeDepth() == 2);
        assert(depthRecursive(myTree.getRoot()) == 2);
        assert(succ(myTree.root) == 11);

        assert(myTree.getRoot() == myTree.root);
        assert(myTree.getNodeByValue(9) == myTree.getRoot());
        myTree.remove(9);
        assert(myTree.root->noduleData == 11);

        myTree.deleteTreeNow();
        assert(myTree.getTreeNodesCount() == 0);
        assert(myTree.getTreeDepth() == -1);

        myTree.insert(9);
        myTree.insert(2);
        myTree.insert(14);
        myTree.insert(6);
        myTree.insert(11);
        myTree.insert(19);

        // Создание массива на основе обхода
        vector<int> inO = {2, 6, 9, 11, 14, 19};
        vector<int> postO = {19, 11, 14, 6, 2, 9};
        vector<int> preO = {9, 14, 19, 11, 2, 6};
        assert(myTree.inorder() == inO);
        assert(myTree.postorder() == postO);
        assert(myTree.preorder() == preO);

        // Дерево для копии
        BinarySearchTree<int> copyTree;
        BinarySearchTree<int> copyTree2;
        copyTree = myTree.copy();
        copyTree2 = myTree.copy();

        // Применение apply
        myTree.applyInorder([](int& val) { val *= 0; });
        copyTree.applyPostorder([](int& val) { val *= 2; });
        copyTree2.applyPreorder([](int& val) { val *= 2; });

        vector<int> inOAfterApply = {0, 0, 0, 0, 0, 0};
        assert(myTree.inorder() == inOAfterApply);
        vector<int> postOAfterApply = {38, 22, 28, 12, 4, 18};
        assert(copyTree.postorder() == postOAfterApply);
        vector<int> preOAfterApply = {18, 28, 38, 22, 4, 12};
        assert(copyTree2.preorder() == preOAfterApply);

        // Проверим корешки
        assert(myTree.root->noduleData == 0);
        assert(copyTree.root->noduleData == 18);
        assert(copyTree2.root->noduleData == 18);

        myTree.deleteTreeNow();

        // Тест для пустого дерева
        BinarySearchTree<int> pustoBlin;
        assert(pustoBlin.getTreeNodesCount() == 0);
        assert(pustoBlin.getTreeDepth() == -1);
        try
        {
            pustoBlin.remove(10);
        }
        catch (const exception e)
        {
            // Перехват исключения out_of_range
            std::cerr << "Ошибка перехвачена" << e.what() << std::endl;
        }

        pustoBlin.deleteTreeNow();


        // Тест для вырожденного дерева (справа)
        BinarySearchTree<int> goTreeRight;
        goTreeRight.insert(9);
        goTreeRight.insert(14);
        goTreeRight.insert(19);
        assert(succ(goTreeRight.root) == 14);

        assert(goTreeRight.getTreeNodesCount() == 3);
        assert(goTreeRight.getTreeDepth() == 2);

        goTreeRight.deleteTreeNow();
        assert(goTreeRight.getTreeNodesCount() == 0);
        assert(goTreeRight.getTreeDepth() == -1);

        // Тест для вырожденного дерева (слева)
        BinarySearchTree<int> goTreeLeft;
        goTreeLeft.insert(11);
        goTreeLeft.insert(6);
        goTreeLeft.insert(3);
        assert(goTreeLeft.getTreeNodesCount() == 3);
        assert(goTreeLeft.getTreeDepth() == 2);
        assert(succ(goTreeLeft.root) == 6);

        goTreeLeft.deleteTreeNow();
        assert(goTreeLeft.getTreeNodesCount() == 0);
        assert(goTreeLeft.getTreeDepth() == -1);

        // Тест для дерева из одного узла
        BinarySearchTree<int> singleTreeNodule;
        singleTreeNodule.insert(11);
        assert(singleTreeNodule.getTreeNodesCount() == 1);
        assert(singleTreeNodule.getTreeDepth() == 0);
        assert(singleTreeNodule.getRoot()->noduleData == 11);

        singleTreeNodule.deleteTreeNow();
        assert(singleTreeNodule.getTreeNodesCount() == 0);
        assert(singleTreeNodule.getTreeDepth() == -1);

        cout << "<========== Тесты прошлепанЫ ==========>" << endl;
    }
};

/// @brief Вспомогательная функция для удаления дерева. Алгоритм LRN. Сложность O(n)
/// @param node
template <typename Type>
void deleteTree(TreeNodule<Type> *node)
{
    if (node)
    {
        deleteTree(node->noduleLeft);
        deleteTree(node->noduleRight);
        delete node;
    }
}

/// @brief Вспомогательная рекурсивная функция для вставки в дерево бинарного поиска BST
/// @brief Сложность O(log(n)) - Так как проверяем  при этом на каждом шаге мы проверяем, в какую сторону
/// @brief (левую или правую) необходимо двигаться. Это позволяет найти нужное место для вставки за логарифмическое время. В несбалансированном дереве O(n)
/// @param node Узел, куда вставляем
/// @param value Значение нового узла
/// @return Готовый узел
template <typename Type>
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
/// @brief Сложность O(log(n)) - Так как проверяем  при этом на каждом шаге мы проверяем, в какую сторону
/// @brief (левую или правую) необходимо двигаться. Это позволяет найти нужное место для вставки за логарифмическое время. В несбалансированном дереве O(n)
/// @param node Текущий узел для рекурсивного поиска
/// @param key Значение ключа для поиска
/// @return Указатель на найденный узел, или nullptr, если не найден
template <typename Type>
TreeNodule<Type> *getNode(TreeNodule<Type> *node, const Type &key)
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

/// @brief Вспомогательная функция для симметрического обхода LNR. Сложность O(n) - посещаем каждый узел 1 раз
/// @param node Текущий узел
template <typename Type>
void inorderRecursive(TreeNodule<Type> *node, vector<Type> &temp)
{
    if (node != nullptr) // Конец дерева - чек
    {
        // Левый
        inorderRecursive(node->noduleLeft, temp);
        // выводим
        temp.push_back(node->noduleData);
        // Правый
        inorderRecursive(node->noduleRight, temp);
    }
}

/// @brief Вспомогательная функция для прямого обхода NRL. Сложность O(n) - посещаем каждый узел 1 раз
/// @param node Текущий узел
template <typename Type>
void preorderRecursive(TreeNodule<Type> *node, vector<Type> &temp)
{
    if (node != nullptr) // Конец дерева - чек
    {
        // выводим
        temp.push_back(node->noduleData);
        // Правый
        preorderRecursive(node->noduleRight, temp);
        // Левый
        preorderRecursive(node->noduleLeft, temp);
    }
}

/// @brief Вспомогательная функция для обратного обхода RLN. Сложность O(n) - посещаем каждый узел 1 раз
/// @param node Текущий узел
template <typename Type>
void postorderRecursive(TreeNodule<Type> *node, vector<Type> &temp)
{
    if (node != nullptr) // Конец дерева - чек
    {
        // Правый
        postorderRecursive(node->noduleRight, temp);
        // Левый
        postorderRecursive(node->noduleLeft, temp);
        // выводим
        temp.push_back(node->noduleData);
    }
}

/// @brief Вспомогательная функция для рисования дерева в консоль. Сложность O(n) - посещаем каждый узел 1 раз
/// @param node Текущий узел дерева
/// @param level Уровень рисования
template <typename Type>
void printTreeRecursive(TreeNodule<Type> *node, int level)
{
    if (node == nullptr)
    {
        return;
    }

    // Вывод правого поддерева
    printTreeRecursive(node->noduleRight, level + 1);

    // Вывод текущего узла
    cout << setw(4 * level) << "" << "===" << node->noduleData << endl;

    // Вывод левого поддерева
    printTreeRecursive(node->noduleLeft, level + 1);
}

/// @brief Рекурсивная функция симметричного(LNR) применения функции к каждому узлу дерева. Сложность O(n) - посещаем каждый узел 1 раз
/// @tparam Type - тип узла
/// @param node Узел, с которым работаем
/// @param func Функция, которую применяем к узлу
template <typename Type>
void inorderApply(TreeNodule<Type> *node, const function<void(Type &)> &func)
{
    if (node != nullptr)
    {
        inorderApply(node->noduleLeft, func);
        func(node->noduleData);
        inorderApply(node->noduleRight, func);
    }
}

/// @brief Рекурсивная функция прямого(NRL) применения функции к каждому узлу дерева. Сложность O(n) - посещаем каждый узел 1 раз
/// @tparam Type - тип узла
/// @param node Узел, с которым работаем
/// @param func Функция, которую применяем к узлу
template <typename Type>
void postorderApply(TreeNodule<Type> *node, const function<void(Type &)> &func)
{
    if (node != nullptr)
    {
        postorderApply(node->noduleLeft, func);
        postorderApply(node->noduleRight, func);
        func(node->noduleData);
    }
}

/// @brief Рекурсивная функция обратного(RLN) применения функции к каждому узлу дерева. Сложность O(n) - посещаем каждый узел 1 раз
/// @tparam Type - тип узла
/// @param node Узел, с которым работаем
/// @param func Функция, которую применяем к узлу
template <typename Type>
void preorderApply(TreeNodule<Type> *node, const function<void(Type &)> &func)
{
    if (node != nullptr)
    {
        func(node->noduleData);
        preorderApply(node->noduleLeft, func);
        preorderApply(node->noduleRight, func);
    }
}

/// @brief Рекурсивная функция для подсчета кол-во узлов в дереве. RLN. Сложность O(n) - посещаем каждый узел 1 раз
/// @tparam Type - тип дерева
/// @param node Текущий узел
/// @return size_t - кол-во узлов в дереве, 0 если нет узлов в дереве
template <typename Type>
size_t countRecursive(const TreeNodule<Type> *node)
{
    if (node != nullptr)
    {
        int rightCount = countRecursive(node->noduleRight);
        int leftCount = countRecursive(node->noduleLeft);
        return 1 + leftCount + rightCount;
    }
    else
    {
        return 0;
    }
}

/// @brief Рекурсивная функция расчета грубины дерева. RLN. Сложность O(n) - посещаем каждый узел 1 раз
/// @tparam Type - тип дерева
/// @param node - Текущий узел
/// @return Возращает int - глубина дерева, -1 если дерево пустое
template <typename Type>
int depthRecursive(const TreeNodule<Type> *node)
{
    if (node != nullptr)
    {
        int rightDepth = depthRecursive(node->noduleRight);
        int leftDepth = depthRecursive(node->noduleLeft);
        return 1 + max(leftDepth, rightDepth);
    }
    else
    {
        return -1;
    }
}

/// @brief Рекурсивная функция для копирования дерева NRL. Сложность O(n) - посещаем каждый узел 1 раз
/// @tparam Type - тип дерева
/// @param node Текщий узел копирования
/// @param tree Дерево, что, куда копируем
template <typename Type>
void copyTreeRecursive(TreeNodule<Type> *node, BinarySearchTree<Type> &tree)
{
    if (node != nullptr)
    {
        tree.insert(node->noduleData);
        copyTreeRecursive(node->noduleRight, tree);
        copyTreeRecursive(node->noduleLeft, tree);
    }
}

/// @brief Рекурсивная функция удаления узла. Сложность: O(log n), в худшем O(n).
/// @tparam Type - тип узла
/// @param node Текущий узел
/// @param data Значение узла, которого ищем
/// @return Узел-замену на удаленный узел
template <typename Type>
TreeNodule<Type> *removeNodeRecoursive(TreeNodule<Type> *node, const Type &data)
{
    // Если пусто, то пусто
    if (!node)
    {
        return nullptr;
    }

    // Проверяем на если значение меньше, чем значение в текущем узле, идем влево
    if (data < node->noduleData)
    {
        node->noduleLeft = removeNodeRecoursive(node->noduleLeft, data);
    }
    // Проверяем на если значение больше, чем значение в текущем узле, идем вправо
    else if (data > node->noduleData)
    {
        node->noduleRight = removeNodeRecoursive(node->noduleRight, data);
    }

    else
    {
        // ну вот нашли что удалять
        // если узел бездетный, жестоко зарезаем его
        if (!node->noduleLeft && !node->noduleRight)
        {
            delete node;
            return nullptr;
        }

        // если у узла есть правый ребенок, оставляем его одного, на месте родителя, без самого родителя
        else if (!node->noduleRight)
        {
            TreeNodule<Type> *temp = node->noduleLeft;
            delete node;
            return temp;
        }

        // если у узла есть левый ребенок, оставляем его одного, на месте родителя, без самого родителя
        else if (!node->noduleLeft)
        {
            TreeNodule<Type> *temp = node->noduleRight;
            delete node;
            return temp;
        }

        // ну а если он многодетный, то выбираем наиболее подходящего ребенка и меняем местами с родителем
        else
        {
            TreeNodule<Type> *successor = succ(node);
            node->noduleData = successor->noduleData;
            node->noduleRight = removeNodeRecoursive(node->noduleRight, successor->noduleData);
        }
    }

    // Мафия сделала свое дело
    return node;
}

/**
 * @brief Находит следующий узел в порядке in-order обхода дерева.
 *
 * Этот метод возвращает следующий узел в порядке in-order обхода
 * для заданного узла. Если узел не имеет правого потомка, или
 * если правый потомок является листовым узлом, этот метод
 * возвращает nullptr.
 *
 * Сложность O(n) - посещаем каждый узел 1 раз (в несбалансированном)
 * Сложность O(Log(n)) - в сбалансированном
 *
 * @param node Узел, для которого нужно найти следующий узел в порядке in-order обхода.
 * @return Указатель на следующий узел в порядке in-order обхода, или nullptr, если
 * следующий узел отсутствует.
 */
template <typename Type>
TreeNodule<Type> *succ(TreeNodule<Type> *node)
{
    // Если узел равен nullptr или не имеет правого потомка, вернуть nullptr
    if (node == nullptr || node->noduleRight == nullptr)
    {
        return nullptr;
    }
    // Начать с правого потомка узла
    TreeNodule<Type> *current = node->noduleRight;
    // Перемещаться влево, пока не будет найден самый левый узел
    while (current->noduleLeft != nullptr)
    {
        current = current->noduleLeft;
    }
    // Вернуть самый левый узел (следующий узел в порядке in-order обхода)
    return current;
}
