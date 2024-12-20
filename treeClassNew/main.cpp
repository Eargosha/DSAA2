// Author: Eargosha

// А я такое дерево вырастил [ну как я))]:
//         8
//        /  \
//      3     10
//     /  \     \
//    1    6     14
//        / \    /
//       4   7  13
// Оно строгое, ЪыЪ

#include <iostream>
#include "BinarySearchTreeHeader.h"

// Проверить комменты

int main()
{
    BinarySearchTree<int> testTree;
    testTree.doTest();
    testTree.assertIterator();

    setlocale(LC_ALL, "Russian");
    int wayToGo;
    cout << "Какой способ построения? Введи 1 чтобы построить дерево через BinarySearchTree, 2 - чтобы через TreeNodule ";
    cin >> wayToGo;
    if (wayToGo == 1)
    {
        BinarySearchTree<int> tree;

        tree.insert(8);
        tree.insert(3);
        tree.insert(10);
        tree.insert(1);
        tree.insert(6);
        tree.insert(14);
        // tree.insert(4);
        // tree.insert(7);
        // tree.insert(13);

        // cout << "Корень:" << endl;
        // tree.printNode(tree.getRoot());

        // cout << "Справа от 14:" << endl;
        // tree.printNode(tree.getNodeByValue(14)->noduleRight);
        // cout << "Слева от 14:" << endl;
        // tree.printNode(tree.getNodeByValue(14)->noduleLeft);

        // cout << "<========== ДеревошкА ==========>" << endl;
        // tree.printTree();

        // tree.printBypassInorderLNR();
        // tree.printBypassPreorderNRL();
        // tree.printBypassPostorderRLN();

        // tree.remove(8);
        // cout << "<========== Деревошка после изменениЯ ==========>" << endl;
        // tree.printTree();
        auto it = tree.begin(); // Сохраняем итератор

        // while (it != tree.end())
        // {
        //     cout << it.data() << endl; // Выводит значение текущего узла
        //     ++it;                      // Переход к следующему элементу
        // }
        // Итерация по дереву
        // auto it = tree.begin();
        for (auto it = tree.begin(); it.hasNext(); ++it)
        {
            cout << *it << endl;
        }
    }
    if (wayToGo == 2)
    {
        //         10
        //        /  \
//      5     15
        //     /  \  /  \
//    3    7 12  20  Совершенно!!!

        cout << "<========== ДеревошкА ==========>" << endl;

        TreeNodule<int> root;
        root.noduleData = 10;

        TreeNodule<int> *leftNode = new TreeNodule<int>(5);
        TreeNodule<int> *leftLeftNode = new TreeNodule<int>(3);
        TreeNodule<int> *leftRightNode = new TreeNodule<int>(7);
        leftNode->noduleLeft = leftLeftNode;
        leftNode->noduleRight = leftRightNode;

        TreeNodule<int> *rightNode = new TreeNodule<int>(15);
        TreeNodule<int> *rightLeftNode = new TreeNodule<int>(12);
        TreeNodule<int> *rightRightNode = new TreeNodule<int>(20);
        rightNode->noduleLeft = rightLeftNode;
        rightNode->noduleRight = rightRightNode;

        root.noduleLeft = leftNode;
        root.noduleRight = rightNode;

        printTreeRecursive(&root, 0);

        cout << "<========== Самый правый листик ==========>\n"
             << root.noduleRight->noduleRight->noduleData;

        delete rightRightNode;
        delete rightLeftNode;
        delete leftRightNode;
        delete leftLeftNode;
        delete leftNode;
        delete rightNode;
        root.~TreeNodule();
    }

    return 0;
}