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
#include "BinaryTree.h"

int main()
{
    BinaryTree<int> tree;

    tree.insert(8);
    tree.insert(3);
    tree.insert(10);
    tree.insert(1);
    tree.insert(6);
    tree.insert(14);
    tree.insert(4);
    tree.insert(7);
    tree.insert(13);

    cout << "Корень:" << endl;
    tree.printNode(tree.getRoot());

    cout << "Справа от 14:" << endl;
    tree.printNode(tree.getNodeByValue(14)->noduleRight);
    cout << "Слева от 14:" << endl;
    tree.printNode(tree.getNodeByValue(14)->noduleLeft);

    cout << "<========== ДеревошкА ==========>" << endl;
    tree.printTree();

    cout << "<========== Инордерный обхоД ==========>" << endl;
    tree.inorder();

    return 0;
}
