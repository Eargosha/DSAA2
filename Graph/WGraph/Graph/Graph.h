#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <limits>
#include <queue>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>

using namespace std;
// Граф (Graph) — совокупность двух множеств — множества самих объектов, называемого множеством вершин ( vertices, nodes), 
// и множества их парных связей, называемого множеством рёбер (edge, link)
// Обозначение: G ( V , E )
// V — множество вершин,
// E – множеством рёбер

/// <summary>
/// Класс неориентированного взвешенного графа
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class UWGraph {
private:
    vector<T> vertices; // Список вершин
    vector<vector<double>> adjacencyMatrix; // Матрица смежности.

public:
    // Конструктор
    UWGraph() {}

    // Деструктор
    ~UWGraph() {}

    /// <summary>
    /// Функция, что возращает индекс вершины из массива по ее значению
    /// Вернет -1, если не удалось найти вершину в графе
    /// BigO - O(n)
    /// </summary>
    /// <param name="vertex">Веришна, что ищут</param>
    /// <returns>Значение int - индекс массива</returns>
    int getVertexIndexInMassiveOfVertices(const T& vertex) const {
        for (int i = 0; i < vertices.size(); ++i) {
            if (vertices[i] == vertex) {
                return i;
            }
        }
        return -1; // Вершина не найдена
    }

    /// <summary>
    /// Метод добавления вершины. Бросает исключение out_of_range если вершина существует 
    /// BigO - O(n^2)
    /// </summary>
    /// <param name="vertex">То, что будет вставлено</param>
    void insertVertex(const T& vertex) {
        // Проверяем, не существует ли уже такая вершина
        if (getVertexIndexInMassiveOfVertices(vertex) != -1) {
            throw out_of_range("Vertex already exists.");
        }

        // Добавляем новую вершину
        vertices.push_back(vertex);
        // Увеличиваем размер матрицы смежности
        int newSize = vertices.size();

        // Обновляем матрицу
        adjacencyMatrix.resize(newSize, vector<double>(newSize, 0.0));
        if (adjacencyMatrix.empty()) {
            adjacencyMatrix.resize(newSize, vector<double>(newSize, 0.0));
        }
        else {
            for (auto& row : adjacencyMatrix) {
                row.resize(newSize, 0.0);
            }
            adjacencyMatrix.resize(newSize, vector<double>(newSize, 0.0));
        }
    }

    /// <summary>
    /// Метод для удаления вершины. Бросает исключение out_of_range если вершина не была найдена
    /// BigO - O(n^2)
    /// </summary>
    /// <param name="vertex">То, что будет удалено</param>
    void deleteVertex(const T& vertex) {
        // Находим вершину в массиве
        int index = getVertexIndexInMassiveOfVertices(vertex);
        if (index == -1) {
            throw out_of_range("Vertex does not exist.");
        }

        // Удаляем вершину
        vertices.erase(index + vertices.begin());

        // Удаляем соответствующий ряд и столбец из матрицы смежности
        adjacencyMatrix.erase(adjacencyMatrix.begin() + index);
        for (auto& row : adjacencyMatrix) {
            row.erase(row.begin() + index);
        }
    }

    /// <summary>
    /// Метод вставки ребра. Бросает исключение out_of_range если вершина не существует
    /// BigO - O(n)
    /// </summary>
    /// <param name="from">Точка, откуда ребро идет</param>
    /// <param name="to">Точка, куда ребро идет</param>
    /// <param name="weight">Вест ребра</param>
    void insertEdge(const T& from, const T& to, double weight) {
        // Находим индексы и проверяем их
        int fromIndex = getVertexIndexInMassiveOfVertices(from);
        int toIndex = getVertexIndexInMassiveOfVertices(to);
        if (fromIndex == -1 || toIndex == -1) {
            throw out_of_range("One or two vertices do not exist.");
        }

        // Добавляем вес ребра в матрицу смежности
        adjacencyMatrix[fromIndex][toIndex] = weight;
        adjacencyMatrix[toIndex][fromIndex] = weight; 
    }

    /// <summary>
    /// Метод удаления ребра. Бросает исключение out_of_range если вершина не существует
    /// BigO - O(n)
    /// </summary>
    /// <param name="from">Точка, откуда ребро идет</param>
    /// <param name="to">Точка, куда ребро идет</param>
    void deleteEdge(const T& from, const T& to) {
        // Находим индексы и проверяем их
        int fromIndex = getVertexIndexInMassiveOfVertices(from);
        int toIndex = getVertexIndexInMassiveOfVertices(to);
        if (fromIndex == -1 || toIndex == -1) {
            throw out_of_range("One or two vertices do not exist.");
        }

        // Удаляем ребро из матрицы смежности
        adjacencyMatrix[fromIndex][toIndex] = 0.0;
        adjacencyMatrix[toIndex][fromIndex] = 0.0; 
    }

    /// <summary>
    /// Метод для редактирования значения вершины. Бросает исключение out_of_range если вершина не существует
    /// Бросает исключение runtime_error если такая вершина существует, дубликаты не создаются
    /// BigO - O(n)
    /// </summary>
    /// <param name="oldVertex">Старое значение вершины</param>
    /// <param name="newVertex">Новое значение вершины</param>
    void editVertex(const T& oldVertex, const T& newVertex) {
        int index = getVertexIndexInMassiveOfVertices(oldVertex);
        if (index == -1) {
            throw out_of_range("This vertex do not exist.");
            return; // Вершина не найдена
        }

        // Проверяем, существует ли новая вершина
        if (getVertexIndexInMassiveOfVertices(newVertex) != -1) {
            throw runtime_error("Vertex already exists! Cannot rename it!");
            return; // Новая вершина уже существует
        }

        // Изменяем значение вершины
        vertices[index] = newVertex;
    }

    /// <summary>
    /// Метод для вывода графа
    /// BigO - O(n^3)
    /// </summary>
    void printGraph() const {
        cout << "\nGraph: \n";
        printVertices();

        printAdjacencyMatrix();
    }

    /// <summary>
    /// Метод для вывода вершин графа
    /// BigO - O(n)
    /// </summary>
    void printVertices() const {
        cout << "Vertices: ";
        for (const auto& v : vertices) {
            cout << v << " ";
        }
        cout << endl;
    }

    /// <summary>
    /// Метод для матрицы смежности графа
    /// BigO - O(n^2)
    /// </summary>
    void printAdjacencyMatrix() const {
        cout << "Adjacency Matrix:" << endl;
        for (const auto& row : adjacencyMatrix) {
            for (const auto& weight : row) {
                cout << weight << " ";
            }
            cout << endl;
        }
    }

    /// <summary>
    /// Возвращает список соседних вершин для заданной вершины.
    /// BigO - O(n) - в худшем случае, если все вершины являются соседями.
    /// </summary>
    /// <param name="vertex">Вершина, для которой нужно найти соседей.</param>
    /// <return>Список соседних вершин.</return>
    vector<T> getNeighbors(const T& vertex) {
        int index = getVertexIndexInMassiveOfVertices(vertex);
        if (index == -1) {
            throw out_of_range("Vertex not found.");
        }

        vector<T> neighbors;
        for (int i = 0; i < adjacencyMatrix.size(); ++i) {
            if (adjacencyMatrix[index][i] != 0.0) { // Предполагаем, что 0 означает отсутствие ребра
                neighbors.push_back(vertices[i]);
            }
        }
        return neighbors;
    }

    /// <summary>
    /// Метод для вывода соседних вершин
    /// BigO - O(n^3)
    /// </summary>
    void printNeighbors(const T& vertex) {
        vector<T> foundNeighbors = getNeighbors(vertex);
        cout << "Neighbors: ";
        for (const auto& v : foundNeighbors) {
            cout << v << " ";
        }
        cout << endl;
    }

    /// <summary>
    /// Возвращает вес ребра между двумя вершинами.
    /// BigO - O(n)
    /// </summary>
    /// <param name="from">Начальная вершина.</param>
    /// <param name="to">Конечная вершина.</param>
    /// <return>Вес ребра между двумя вершинами.</return>
    double getWeight(const T& from, const T& to) {
        int fromIndex = getVertexIndexInMassiveOfVertices(from);
        int toIndex = getVertexIndexInMassiveOfVertices(to);

        if (fromIndex == -1 || toIndex == -1) {
            throw out_of_range("One or both vertices not found.");
        }

        return adjacencyMatrix[fromIndex][toIndex];
    }

    /// <summary>
    /// Метод, что возращает кол-во ребер в графе
    /// BigO - O(n^2)
    /// </summary>
    /// <returns>Возращает число int - кол-во ребер</returns>
    int getCountOfEdges() {
        int count = 0;
        for (int i = 0; i < vertices.size(); ++i) {
            for (int j = i + 1; j < vertices.size(); ++j) {
                if (adjacencyMatrix[i][j] != 0.0) {
                    count++;
                }
            }
        }
        return count;
    }

    /// <summary>
    /// Метод, что возращает кол-во узлов в графе
    /// BigO - O(1)
    /// </summary>
    /// <returns>Возращает число int - кол-во узлов</returns>
    int getCountOfVertices() const {
        return vertices.size();
    }

    /// <summary>
    /// Поиск минимально возможного пути между двумя вершинами методом Дейкстры. Бросает исключение out_of_range если вершина не существует
    /// BigO - O(n^2 + E) = связный|несвязный - O(1) = пустой
    /// O(|V| log|V|) в лучшем, O(|V| + |E| log|V|) в среднем и худшем
    /// Memory - O(V+E) 
    /// </summary>
    /// <param name="start">Та самая вершина u, откуда считаются минимальные возможные пути</param>
    /// <returns>Массив с кратчайшими расстояниями от стартовой вершины</returns>
    vector<double> findShortestPathForVertex(const T& start) {
        // Проверка, что вершина вообще есть
        int startIndex = getVertexIndexInMassiveOfVertices(start);
        if (startIndex == -1) {
            throw out_of_range("Start vertex not found.");
        }

        int n = vertices.size();
        // У каждой вершины графа будет своя метка - минимальное известное расстояние до start. 
        // Метки всех вершин в начале помечаются как бесконечность
        vector<double> distances(n, numeric_limits<double>::infinity());
        // Массив для понимания, какую вершину посетили. В начале они все не посещаны
        vector<bool> visited(n, false);
        // Метка самой вершины start полагается быть равной 0
        distances[startIndex] = 0;

        for (int count = 0; count < n - 1; ++count) {
            // Из ещё не посещённых вершин выбирается вершина, имеющая минимальную метку
            double minDistance = numeric_limits<double>::infinity();
            int currentIndex = -1;

            for (int i = 0; i < n; ++i) {
                if (!visited[i] && distances[i] < minDistance) {
                    minDistance = distances[i];
                    currentIndex = i;
                }
            }

            // Если не найдено ни одной непосещенной вершины, выходим
            if (currentIndex == -1) {
                break;
            }

            // Пометим вершину u как посещённую
            visited[currentIndex] = true;

            // Обновляем расстояния для соседних вершин
            // Для каждого соседа вершины (кроме отмеченных как посещённые) рассмотрим новую длину пути
            for (int i = 0; i < n; ++i) {
                if (adjacencyMatrix[currentIndex][i] > 0 && !visited[i]) { // Если есть ребро и вершину еще не посетили
                    // Сумма значений текущей метки и длины ребра, соединяющего эту вершину с этим соседом.
                    double newDistance = distances[currentIndex] + adjacencyMatrix[currentIndex][i];
                    // Если полученное значение длины меньше значения метки соседа, заменим значение метки полученным значением длины.
                    if (newDistance < distances[i]) {
                        distances[i] = newDistance;
                    }
                }
            }
        }

        return distances; // Возвращаем массив с кратчайшими расстояниями от стартовой вершины
    }

    /// <summary>
    /// Алгоритм обхода графа в глубину. Использует рекурсию. Производит вывод обхода на экран. 
    /// Бросает исключение out_of_range если вершина не существует
    /// BigO - O(n + E) = связный|несвязный - O(1) = пустой
    /// </summary>
    /// <param name="startVertex">Вершина, с которой начинается обход</param>
    void depthFirstSearch(const T& startVertex) const {
        // Находим позицию стартовой вершины в графе 
        int startPosition = getVertexIndexInMassiveOfVertices(startVertex);

        // Если вершина не найдена, бросаем исключение 
        if (startPosition == -1) {
            throw out_of_range("Start vertex not found.");
        }

        // Создаем массив для отслеживания Visited
        vector<bool> visited(vertices.size(), false);

        // Вызываем рекурсивную функцию для обхода
        depthFirstSearchRecursive(startPosition, visited);
        cout << endl;
    }

    /// <summary>
    /// Вспомогательная рекурсивная для обхода графа в глубину. Находится внутри класса так как использует его поле.
    /// Производит вывод обхода на экран.
    /// </summary>
    /// <param name="position">Позиция обхода сейчас</param>
    /// <param name="visited">Массив посещенных вершин</param>
    void depthFirstSearchRecursive(int position, vector<bool>& visited) const {
        // Помечаем вершину как посещенную 
        visited[position] = true;
        // Выводим вершину на экран 
        cout << vertices[position] << " ";

        // Для всех соседних вершин, которые еще не были посещены
        for (int i = 0; i < vertices.size(); ++i) {
            double weight = 0.0;
            if (position > i) {
                weight = adjacencyMatrix[position][i];
            }
            else if (position < i) {
                weight = adjacencyMatrix[i][position];
            }

            // Если существует связь и вершина ещё не посещена
            if (weight != 0.0 && !visited[i]) {
                depthFirstSearchRecursive(i, visited); // Рекурсивно вызываем функцию для соседней вершины
            }
        }
    }
    
    /// <summary>
    /// Алгоритм обхода графа в ширину. Бросает исключение out_of_range если вершина не существует. 
    /// Производит вывод обхода на экран. Использует очередь для реализации алгоритма.
    /// BigO - O(n + E) = связный|несвязный - O(1) = пустой
    /// </summary>
    /// <param name="startVertex">Вершина с которой начинается обход</param>
    void breadthFirstSearch(const T& startVertex) const {
        // Находим позицию стартовой вершины в графе 
        int startPosition = getVertexIndexInMassiveOfVertices(startVertex);

        // Если вершина не найдена, бросаем исключение 
        if (startPosition == -1) {
            throw out_of_range("Start vertex not found.");
        }

        // Очередь
        queue<T> q;
        // Массив для хранения меток о посещении элементов
        vector<T> visited;

        // Поместить узел, с которого начинается поиск, в изначально пустую очередь.
        q.push(startVertex);
        // Пометить его как посещённый
        visited.push_back(startVertex);

        while (!q.empty()) {
            // Извлекаем его
            T vertex = q.front();
            q.pop();

            // Показываем
            cout << vertex << " ";

            // Получаем индекс current вершины
            int index = getVertexIndexInMassiveOfVertices(vertex);
            // В конец очереди добавляются все соседи current узла, которые ещё не посещены и не находятся в очереди.
            for (size_t i = 0; i < vertices.size(); ++i) {
                if (adjacencyMatrix[index][i] > 0 && find(visited.begin(), visited.end(), vertices[i]) == visited.end()) {
                    q.push(vertices[i]);
                    // Помечаем вершину как посещённую
                    visited.push_back(vertices[i]); 
                }
            }
            // Если очередь пуста, то завершить обход
        }
        cout << endl;
    }

    /// <summary> 
    /// Реализует алгоритм Форда-Беллмана для нахождения кратчайших расстояний 
    /// от заданной стартовой вершины до всех остальных вершин в графе.
    /// out_of_range выбрасывается, если стартовая вершина не найдена в графе.
    /// runtime_error выбрасывается, если в графе обнаружен отрицательный цикл.
    /// BigO - O(|E|)
    /// Лучший случай O(|E|), O(|V| |E|) - худший и средний
    /// Memory - O(V+E)
    /// </summary> 
    /// <param name="start">Стартовая вершина, от которой будут вычисляться расстояния.</param> 
    /// <returns>Вектор расстояний от стартовой вершины до каждой из вершин графа.</returns> 
    vector<double> bellmanFord(const T& start) {
        // Инициализировать массив расстояний от данной вершины к остальным значением бесконечности
        vector<double> distances(vertices.size(), numeric_limits<double>::infinity());

        // Проверка, существует ли данная вершина 
        int startIndex = getVertexIndexInMassiveOfVertices(start);
        if (startIndex == -1) throw out_of_range("Start vertex not found");

        // К себе расстояние = 0 
        distances[startIndex] = 0;

        // Основной цикл алгоритма - цикл по |V|-1 раз
        // Это необходимо, потому что в худшем случае для нахождения кратчайших путей нам нужно пройти через все вершины
        for (size_t i = 1; i < vertices.size(); ++i) {
            // Проходим по всем вершинам графа
            for (size_t u = 0; u < vertices.size(); ++u) {
                // Проверяем все возможные соседние вершины v для текущей вершины u
                for (size_t v = 0; v < vertices.size(); ++v) {
                    // Проверяем, есть ли ребро между вершинами u и v
                    if (adjacencyMatrix[u][v] < numeric_limits<double>::infinity()) { 
                        // Если есть ребро и
                        // Если найдено более короткое расстояние до v через u, обновляем его
                        if (distances[u] + adjacencyMatrix[u][v] < distances[v]) {
                            distances[v] = distances[u] + adjacencyMatrix[u][v];
                        }
                    }
                }
            }
        }

        // Проверка на наличие отрицательных циклов - дополнительный цикл 
        // Если после |V|-1 итераций мы можем еще улучшить расстояние, значит в графе есть отрицательный цикл
        for (size_t u = 0; u < vertices.size(); ++u) {
            for (size_t v = 0; v < vertices.size(); ++v) {
                // Проверяем, есть ли ребро между вершинами u и v
                if (adjacencyMatrix[u][v] < numeric_limits<double>::infinity()) { 
                    // Если есть ребро и
                    // Если можем улучшить расстояние до v, значит есть отрицательный цикл
                    if (distances[u] + adjacencyMatrix[u][v] < distances[v]) {
                        throw runtime_error("Graph contains a negative-weight cycle");
                    }
                }
            }
        }
        return distances;
    }

    static void testGraph() {
        /////////////// Пустой граф
        UWGraph<int> emptyGraph;
        assert(emptyGraph.getCountOfEdges() == 0);
        assert(emptyGraph.getCountOfVertices() == 0);

        try {
            emptyGraph.depthFirstSearch(3);
            assert(false);
        }
        catch (const out_of_range& e) { }

        try {
            emptyGraph.breadthFirstSearch(69);
            assert(false); 
        }
        catch (const out_of_range& e) { }

        try {
            emptyGraph.editVertex(132, 123);
            assert(false);
        }
        catch (const out_of_range& e) {}

        try {
            emptyGraph.deleteVertex(12312);
            assert(false);
        }
        catch (const out_of_range& e) {}

        try {
            emptyGraph.getNeighbors(12312);
            assert(false);
        }
        catch (const out_of_range& e) {}

        try {
            emptyGraph.getWeight(12312, 123);
            assert(false);
        }
        catch (const out_of_range& e) {}

        try {
            emptyGraph.findShortestPathForVertex(12312);
            assert(false);
        }
        catch (const out_of_range& e) {}

        try {
            emptyGraph.bellmanFord(12312);
            assert(false);
        }
        catch (const out_of_range& e) {}

        //////////// Связный граф
        UWGraph<int> connectedGraph;
        connectedGraph.insertVertex(1);
        connectedGraph.insertVertex(2);
        connectedGraph.insertVertex(3);
        connectedGraph.insertVertex(4);
        connectedGraph.insertVertex(5);
        connectedGraph.insertEdge(1, 2, 5.0);
        connectedGraph.insertEdge(2, 3, 1.0);
        connectedGraph.insertEdge(3, 4, 7.0);
        connectedGraph.insertEdge(4, 5, 1.0);

        assert(connectedGraph.getCountOfVertices() == 5);
        assert(connectedGraph.getCountOfEdges() == 4);

        // Проверка наличия вершин
        assert(connectedGraph.getVertexIndexInMassiveOfVertices(1) == 0);
        assert(connectedGraph.getVertexIndexInMassiveOfVertices(2) == 1);

        // Проверка удаления ребра
        connectedGraph.deleteEdge(1, 2);
        assert(connectedGraph.getCountOfEdges() == 3);

        // Проверка удаления вершины
        connectedGraph.deleteVertex(1);
        assert(connectedGraph.getCountOfVertices() == 4);
        vector<int> testik = {2, 4};
        assert(connectedGraph.getNeighbors(3) == testik);
        vector<double> testik2 = { 9, 8, 1, 0 };
        assert(connectedGraph.findShortestPathForVertex(5) == testik2);
        connectedGraph.depthFirstSearch(5);
        connectedGraph.breadthFirstSearch(5);


        ///////////// Несвязный граф
        UWGraph<int> disconnectedGraph;
        disconnectedGraph.insertVertex(3);
        disconnectedGraph.insertVertex(4);

        assert(disconnectedGraph.getCountOfVertices() == 2);
        assert(disconnectedGraph.getCountOfEdges() == 0);

        // Проверка удаления несуществующей вершины
        try {
            disconnectedGraph.deleteVertex(5);
            assert(false); // Должно выбросить исключение
        }
        catch (const out_of_range&) {
            // Ожидаем исключение
        }

        disconnectedGraph.breadthFirstSearch(3);
        disconnectedGraph.depthFirstSearch(4);
        testik2 = { 0, INFINITY };

        assert(disconnectedGraph.findShortestPathForVertex(3) == testik2);

        cout << "All tests passed!" << endl;
    }

};