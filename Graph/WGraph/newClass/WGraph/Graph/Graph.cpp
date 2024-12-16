#include <iostream>
#include "Graph.h"

using namespace std;

int main()
{
    UWGraph<string> graph;
    UWGraph<int>::testGraph();

    graph.insertVertex("A");
    graph.insertVertex("B");
    graph.insertVertex("C");
    graph.insertVertex("D");
    graph.insertVertex("E");
    graph.insertVertex("F");

    graph.insertEdge("A", "B", 1.5);
    graph.insertEdge("B", "C", 2.5);
    graph.insertEdge("D", "C", 0.5);
    graph.insertEdge("E", "F", 3.5);
    graph.insertEdge("A", "F", 2.5);
    graph.insertEdge("B", "E", 2.0);

    /*graph.printNeighbors("B");
    cout << graph.getWeight("B", "C") << endl;

    graph.printGraph();

    graph.deleteEdge("A", "B");
    graph.deleteVertex("C");

    graph.printGraph();*/
    vector<string> show;

    cout << "BFS now\n";
    show = graph.breadthFirstSearch("A");

    for (int i = 0; i < show.size(); i++) {
        cout << show[i] << " ";
    }
    cout << "\n";
    vector<string> show2;

    cout << "DFS now\n";
    show2 = graph.depthFirstSearch("A");

    for (int i = 0; i < show2.size(); i++) {
        cout << show2[i] << " ";
    }

    cout << "\n";
    cout << "Try FordBell\n";
    vector<double> show3;
    show3 = graph.bellmanFord("A");

    for (int i = 0; i < show3.size(); i++) {
        cout << show3[i] << " ";
    }

    //UWGraph<string> graph2;

    //// Добавляем вершины
    //graph2.insertVertex("A");
    //graph2.insertVertex("B");
    //graph2.insertVertex("C");
    //graph2.insertVertex("D");
    //graph2.insertVertex("E");

    //// Добавляем рёбра
    //graph2.insertEdge("A", "B", 1); // A-B
    //graph2.insertEdge("A", "C", 2); // A-C
    //graph2.insertEdge("A", "D", 4); // A-D
    //graph2.insertEdge("B", "C", 3); // B-C
    //graph2.insertEdge("B", "D", 2); // B-D
    //graph2.insertEdge("C", "D", 1); // C-D
    //graph2.insertEdge("D", "E", 5); // D-E

    //string startVertex = "A";

    //graph2.printGraph();

    //try {
    //    vector<double> shortestPaths = graph2.findShortestPathForVertex(startVertex);

    //    cout << "Shortest paths from vertex " << startVertex << ":\n";
    //    cout << "To Vertices \n ";
    //    graph2.printVertices();
    //    cout << "Have this shortest paths of D alg: \n";
    //    for (int i = 0; i < shortestPaths.size(); ++i) {
    //        
    //        cout << shortestPaths[i] << "\n";
    //    }

    //    
    //    UWGraph<string> graph3;
    //    graph3.insertVertex("a");
    //    graph3.insertVertex("b");
    //    graph3.insertVertex("c");
    //    graph3.insertVertex("d");
    //    graph3.insertEdge("a", "b", 1); // A-B
    //    graph3.insertEdge("b", "c", 2); // A-C
    //    graph3.insertEdge("b", "d", 4); // A-D

    //    vector<double> shortestPaths2 = graph3.bellmanFord("c");

    //    cout << "Shortest paths from vertex " << startVertex << ":\n";
    //    cout << "To Vertices \n ";
    //    graph3.printVertices();
    //    cout << "Have this shortest paths of B-F alg: \n";
    //    for (int i = 0; i < shortestPaths2.size(); ++i) {

    //        cout << shortestPaths2[i] << "\n";
    //    }
    //}
    //catch (const out_of_range& e) {
    //    cout << e.what() << endl;
    //}

    return 0;
}
