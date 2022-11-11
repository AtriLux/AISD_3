#include "Tree.h"

int main()
{
    Tree<int> tree(12);
    /*tree.insert(tree.getRoot(), 1);
    tree.insert(tree.getRoot(), 2);
    tree.insert(tree.getRoot(), 3);
    tree.insert(tree.getRoot(), 4);
    tree.insert(tree.getRoot(), 5);
    tree.insert(tree.getRoot(), 6);
    tree.insert(tree.getRoot(), 7);
    tree.insert(tree.getRoot(), 8);
    tree.insert(tree.getRoot(), 9);
    tree.insert(tree.getRoot(), 10);
    tree.insert(tree.getRoot(), 125);
    tree.insert(tree.getRoot(), 11);
    tree.insert(tree.getRoot(), 12);
    tree.insert(tree.getRoot(), 13);
    tree.insert(tree.getRoot(), 14);*/

    //tree.treePrint(tree.getRoot());
    tree.print(tree.getRoot());
    _getch();
    //cout << endl << endl;

    //tree.remove(tree.getRoot(), 3);

    //tree.treePrint(tree.getRoot());
}

