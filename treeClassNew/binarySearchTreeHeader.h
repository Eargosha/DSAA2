#pragma once
// Done by Eargosha
#include "binarySearchTree.h"
#include <vector>

// АВЛ - дерево — сбалансированное по высоте двоичное дерево поиска : для каждой его вершины высота 
// её двух поддеревьев различается не более чем на 1.

// Баланс = hright - hleft, результат может быть:
//                                              + перевес вправо
//                                              - перевес влево
// Он несет в себе значения: 
// - -1: Левое поддерево выше (высота левого поддерева больше).
// - 0: Поддеревья одинаковой высоты.
// - 1: Правое поддерево выше(высота правого поддерева больше).

// Этот ваш баланс вычисляется для каждого узла вдоль пути поиска после каждой операции 
// изменяющей дерево (вставки или удаления)

template <typename T>
// Класс AVLTreeNodule, узел AVLTree, имеет дополнительное поле - коэффициент баланса
// Наследуется от TreeNodule.
class AVLTreeNodule : public TreeNodule<T> {
public:
    // Коэффициент баланса узла.
    short int balanceFactor;

    // Конструктор по умолчанию.
    AVLTreeNodule() : TreeNodule<T>(), balanceFactor(0) {}

    // Конструктор, принимающий данные.
    AVLTreeNodule(const T& data) : TreeNodule<T>(data), balanceFactor(0) {}

    // Конструктор, принимающий данные и указатели на предыдущий и следующий узлы.
    AVLTreeNodule(const T& data, TreeNodule<T>* getLeft(), TreeNodule<T>* getRight()) : TreeNodule<T>(data, getLeft(), getRight()), balanceFactor(0) {}

    // Преобразование указателя на левый TreeNodule в указатель на AVLTreeNodule
    AVLTreeNodule<T>* getLeft() {
        return static_cast<AVLTreeNodule<T>*>(this->noduleLeft);
    }

    // Преобразование указателя на правый TreeNodule в указатель на AVLTreeNodule
    AVLTreeNodule<T>* getRight() {
        return static_cast<AVLTreeNodule<T>*>(this->noduleRight);
    }
};

// AVLTree является сбалансированным бинарным деревом поиска.
template<typename T>
class AVLTree {
private:
    /// <summary>
    /// Корень дерева
    /// </summary>
    AVLTreeNodule<T>* root;

    /// <summary>
    /// Функция для обновления баланс фактора дерева
    /// Сложность O(log2(n))
    /// </summary>
    /// <param name="node">Узел, у которого обновляем баланс фактор</param>
    void renewBalanceFactor(AVLTreeNodule<T>* node) {
        if (node == nullptr) {
            return;
        }

        int hLeft = getHeight(node->getLeft());
        int hRight = getHeight(node->getRight());
        node->balanceFactor = hLeft - hRight;
    }

    /// <summary>
    /// Функция для получения высоты узла. Высота считается по дугам
    /// Сложность O(log2(n))
    /// </summary>
    /// <param name="node">Узел, у которого получаем высоту</param>
    /// <returns>Высоту узла</returns>
    int getHeight(AVLTreeNodule<T>* node) {
        if (node == nullptr) {
            return 0;
        }

        return 1 + std::max(getHeight(node->getLeft()), getHeight(node->getRight()));
    }

    /// <summary>
    /// Выполняет правый поворот относительно заданного узла AVL-дерева.
    /// Сложность: O(1)
    /// </summary>
    /// <param name="node">Узел, вокруг которого выполняется правый поворот.
    /// Это узел, который станет правым поддеревом после выполнения операции.</param>
    /// <returns>
    /// Возвращает новый корень поддерева, ставший результатом выполнения
    /// правого поворота. Этот узел будет заменять узел, переданный в параметре.
    /// </returns>
    AVLTreeNodule<T>* rotateRight(AVLTreeNodule<T>* node) {
        // Сохраняем указатель на левое поддерево
        AVLTreeNodule<T>* temp = node->getLeft();

        // Перемещаем правое поддерево левого узла вниз
        node->n_left = temp->getRight();

        // Устанавливаем новый корень поддерева
        temp->n_right = node;

        // Обновляем балансировочные факторы для обоих узлов
        renewBalanceFactor(node);
        renewBalanceFactor(temp);

        // Возвращаем новый корень поддерева
        return temp;
    }

    /// <summary>
    /// Выполняет левый поворот относительно заданного узла AVL-дерева.
    /// Сложность: O(1)
    /// </summary>
    /// <param name="node">Узел, вокруг которого выполняется левый поворот.
    /// Этот узел станет левым поддеревом нового корня.</param>
    /// <returns>
    /// Возвращает новый корень поддерева, ставший результатом выполнения
    /// левого поворота. Этот узел будет заменять узел, переданный в параметре.
    /// </returns>
    AVLTreeNodule<T>* rotateLeft(AVLTreeNodule<T>* node) {
        // Сохраняем указатель на правое поддерево
        AVLTreeNodule<T>* temp = node->getRight();

        // Перемещаем левое поддерево правого узла вниз
        node->n_right = temp->getLeft();

        // Устанавливаем новый корень поддерева
        temp->n_left = node;

        // Обновляем балансировочные факторы для обоих узлов
        renewBalanceFactor(node);
        renewBalanceFactor(temp);

        // Возвращаем новый корень поддерева
        return temp;
    }

    /// <summary>
    /// Балансирует узел AVL-дерева, восстанавливая его сбалансированное состояние,
    /// если это необходимо.
    /// Сложность O(log2(n))
    /// <param name="node">Узел, который необходимо сбалансировать. Может быть равен nullptr.</param>
    /// <returns>
    /// Возвращает сбалансированный узел. Если изначально переданный узел равен nullptr,
    /// возвращается nullptr.
    /// </returns>
    AVLTreeNodule<T>* balanceTree(AVLTreeNodule<T>* node) {
        // Проверяем, является ли узел пустым
        if (node == nullptr) {
            return nullptr;
        }

        // Обновляем балансировочный фактор текущего узла
        renewBalanceFactor(node);

        // Проверяем, вызывает ли левое поддерево дисбаланс
        if (node->balanceFactor > 1) {
            // Определяем, какой тип поворота требуется
            // Если левое поддерево левого узла выше или равно правому поддереву этого же узла
            // То один правый
            if (getHeight(node->getLeft()->getLeft()) >= getHeight(node->getLeft()->getRight())) {
                // Одноразовый правый поворот
                node = rotateRight(node);
            }
            // Если высота правого поддерева левого узла больше, то это требует 
            // двойного поворота
            else {
                // Двойной правый поворот'
                // Сначала выполняем левый поворот на левом узле
                node->n_left = rotateLeft(node->getLeft());
                // А затем правый поворот на текущем узле, чтобы поддерживать баланс
                node = rotateRight(node);
            }
        }
        // Проверяем, вызывает ли правое поддерево дисбаланс
        else if (node->balanceFactor < -1) {
            // Определяем, какой тип поворота требуется
            // Если правое поддерево правого узла выше или равно левому поддереву
            // То одинарно вправо
            if (getHeight(node->getRight()->getRight()) >= getHeight(node->getRight()->getLeft())) {
                // Одноразовый левый поворот
                node = rotateLeft(node);
            }
            // Если высота левого поддерева правого узла больше, то это также требует 
            // двойного поворота
            else {
                // Двойной левый поворот
                // Сначала выполняем правый поворот на правом узле
                node->n_right = rotateRight(node->getRight());
                // Затем левый поворот на текущем узле, чтобы вернуть баланс
                node = rotateLeft(node);
            }
        }

        // Возвращаем сбалансированный узел
        return node;
    }

    /// <summary>
    /// Функция для вставки узла в дерево. Если узел с таким значением уже существует,
    /// вставка не производится, и дерево остается неизменным.
    /// Сложность: O(log2(n))
    /// </summary>
    /// <param name="node">Узел, куда будет производиться вставка.
    /// Если данное значение nullptr, будет создан новый узел.</param>
    /// <param name="data">Данные, которые необходимо вставить в дерево.</param>
    /// <returns>
    /// Возвращает указатель на новый узел, либо на существующий узел после 
    /// вставки, при этом дерево остается сбалансированным.
    /// </returns>
    AVLTreeNodule<T>* insertNode(AVLTreeNodule<T>* node, const T& data) {
        // Если текущий узел пустой, создаем новый узел с переданными данными
        if (node == nullptr) {
            return new AVLTreeNodule<T>(data);
        }

        // Если данные меньше, чем данные текущего узла,
        // продолжаем вставку в левое поддерево
        if (data < node->noduleData) {
            node->noduleLeft = insertNode(node->getLeft(), data);
        }
        // Если данные больше, чем данные текущего узла,
        // продолжаем вставку в правое поддерево
        else if (data > node->noduleData) {
            node->noduleRight = insertNode(node->getRight(), data);
        }

        // После вставки обеспечиваем сбалансированность дерева
        return balanceTree(node);
    }

    /// <summary>
    /// Функция для удаления узла из дерева. Если узел с указанным значением не найден,
    /// Сложность: O(log2(n))
    /// дерево остается неизменным.
    /// </summary>
    /// <param name="node">Узел, из которого будет производиться удаление.
    /// Если данное значение nullptr, функция возвращает nullptr, что сигнализирует о том,
    /// что удаление не может быть произведено.</param>
    /// <param name="data">Данные, которые необходимо удалить из дерева.</param>
    /// <returns>
    /// Возвращает указатель на новый корень поддерева после удаления узла,
    /// при этом дерево остается сбалансированным.
    /// </returns>
    AVLTreeNodule<T>* deleteNode(AVLTreeNodule<T>* node, const T& data) {
        // Если текущий узел пустой, возвращаем nullptr
        if (node == nullptr) {
            return nullptr;
        }

        // Если данные меньше, чем данные текущего узла,
        // продолжаем удаление в левом поддереве
        if (data < node->noduleData) {
            node->noduleLeft = deleteNode(node->getLeft(), data);
        }
        // Если данные больше, чем данные текущего узла,
        // продолжаем удаление в правом поддереве
        else if (data > node->noduleData) {
            node->noduleRight = deleteNode(node->getRight(), data);
        }
        // Если данные равны, значит мы нашли узел для удаления
        else {
            // Случай, когда у узла нет левого поддерева
            if (node->getLeft() == nullptr) {
                AVLTreeNodule<T>* temp = node->getRight();
                delete node; // Удаляем узел
                return temp; // Возвращаем правое поддерево
            }
            // Случай, когда у узла нет правого поддерева
            else if (node->getRight() == nullptr) {
                AVLTreeNodule<T>* temp = node->getLeft();
                delete node; // Удаляем узел
                return temp; // Возвращаем левое поддерево
            }

            // Случай, когда у узла есть оба поддерева
            AVLTreeNodule<T>* temp = node->getRight();
            // Находим минимальный узел в правом поддереве (наиболее левый узел)
            while (temp->getLeft() != nullptr) {
                temp = temp->getLeft();
            }

            // Копируем данные из найденного узла в текущий узел
            node->noduleData = temp->noduleData;
            // Удаляем узел с минимальными данными из правого поддерева
            node->noduleRight = deleteNode(node->getRight(), temp->noduleData);
        }

        // После удаления обеспечиваем сбалансированность дерева
        return balanceTree(node);
    }


public:
    // Конструктор по умолчанию.
    AVLTree() : root(nullptr) {}

    // Деструктор.
    ~AVLTree() {
        clear();
    }

    /// <summary>
    /// Функция для вставки элемента в дерево
    /// </summary>
    /// <param name="data">Что нужно вставить</param>
    void insert(const T& data) {
        root = insertNode(root, data);
    }

    /// <summary>
    /// Функция для удаления элемента из дерева
    /// </summary>
    /// <param name="data"></param>
    void remove(const T& data) {
        root = deleteNode(root, data);
    }

    /// <summary>
    /// Метод для вывода дерева в виде вектора.
    /// Создаёт вектор, содержащий значения узлов дерева в порядке обхода LNR (in-order)
    /// </summary>
    /// <returns>vector в LNR</returns>
    std::vector<T> toVector() {
        std::vector<T> result; // Вектор для хранения значений узлов дерева.
        toVectorHelper(root, result); // Заполняем вектор с помощью вспомогательного метода.
        return result; // Возвращаем заполненный вектор.
    }

    /// <summary>
    /// Метод поиска узла в дереве по его данным
    /// Использует рекурсивный подход через вспомогательную функцию findReqursive
    /// </summary>
    /// <param name="data">Данные, по которым ищем узел</param>
    /// <returns>AVLTreeNodule - узел дерева</returns>
    AVLTreeNodule<T>* findIt(const T& data) {
        return findReqursive(root, data);
    }

    /// <summary>
    /// Метод очистки дерева
    /// </summary>
    void clear() {
        if (root)
        {
            clearNode(root);
            root = nullptr;
        }
    }

    /// <summary>
    /// Метод для получения баланса узла.
    /// Если узел == null, возвращает 0.
    /// </summary>
    /// <param name="node">Узел, с которого требуется баланс фактор.</param>
    /// <returns>
    /// Возвращает значение баланс фактора для node
    /// </returns> 
    int getBalanceFactor(AVLTreeNodule<T>* node) {
        if (node == nullptr) {
            return 0;
        }

        return node->balanceFactor;
    }

    /// <summary>
    /// Метод для получения коэффициента баланса узла по заданному значению.
    /// Если узел с указанным значением найден, возвращает его коэффициент баланса.
    /// В противном случае выбрасывает исключение out_of_range 
    /// с сообщением о том, что значение не найдено.
    /// </summary>
    /// <param name="val">Значение, по которому мы ищем узел в дереве.</param>
    /// <returns>
    /// Возвращает коэффициент баланса узла, соответствующего переданному значению.
    /// </returns>
    /// <exception>
    /// Вызывает исключение out_of_range, если узел с указанным значением не найден.
    /// </exception>
    int getBalanceFactorByValue(T val) {
        AVLTreeNodule<T>* node = findIt(val);
        if (node)
        {
            return getBalanceFactor(node);
        }
        else
            throw out_of_range("Value not found");
    }



    /// <summary>
    /// Класс Итератора для обхода узлов AVL-дерева по порядку LNR (in-order).
    /// Итератор позволяет последовательно получать значения всех узлов
    /// дерева в отсортированном порядке.
    /// </summary>
    class Iterator {
    private:
        AVLTreeNodule<T>* root; // Корень дерева, с которого начинается итерация
        stack<AVLTreeNodule<T>*> nodeStack; // Стек для хранения узлов для обхода

    public:
        /// <summary>
        /// Конструктор итератора.
        /// Инициализирует итератор, устанавливая начальный узел и заполняя стек
        /// левой веткой дерева, начиная с корня.
        /// </summary>
        /// <param name="node">Указатель на начальный узел для итерации.</param>
        Iterator(AVLTreeNodule<T>* node) {
            root = node;
            pushLeftBranch(node);
        }

        /// <summary>
        /// Оператор проверки на неравенство.
        /// Сравнивает два итератора и возвращает true, если они не равны.
        /// Итераторы считаются не равными, если хотя бы один из них
        /// имеет следующий элемент.
        /// </summary>
        /// <param name="other">Другой итератор для сравнения.</param>
        /// <returns>Возвращает true, если итераторы не равны; иначе false.</returns>
        bool operator!=(const Iterator& other) const {
            return !(hasNext() == false && other.hasNext() == false);
        }

        /// <summary>
        /// Оператор проверки на равенство.
        /// Сравнивает два итератора и возвращает true, если они равны.
        /// Итераторы считаются равными, если оба они либо имеют,
        /// либо не имеют следующего элемента.
        /// </summary>
        /// <param name="other">Другой итератор для сравнения.</param>
        /// <returns>Возвращает true, если итераторы равны; иначе false.</returns>
        bool operator==(const Iterator& other) const {
            return nodeStack.empty() == other.nodeStack.empty();
        }

        /// <summary>
        /// Проверка наличия следующего элемента.
        /// Возвращает true, если стек не пуст, что означает, что
        /// есть узлы для итерации, иначе возвращает false.
        /// </summary>
        /// <returns>Возвращает true, если есть следующий элемент; иначе false.</returns>
        bool hasNext() const {
            return !nodeStack.empty();
        }

        /// <summary>
        /// Оператор разыменования.
        /// Возвращает ссылку на данные текущего верхнего узла в стеке.
        /// </summary>
        /// <returns>Ссылка на данные текущего узла типа T.</returns>
        T& operator*() const {
            return nodeStack.top()->noduleData;
        }

        /// <summary>
        /// Получение информации о текущем узле.
        /// Возвращает ссылку на данные текущего узла
        /// не изменяя его состояния.
        /// </summary>
        /// <returns>Ссылка на данные текущего узла типа T.</returns>
        T& data() {
            return nodeStack.top()->noduleData;
        }

        /// <summary>
        /// Оператор инкремента.
        /// Перемещает итератор к следующему элементу
        /// и возвращает ссылку на текущий итератор.
        /// </summary>
        /// <returns>Ссылку на текущий итератор.</returns>
        Iterator& operator++() {
            return next();
        }

        /// <summary>
        /// Сбрасывает итератор.
        /// Очищает стек и заполняет его, начиная с левой ветви
        /// корневого узла, как будто итератор инициализировался заново.
        /// </summary>
        void reset() {
            while (!nodeStack.empty())
                nodeStack.pop();
            pushLeftBranch(root);
        }

        /// <summary>
        /// Переходит к следующему элементу.
        /// Обновляет стек, чтобы указать на следующий узел
        /// в порядке обхода. Выбрасывает исключение, если больше /// нет элементов для итерации.
        /// </summary>
        /// <returns>Ссылку на текущий итератор для цепочного вызова.</returns>
        /// <exception>Выбрасывает std::out_of_range, если больше нет элементов.</exception>
        Iterator& next() {
            AVLTreeNodule<T>* currentNode;
            if (!hasNext()) {
                throw std::out_of_range("No more elements in the iterator");
            }
            currentNode = nodeStack.top();
            nodeStack.pop();
            pushLeftBranch(currentNode->getRight());
            return *this;
        }

    private:
        /// <summary>
        /// Метод помещает все левые узлы узла node в nodeStack.
        /// Этот метод используется для инициализации и обновления стека
        /// перед доступом к узлам в порядке обхода LNR.
        /// </summary>
        /// <param name="node">Указатель на узел, с которого начинается добавление в стек.</param>
        void pushLeftBranch(AVLTreeNodule<T>* node) {
            while (node != nullptr) {
                nodeStack.push(node);
                node = node->getLeft();
            }
        }
    };

    /// <summary>
    /// Возращает итератор на начало дерева
    /// </summary>
    /// <returns>Итератор</returns>
    Iterator begin() const {
        return Iterator(root);
    }

    /// <summary>
    /// Ставит итератор на конец дерева
    /// </summary>
    /// <returns></returns>
    Iterator end() const {
        return Iterator(nullptr);
    }

    //// тестирование
    //static void AVLTreeRunTest() {
    //    AVLTree<int> tree;

    //    // Тестирование вставки в пустое дерево
    //    tree.insert(10);
    //    assert(tree.findIt(10) != nullptr); // Должен найти 10
    //    assert(tree.findIt(20) == nullptr); // Не должен найти 20

    //    // Тестирование вставки нескольких элементов
    //    tree.insert(20);
    //    tree.insert(5);
    //    tree.insert(15);
    //    tree.printTree();
    //    assert(tree.findIt(5) != nullptr);  // Должен найти 5
    //    assert(tree.findIt(15) != nullptr); // Должен найти 15
    //    assert(tree.findIt(25) == nullptr); // Не должен найти 25

    //    // Тестирование удаления элемента
    //    tree.remove(20);
    //    assert(tree.findIt(20) == nullptr); // 20 должен быть удален

    //    // Тестирование удаления корня
    //    tree.remove(10);
    //    assert(tree.findIt(10) == nullptr); // 10 должен быть удален

    //    // Тестирование удаления элемента, которого нет
    //    tree.remove(100); // Ничего не произойдет, дерево остается корректным

    //    // Тестирование итератора
    //    tree.insert(7);
    //    tree.insert(3);
    //    tree.insert(8);

    //    AVLTree<int>::Iterator it = tree.begin();
    //    for (int val : tree) {
    //        cout << val << " ";
    //    }
    //    assert(it != tree.end()); // Должен иметь хотя бы один элемент
    //    assert(*it == 3);         // Первый элемент должен быть 3
    //    ++it;
    //    assert(*it == 5);         // Второй элемент должен быть 5
    //    ++it;
    //    assert(*it == 7);         // Третий элемент должен быть 7
    //    ++it;
    //    assert(*it == 8);         // Четвертый элемент должен быть 8
    //    ++it;
    //    assert(*it == 15);         // Четвертый элемент должен быть 8
    //    ++it;
    //    assert(it == tree.end()); // Достигнуть конца

    //    // Тестирование функции очистки дерева
    //    tree.clear();
    //    assert(tree.findIt(5) == nullptr);  // Все элементы должны быть удалены
    //    assert(tree.begin() == tree.end()); // Итератор должен быть пустым

    //    // Тестирование поворотов дерева
    //    tree.clear();
    //    vector<int> right;
    //    size_t i;

    //    // Поворот право право (малый правый поворот)
    //    tree.insert(7);
    //    tree.insert(8);
    //    tree.insert(5);
    //    tree.insert(6);
    //    tree.insert(3);
    //    tree.insert(4);
    //    tree.insert(1);
    //    right = { 1, 3, 4, 5, 6, 7, 8 };
    //    i = 0;
    //    for (int value : tree)
    //    {
    //        assert(abs(tree.renewBalanceFactor(value)) < 2);
    //        i++;
    //    }
    //    tree.clear();


    //    // Поворот лево лево (малый левый поворот)
    //    tree.insert(2);
    //    tree.insert(1);
    //    tree.insert(4);
    //    tree.insert(3);
    //    tree.insert(6);
    //    tree.insert(5);
    //    tree.insert(7);
    //    right.clear();
    //    right = { 1, 2, 3, 4, 5, 6, 7 };
    //    i = 0;
    //    for (int value : tree)
    //    {
    //        assert(abs(tree.renewBalanceFactor(value)) < 2);
    //        i++;
    //    }
    //    tree.clear();

    //    // Поворот право лево
    //    tree.insert(6);
    //    tree.insert(7);
    //    tree.insert(2);
    //    tree.insert(1);
    //    tree.insert(4);
    //    tree.insert(3);
    //    tree.insert(5);
    //    right.clear();
    //    right = { 1, 2, 3, 4, 5, 6, 7 };
    //    i = 0;
    //    for (int value : tree)
    //    {
    //        assert(abs(tree.renewBalanceFactor(value)) < 2);
    //        i++;
    //    }
    //    tree.clear();

    //    // Поворот лево право
    //    tree.insert(2);
    //    tree.insert(1);
    //    tree.insert(6);
    //    tree.insert(4);
    //    tree.insert(3);
    //    tree.insert(5);
    //    tree.insert(7);
    //    AVLTree<int>::Iterator it5 = tree.begin();
    //    right.clear();
    //    right = { 1, 2, 3, 4, 5, 6, 7 };
    //    i = 0;
    //    for (int value : tree)
    //    {
    //        assert(*it5 == right[i]);
    //        ++it5;
    //        i++;
    //    }
    //    tree.clear();

    //    std::cout << "All tests passed successfully!" << std::endl;
    //}


};
/// <summary>
/// Вспомогательный метод для рекурсивного поиска элемента в AVL-дереве.
/// Если элемент не найден, функция возвращает nullptr.
/// </summary>
/// <param name="node">Узел, в котором будет производиться поиск.
/// Если значение равно nullptr, поиск завершится безуспешно.</param>
/// <param name="data">Данные, которые необходимо найти в дереве.</param>
/// <returns>
/// Возвращает указатель на узел, содержащий искомые данные,
/// или nullptr, если данные не найдены.
/// </returns>
template<typename T>
AVLTreeNodule<T>* findReqursive(AVLTreeNodule<T>* node, const T& data) {
    // Если текущий узел пустой, значит элемент не найден
    if (node == nullptr) {
        return nullptr;
    }

    // Если искомые данные меньше данных текущего узла,
    // продолжаем поиск в левом поддереве
    if (data < node->noduleData) {
        return findReqursive(node->getLeft(), data);
    }
    // Если искомые данные больше данных текущего узла,
    // продолжаем поиск в правом поддереве
    else if (data > node->noduleData) {
        return findReqursive(node->getRight(), data);
    }
    // Если искомые данные равны данным текущего узла,
    // возвращаем текущий узел как результат поиска
    else {
        return node;
    }
}


/// <summary>
/// Функция для очистки дерева по узлу node
/// Если node пуст, функция закончит выполнение
/// </summary>
/// <param name="node">Узел, что очищается</param>
template<typename T>
void clearNode(AVLTreeNodule<T>* node) {
    if (node == nullptr) {
        return;
    }

    // Слева
    clearNode(node->getLeft());
    // Справа
    clearNode(node->getRight());
    delete node;
}

/// <summary>
/// Вспомогательный метод для вывода дерева в виде вектора.
/// Рекурсивный метод, который добавляет значения узлов дерева в переданный вектор.
/// Обходит левое поддерево, затем добавляет значение узла и обходит правое поддерево.
/// </summary>
/// <param name="node">Узел, с которого начинаем</param>
/// <param name="result">То, куда результат пишется</param>
template<typename T>
void toVectorHelper(AVLTreeNode<T>* node, std::vector<T>& result) {
    if (node == nullptr) {
        return; // Если узел пустой, просто возвращаемся.
    }

    // Рекурсивный вызов для левого поддерева
    toVectorHelper(node->getLeft(), result);
    // Добавляем значение текущего узла в вектор
    result.push_back(node->n_data);
    // Рекурсивный вызов для правого поддерева
    toVectorHelper(node->getRight(), result);
};
