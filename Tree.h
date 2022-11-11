#pragma once
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <list>
#include <iterator>

using namespace std;

void GoToXY(int x, int y) {
	HANDLE StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { x, y };
	SetConsoleCursorPosition(StdOut, coord);
}

template <typename T>
class Tree {
public:
	Tree();
	Tree(int size);
	Tree(const Tree<T>& other);
	~Tree();

	class Node {
	public:
		T keys[3];
		Node* first;
		Node* second;
		Node* third;
		Node* fourth;
		Node* parent;
		int size; // ���-�� ������� ������

		bool contain(int key) { // ���� ����� ���������� true, ���� ���� key ��������� � �������, ����� false.
			for (int i = 0; i < size; ++i)
				if (keys[i] == key) return true;
			return false;
		}

		void swap(int& x, int& y) {
			int r = x;
			x = y;
			y = r;
		}

		void sort2(int& x, int& y) {
			if (x > y) swap(x, y);
		}

		void sort3(int& x, int& y, int& z) {
			if (x > y) swap(x, y);
			if (x > z) swap(x, z);
			if (y > z) swap(y, z);
		}

		void sort() { // ����� � �������� ������ ���� �������������
			if (size == 1) return;
			if (size == 2) sort2(keys[0], keys[1]);
			if (size == 3) sort3(keys[0], keys[1], keys[2]);
		}

		void insertToNode(int key) {  // ��������� ���� key � ������� (�� � ������)
			keys[size] = key;
			size++;
			sort();
		}

		void removeFromNode(int key) { // ������� ���� key �� ������� (�� �� ������)
			if (size >= 1 && keys[0] == key) {
				keys[0] = keys[1];
				keys[1] = keys[2];
				size--;
			}
			else if (size == 2 && keys[1] == key) {
				keys[1] = keys[2];
				size--;
			}
		}

		void becomeNode2(int key, Node* f, Node* s) {  // ������������� � 2-�������.
			keys[0] = key;
			first = f;
			second = s;
			third = nullptr;
			fourth = nullptr;
			parent = nullptr;
			size = 1;
		}

		bool isLeaf() { // �������� �� ���� ������; �������� ������������ ��� ������� � ��������.
			return (first == nullptr) && (second == nullptr) && (third == nullptr);
		}
	};

	// ��������������� �������
	Node* getRoot() { // ����� ����� ������
		return root;
	};

	// �������� �������
	void treePrint(Node* node) {
		if (node->first)
			treePrint(node->first);
		cout << "ldata: " << node->keys[0] << endl;
		if (node->second)
			treePrint(node->second);
		if (node->size == 2) {
			cout << "rdata: " << node->keys[1] << endl;
		}
		if (node->third)
			treePrint(node->third);
	}
	
	void print(Node* root, int level = 0, int x = 50, int y = 0, int c = 0) { // c = 0 - ������, c = 1 - first, c = 2 - second
		if (root) {
			if (c != 0) {
				if (c == 1) x -= 40 - level * 15;
				else if (c == 3) x += 30 - level * 10;
			}

			GoToXY(x, y++);
			if (c == 1) printf("/");
			if (c == 2) printf("|");
			if (c == 3) printf("\\");

			if (c == 1) GoToXY(x - 2, y++);
			else GoToXY(x, y++);
			printf("%d", root->keys[0]);
			if (root->size == 2) printf(" %d", root->keys[1]);

			level++;
			if (root->first) print(root->first, level, x, y, 1);
			if (root->second) print(root->second, level, x, y, 2);
			if (root->third) print(root->third, level, x, y, 3);
		}
	}

	// ������� � ������
	Node* insert(Node* p, int key) { // ������� ����� key � ������ � ������ p; ������ ���������� ������ ������, �.�. �� ����� ��������
		if (!p) {
			Node* newNode = (Node*)malloc(sizeof(Node)); // ���� ������ �����, �� ������� ������ 2-3-������� (������)
			newNode->keys[0] = key;
			newNode->first = nullptr;
			newNode->second = nullptr;
			newNode->third = nullptr;
			newNode->fourth = nullptr;
			newNode->parent = nullptr;
			newNode->size = 1;
			return newNode;
		}

		if (p->isLeaf()) p->insertToNode(key);
		else if (key <= p->keys[0]) insert(p->first, key);
		else if ((p->size == 1) || ((p->size == 2) && key <= p->keys[1])) insert(p->second, key);
		else insert(p->third, key);

		return split(p);
	}

	// ����� ��� ���������� ������� ��� ������������
	Node* split(Node* elem) {
		if (elem->size < 3) return elem;

		// ������� ��� ����� �������, ������� ����� ������ �� ��������, ��� � ������������� �������.
		Node* x = (Node*)malloc(sizeof(Node));
		x->keys[0] = elem->keys[0];
		x->first = elem->first;
		x->second = elem->second;
		x->third = nullptr;
		x->fourth = nullptr;
		x->parent = elem->parent;
		x->size = 1;

		Node* y = (Node*)malloc(sizeof(Node));
		y->keys[0] = elem->keys[2];
		y->first = elem->third;
		y->second = elem->fourth;
		y->third = nullptr;
		y->fourth = nullptr;
		y->parent = elem->parent;
		y->size = 1;

		if (x->first)  x->first->parent = x;    // ��������� ������������� "��������" "�������".
		if (x->second) x->second->parent = x;   // ����� ����������, "���������" "�������" �������� "�������",
		if (y->first)  y->first->parent = y;    // ������� ����� ��������� ���������� ���������.
		if (y->second) y->second->parent = y;

		if (elem->parent) {
			elem->parent->insertToNode(elem->keys[1]);

			if (elem->parent->first == elem) elem->parent->first = nullptr;
			else if (elem->parent->second == elem) elem->parent->second = nullptr;
			else if (elem->parent->third == elem) elem->parent->third = nullptr;

			// ������ ���������� ������������ ���������� ������ ��� ����������.
			if (elem->parent->first == nullptr) {
				elem->parent->fourth = elem->parent->third;
				elem->parent->third = elem->parent->second;
				elem->parent->second = y;
				elem->parent->first = x;
			}
			else if (elem->parent->second == nullptr) {
				elem->parent->fourth = elem->parent->third;
				elem->parent->third = y;
				elem->parent->second = x;
			}
			else {
				elem->parent->fourth = y;
				elem->parent->third = x;
			}

			Node* temp = elem->parent;
			delete elem;
			return temp;
		}
		else {
			x->parent = elem;   // ��� ��� � ��� ����� �������� ������ ������,
			y->parent = elem;   // �� �� "���������" ����� ������ ������ ������������� �������.
			elem->becomeNode2(elem->keys[1], x, y);
			return elem;
		}
	}

	// ����� � ������
	Node* search(Node* p, int key) { // ����� ����� key � 2-3 ������ � ������ p.
		if (!p) return nullptr;

		if (p->contain(key)) return p;
		else if (key < p->keys[0]) return search(p->first, key);
		else if ((p->size == 2) && (key < p->keys[1]) || (p->size == 1)) return search(p->second, key);
		else if (p->size == 2) return search(p->third, key);
	}

	// ����� ������������ �������� � ���������
	Node* searchMin(Node* p) { // ����� ���� � ����������� ��������� � 2-3-������ � ������ p.
		if (!p) return p;
		if (!(p->first)) return p;
		else return searchMin(p->first);
	}

	// �������� �� ������
	Node* remove(Node* p, int key) { // �������� ����� key � 2-3-������ � ������ p.
		Node* elem = search(p, key); // ���� ����, ��� ��������� ���� key

		if (!elem) return p;

		Node* min = nullptr;
		if (elem->keys[0] == key) min = searchMin(elem->second); // ���� ������������� ����
		else min = searchMin(elem->third);

		if (min) { // ������ ����� �������
			int& z = (key == elem->keys[0] ? elem->keys[0] : elem->keys[1]);
			elem->swap(z, min->keys[0]);
			elem = min; // ���������� ��������� �� ����, �.�. min - ������ ����
		}

		elem->removeFromNode(key); // � ������� ��������� ���� �� �����
		return fix(elem); // �������� ������� ��� �������������� ������� ������.
	}

	// ������������ ����� �������� ��� ����������� ������� ������ (���������� merge ��� redistribute) 
	Node* fix(Node* leaf) {
		if (leaf->size == 0 && leaf->parent == nullptr) { // ������ 0, ����� ������� ������������ ���� � ������
			delete leaf;
			return nullptr;
		}
		if (leaf->size != 0) { // ������ 1, ����� �������, � ������� ������� ����, ����� ��� �����
			if (leaf->parent) return fix(leaf->parent);
			else return leaf;
		}

		Node* parent = leaf->parent;
		if (parent->first->size == 2 || parent->second->size == 2 || parent->size == 2) leaf = redistribute(leaf); // ������ 2, ����� ���������� ���������������� ����� � ������
		else if (parent->size == 2 && parent->third->size == 2) leaf = redistribute(leaf); // ����������
		else leaf = merge(leaf); // ������ 3, ����� ����� ���������� ���������� � �������� ����� �� ������ ��� ������� �� ��� ���� �������

		return fix(leaf);
	}

	// ����������������� ����� ������������ ��� ��������
	Node* redistribute(Node* leaf) {
		Node* parent = leaf->parent;
		Node* first = parent->first;
		Node* second = parent->second;
		Node* third = parent->third;

		if ((parent->size == 2) && (first->size < 2) && (second->size < 2) && (third->size < 2)) {
			if (first == leaf) {
				parent->first = parent->second;
				parent->second = parent->third;
				parent->third = nullptr;
				parent->first->insertToNode(parent->keys[0]);
				parent->first->third = parent->first->second;
				parent->first->second = parent->first->first;

				if (leaf->first != nullptr) parent->first->first = leaf->first;
				else if (leaf->second != nullptr) parent->first->first = leaf->second;

				if (parent->first->first != nullptr) parent->first->first->parent = parent->first;

				parent->removeFromNode(parent->keys[0]);
				free(first);
			}
			else if (second == leaf) {
				first->insertToNode(parent->keys[0]);
				parent->removeFromNode(parent->keys[0]);
				if (leaf->first != nullptr) first->third = leaf->first;
				else if (leaf->second != nullptr) first->third = leaf->second;

				if (first->third != nullptr) first->third->parent = first;

				parent->second = parent->third;
				parent->third = nullptr;

				free(second);
			}
			else if (third == leaf) {
				second->insertToNode(parent->keys[1]);
				parent->third = nullptr;
				parent->removeFromNode(parent->keys[1]);
				if (leaf->first != nullptr) second->third = leaf->first;
				else if (leaf->second != nullptr) second->third = leaf->second;

				if (second->third != nullptr)  second->third->parent = second;

				free(third);
			}
		}
		else if ((parent->size == 2) && ((first->size == 2) || (second->size == 2) || (third->size == 2))) {
			if (third == leaf) {
				if (leaf->first != nullptr) {
					leaf->second = leaf->first;
					leaf->first = nullptr;
				}

				leaf->insertToNode(parent->keys[1]);
				if (second->size == 2) {
					parent->keys[1] = second->keys[1];
					second->removeFromNode(second->keys[1]);
					leaf->first = second->third;
					second->third = nullptr;
					if (leaf->first != nullptr) leaf->first->parent = leaf;
				}
				else if (first->size == 2) {
					parent->keys[1] = second->keys[0];
					leaf->first = second->second;
					second->second = second->first;
					if (leaf->first != nullptr) leaf->first->parent = leaf;

					second->keys[0] = parent->keys[0];
					parent->keys[0] = first->keys[1];
					first->removeFromNode(first->keys[1]);
					second->first = first->third;
					if (second->first != nullptr) second->first->parent = second;
					first->third = nullptr;
				}
			}
			else if (second == leaf) {
				if (third->size == 2) {
					if (leaf->first == nullptr) {
						leaf->first = leaf->second;
						leaf->second = nullptr;
					}
					second->insertToNode(parent->keys[1]);
					parent->keys[1] = third->keys[0];
					third->removeFromNode(third->keys[0]);
					second->second = third->first;
					if (second->second != nullptr) second->second->parent = second;
					third->first = third->second;
					third->second = third->third;
					third->third = nullptr;
				}
				else if (first->size == 2) {
					if (leaf->second == nullptr) {
						leaf->second = leaf->first;
						leaf->first = nullptr;
					}
					second->insertToNode(parent->keys[0]);
					parent->keys[0] = first->keys[1];
					first->removeFromNode(first->keys[1]);
					second->first = first->third;
					if (second->first != nullptr) second->first->parent = second;
					first->third = nullptr;
				}
			}
			else if (first == leaf) {
				if (leaf->first == nullptr) {
					leaf->first = leaf->second;
					leaf->second = nullptr;
				}
				first->insertToNode(parent->keys[0]);
				if (second->size == 2) {
					parent->keys[0] = second->keys[0];
					second->removeFromNode(second->keys[0]);
					first->second = second->first;
					if (first->second != nullptr) first->second->parent = first;
					second->first = second->second;
					second->second = second->third;
					second->third = nullptr;
				}
				else if (third->size == 2) {
					parent->keys[0] = second->keys[0];
					second->keys[0] = parent->keys[1];
					parent->keys[1] = third->keys[0];
					third->removeFromNode(third->keys[0]);
					first->second = second->first;
					if (first->second != nullptr) first->second->parent = first;
					second->first = second->second;
					second->second = third->first;
					if (second->second != nullptr) second->second->parent = second;
					third->first = third->second;
					third->second = third->third;
					third->third = nullptr;
				}
			}
		}
		else if (parent->size == 1) {
			leaf->insertToNode(parent->keys[0]);

			if (first == leaf && second->size == 2) {
				parent->keys[0] = second->keys[0];
				second->removeFromNode(second->keys[0]);

				if (leaf->first == nullptr) leaf->first = leaf->second;

				leaf->second = second->first;
				second->first = second->second;
				second->second = second->third;
				second->third = nullptr;
				if (leaf->second != nullptr) leaf->second->parent = leaf;
			}
			else if (second == leaf && first->size == 2) {
				parent->keys[0] = first->keys[1];
				first->removeFromNode(first->keys[1]);

				if (leaf->second == nullptr) leaf->second = leaf->first;

				leaf->first = first->third;
				first->third = nullptr;
				if (leaf->first != nullptr) leaf->first->parent = leaf;
			}
		}
		return parent;
	}

	// ������� ������������ ��� ��������
	Node* merge(Node* leaf) {
		Node* parent = leaf->parent;

		if (parent->first == leaf) {
			parent->second->insertToNode(parent->keys[0]);
			parent->second->third = parent->second->second;
			parent->second->second = parent->second->first;

			if (leaf->first != nullptr) parent->second->first = leaf->first;
			else if (leaf->second != nullptr) parent->second->first = leaf->second;

			if (parent->second->first != nullptr) parent->second->first->parent = parent->second;

			parent->removeFromNode(parent->keys[0]);
			delete parent->first;
			parent->first = nullptr;
		}
		else if (parent->second == leaf) {
			parent->first->insertToNode(parent->keys[0]);

			if (leaf->first != nullptr) parent->first->third = leaf->first;
			else if (leaf->second != nullptr) parent->first->third = leaf->second;

			if (parent->first->third != nullptr) parent->first->third->parent = parent->first;

			parent->removeFromNode(parent->keys[0]);
			delete parent->second;
			parent->second = nullptr;
		}

		if (parent->parent == nullptr) {
			Node* temp = nullptr;
			if (parent->first != nullptr) temp = parent->first;
			else temp = parent->second;
			temp->parent = nullptr;
			delete parent;
			return temp;
		}
		return parent;
	}

protected:
	Node* root;	// ������ ������
};

template <typename T>
Tree<T>::Tree() {
	Node* first = nullptr;
	Node* second = nullptr;
	Node* third = nullptr;
	Node* fourth = nullptr;
	Node* parent = nullptr;
}

template <typename T>
Tree<T>::Tree(int size) {
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		T key = rand() % 100;
		//Node* temp = contain(root, elem);
		//while (temp && temp->key == elem) {
			//elem = rand() % 100;
		//}
		root = insert(root, key);
	}
}

template <typename T>
Tree<T>::~Tree() {
}


// TODO
// 1.1 ���������� -_-
// 1.2 ����������� ����������� ;|
// 2. ��������� )00))0)
// 3. ����� ����� ������������� ����� ���������� ��������� :/
// 4. ������ ��������� ����� ^-^
// 5. ����������� �_�
// 6. ���� F
// 7. ����� ������� ������ c==3