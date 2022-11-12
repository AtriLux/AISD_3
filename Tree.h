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

		bool contain(T key) { // ���� ����� ���������� true, ���� ���� key ��������� � �������, ����� false.
			for (int i = 0; i < size; ++i)
				if (keys[i] == key) return true;
			return false;
		}

		void swap(T& x, T& y) {
			T r = x;
			x = y;
			y = r;
		}

		void sort2(T& x, T& y) {
			if (x > y) swap(x, y);
		}

		void sort3(T& x, T& y, T& z) {
			if (x > y) swap(x, y);
			if (x > z) swap(x, z);
			if (y > z) swap(y, z);
		}

		void sort() { // ����� � �������� ������ ���� �������������
			if (size == 1) return;
			if (size == 2) sort2(keys[0], keys[1]);
			if (size == 3) sort3(keys[0], keys[1], keys[2]);
		}

		void insertToNode(T key) {  // ��������� ���� key � ������� (�� � ������)
			keys[size] = key;
			size++;
			sort();
		}

		void removeFromNode(T key) { // ������� ���� key �� ������� (�� �� ������)
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

		void becomeNode2(T key, Node* f, Node* s) {  // ������������� � 2-�������.
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

	int getCounter() {
		return counter;
	}

	int getPrevNodeCounter() {
		return prevNodeCounter;
	}

	void resetPrevNodeCounter() {
		prevNodeCounter = 0;
	}

	bool isEmpty() {
		return counter == 0;
	}

	Tree<T>& operator=(const Tree<T>& other);

	// �������� �������
	void print(Node* root, int level = 0, int x = 50, int y = 0, int c = 0) { // c = 0 - ������, c = 1 - first, c = 2 - second, � = 3 - third
		if (root) {
			if (level == 0) system("cls");

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

			prevNodeCounter++;
			level++;
			if (root->first) print(root->first, level, x, y, 1);
			if (root->second) print(root->second, level, x, y, 2);
			if (root->third) print(root->third, level, x, y, 3);
		}
	}

	// ������� � ������
	Node* insert(Node* p, T key) { // ������� ����� key � ������ � ������ p; ������ ���������� ������ ������, �.�. �� ����� ��������
		prevNodeCounter++;
		if (counter == 0) {
			root = (Node*)malloc(sizeof(Node)); // ���� ������ �����, �� ������� ������ 2-3-������� (������)
			root->keys[0] = key;
			root->first = nullptr;
			root->second = nullptr;
			root->third = nullptr;
			root->fourth = nullptr;
			root->parent = nullptr;
			root->size = 1;
			counter++;
			return root;
		}
		else if (!p) {
			Node* newNode = (Node*)malloc(sizeof(Node)); // ������� ����� �������
			newNode->keys[0] = key;
			newNode->first = nullptr;
			newNode->second = nullptr;
			newNode->third = nullptr;
			newNode->fourth = nullptr;
			newNode->parent = nullptr;
			newNode->size = 1;
			counter++;
			return newNode;
		}

		if (p && p->isLeaf()) {
			p->insertToNode(key);
			counter++;
		}
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
		prevNodeCounter++;

		Node* y = (Node*)malloc(sizeof(Node));
		y->keys[0] = elem->keys[2];
		y->first = elem->third;
		y->second = elem->fourth;
		y->third = nullptr;
		y->fourth = nullptr;
		y->parent = elem->parent;
		y->size = 1;
		prevNodeCounter++;

		if (x->first)  x->first->parent = x;    // ��������� ������������� "��������" "�������".
		if (x->second) x->second->parent = x;   // ����� ����������, "���������" "�������" �������� "�������",
		if (y->first)  y->first->parent = y;    // ������� ����� ��������� ���������� ���������.
		if (y->second) y->second->parent = y;
		
		prevNodeCounter++;
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
			free(elem);
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
	Node* search(Node* p, T key) { // ����� ����� key � 2-3 ������ � ������ p.
		prevNodeCounter++;
		if (!p) return nullptr;

		if (p->contain(key)) return p;
		else if (key < p->keys[0]) return search(p->first, key);
		else if ((p->size == 2) && (key < p->keys[1]) || (p->size == 1)) return search(p->second, key);
		else if (p->size == 2) return search(p->third, key);
	}

	// ����� ������������ �������� � ���������
	Node* searchMin(Node* p) { // ����� ���� � ����������� ��������� � 2-3-������ � ������ p.
		prevNodeCounter++;
		if (!p) return p;
		if (!(p->first)) return p;
		else return searchMin(p->first);
	}

	// �������� �� ������
	Node* remove(Node* p, T key) { // �������� ����� key � 2-3-������ � ������ p.
		Node* elem = search(p, key); // ���� ����, ��� ��������� ���� key
		
		prevNodeCounter++;
		if (!elem) return p;

		counter--;
		Node* min = nullptr;
		if (elem->keys[0] == key) min = searchMin(elem->second); // ���� ������������� ����
		else min = searchMin(elem->third);

		if (min) { // ������ ����� �������
			T& z = (key == elem->keys[0] ? elem->keys[0] : elem->keys[1]);
			elem->swap(z, min->keys[0]);
			elem = min; // ���������� ��������� �� ����, �.�. min - ������ ����
		}

		elem->removeFromNode(key); // � ������� ��������� ���� �� �����
		return fix(elem); // �������� ������� ��� �������������� ������� ������.
	}

	// ������������ ����� �������� ��� ����������� ������� ������ (���������� merge ��� redistribute) 
	Node* fix(Node* leaf) {
		prevNodeCounter++;
		if (leaf->size == 0 && leaf->parent == nullptr) { // ������ 0, ����� ������� ������������ ���� � �������
			free(leaf);
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
		prevNodeCounter++;
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
		prevNodeCounter++;
		Node* parent = leaf->parent;

		if (parent->first == leaf) {
			parent->second->insertToNode(parent->keys[0]);
			parent->second->third = parent->second->second;
			parent->second->second = parent->second->first;

			if (leaf->first != nullptr) parent->second->first = leaf->first;
			else if (leaf->second != nullptr) parent->second->first = leaf->second;

			if (parent->second->first != nullptr) parent->second->first->parent = parent->second;

			parent->removeFromNode(parent->keys[0]);
			free(parent->first);
			parent->first = nullptr;
		}
		else if (parent->second == leaf) {
			parent->first->insertToNode(parent->keys[0]);

			if (leaf->first != nullptr) parent->first->third = leaf->first;
			else if (leaf->second != nullptr) parent->first->third = leaf->second;

			if (parent->first->third != nullptr) parent->first->third->parent = parent->first;

			parent->removeFromNode(parent->keys[0]);
			free(parent->second);
			parent->second = nullptr;
		}

		if (parent->parent == nullptr) {
			Node* temp = nullptr;
			if (parent->first != nullptr) temp = parent->first;
			else temp = parent->second;
			temp->parent = nullptr;
			free(parent);
			return temp;
		}
		return parent;
	}

	void clear(Node* node) {
		if (node == root) root = nullptr;
		if (node) {
			prevNodeCounter++;
			clear(node->first);
			clear(node->second);
			clear(node->third);
			counter--;
			if (node->size == 2) counter--;
			free(node);
			node = nullptr;
		}
		//if (counter == 0) root = nullptr;
	}

	void copy(Node* node) {
		if (node) {
			prevNodeCounter++;
			insert(this->root, node->keys[0]);
			if (node->size == 2) insert(this->root, node->keys[1]);
			if (node->first) copy(node->first);
			if (node->second) copy(node->second);
			if (node->third) copy(node->third);
		}
	}

	class Iterator // ���������� ����� - ��������
	{
	private:
		Node* cur = nullptr;	// ������� ����
		T key;
		Tree<T>* tree;
		int counter = 1;
	public:
		Iterator(Tree* tree) { // ����������� ���������
			this->tree = tree;
		};
		Iterator begin(Node* node) { // ��������� �� ������ �������� � �������
			if (node->first)
				begin(node->first);
			else {
				cur = node;
				key = node->keys[0];
			}
			counter = 1;
			return *this;
		}
		Iterator end(Node* node) { // ��������� �� ��������� �������� � �������
			if (node->third)
				end(node->third);
			else if (node->second)
				end(node->second);
			else {
				cur = node;
				if (node->size == 2) key = node->keys[1];
				else key = node->keys[0];
			}
			counter = tree->counter;
			return *this;
		}
		Iterator& operator++(int) { // ���������� ����������� ������
			if (tree->counter != counter) {
				if (cur->isLeaf()) {
					if (cur == cur->parent->first) {
						if (cur->size == 1 || key == cur->keys[1]) {
							cur = cur->parent;
							key = cur->keys[0];
							return *this;
						}
					}
					if (cur->size == 2 && key == cur->keys[0]) {
						key = cur->keys[1];
					}
					else {
						if ((cur == cur->parent->second && !cur->parent->third) || cur == cur->parent->third) {
							while (cur->parent) cur = cur->parent;
							if (key > cur->keys[0] && cur->size == 2)
								key = cur->keys[1];
							else
								key = cur->keys[0];
						}
						else if (cur == cur->parent->second && cur->parent->third) {
							cur = cur->parent;
							key = cur->keys[1];
						}
					}
				}
				else {
					if (key == cur->keys[1] && cur->size == 2) {
						cur = cur->third;
					}
					else {
						cur = cur->second;
					}
					while (cur->first) cur = cur->first;
					key = cur->keys[0];
				}
				counter++;
			}
			else
				begin(tree->root);
			return *this;
		};

		Iterator& operator--(int) { // ���������� ����������� �����
			if (tree->counter > 0) {
				if (cur->isLeaf()) {
					if (cur == cur->parent->third || cur == cur->parent->second) {
						if (cur->size == 1 || key == cur->keys[0]) {
							if (cur->parent->size == 2 && cur == cur->parent->third) {
								cur = cur->parent;
								key = cur->keys[1];
							}
							else {
								cur = cur->parent;
								key = cur->keys[0];
							}
							return *this;
						}
					}
					if (cur->size == 2 && key == cur->keys[1]) {
						key = cur->keys[0];
					}
					else {
						if ((cur == cur->parent->second && !cur->parent->third) || cur == cur->parent->third) {
							cur = cur->parent;
							if (key > cur->keys[0])
								key = cur->keys[1];
							else
								key = cur->keys[0];
						}
						else if (cur == cur->parent->first) {
							while (cur->parent) cur = cur->parent;
							if (key < cur->keys[1]) key = cur->keys[0];
							else key = cur->keys[1];
						}
					}
				}
				else {
					if (key == cur->keys[1] && cur->size == 2) {
						cur = cur->second;
					}
					else {
						cur = cur->first;
					}
					while (cur->third) cur = cur->third;
					while (cur->second) cur = cur->second;
					if (cur->size == 2) key = cur->keys[1];
					else key = cur->keys[0];
				}
				counter--;
			}
			else
				end(tree->root);
			return *this;
		};
		T& operator*() { // ���������� ������� �� ������/������
			return key;
		}

		bool operator==(const Iterator&); // ���������� ���������
		bool operator!=(const Iterator&); // ���������� �����������
	};
	
	class ReverseIterator // ���������� ����� - ��������
	{
	private:
		Node* cur = nullptr;	// ������� ������
		Tree<T>* tree;
		T key;
		int counter = 1;
	public:
		ReverseIterator(Tree* tree) { // ����������� ���������
			this->tree = tree;
		};
		ReverseIterator rbegin(Node* node) { // ��������� �� ��������� �������� � �������
			if (node->third)
				rbegin(node->third);
			else if (node->second)
				rbegin(node->second);
			else {
				cur = node;
				if (node->size == 2) key = node->keys[1];
				else key = node->keys[0];
			}
			counter = tree->counter;
			return *this;
		}
		ReverseIterator rend(Node* node) { // ��������� �� ������ �������� � �������
			if (node->first)
				rend(node->first);
			else {
				cur = node;
				key = node->keys[0];
			}
			counter = 1;
			return *this;
		}
		ReverseIterator& operator--(int) { // ���������� ����������� �����
			if (tree->counter != counter) {
				if (cur->isLeaf()) {
					if (cur == cur->parent->first) {
						if (cur->size == 1 || key == cur->keys[1]) {
							cur = cur->parent;
							key = cur->keys[0];
							return *this;
						}
					}
					if (cur->size == 2 && key == cur->keys[0]) {
						key = cur->keys[1];
					}
					else {
						if ((cur == cur->parent->second && !cur->parent->third) || cur == cur->parent->third) {
							while(cur->parent) cur = cur->parent;
							if (key > cur->keys[0])
								key = cur->keys[1];
							else
								key = cur->keys[0];
						}
						else if (cur == cur->parent->second && cur->parent->third) {
							cur = cur->parent;
							key = cur->keys[1];
						}
					}
				}
				else {
					if (key == cur->keys[1] && cur->size == 2) {
						cur = cur->third;
					}
					else {
						cur = cur->second;
					}
					while (cur->first) cur = cur->first;
					key = cur->keys[0];
				}
				counter++;
			}
			else
				rend(tree->root);
			return *this;
		};

		ReverseIterator& operator++(int) { // ���������� ����������� �����
			if (tree->counter > 0) {
				if (cur->isLeaf()) {
					if (cur == cur->parent->third || cur == cur->parent->second) {
						if (cur->size == 1 || key == cur->keys[0]) {
							if (cur->parent->size == 2 && cur == cur->parent->third) {
								cur = cur->parent;
								key = cur->keys[1];
							}
							else {
								cur = cur->parent;
								key = cur->keys[0];
							}
							return *this;
						}
					}
					if (cur->size == 2 && key == cur->keys[1]) {
						key = cur->keys[0];
					}
					else {
						if ((cur == cur->parent->second && !cur->parent->third) || cur == cur->parent->third) {
							cur = cur->parent;
							if (key > cur->keys[0])
								key = cur->keys[1];
							else
								key = cur->keys[0];
						}
						else if (cur == cur->parent->first) {
							while (cur->parent) cur = cur->parent;
							if (key < cur->keys[1]) key = cur->keys[0];
							else key = cur->keys[1];
						}
					}
				}
				else {
					if (key == cur->keys[1] && cur->size == 2) {
						cur = cur->second;
					}
					else {
						cur = cur->first;
					}
					while (cur->third) cur = cur->third;
					while (cur->second) cur = cur->second;
					if (cur->size == 2) key = cur->keys[1];
					else key = cur->keys[0];
				}
				counter--;
			}
			else
				rbegin(tree->root);
			return *this;
		};
		T& operator*() { // ���������� ������� �� ������/������
			return key;
		}

		bool operator==(const ReverseIterator&); // ���������� ���������
		bool operator!=(const ReverseIterator&); // ���������� �����������
	};

protected:
	Node* root = nullptr;	// ������ ������
	int counter = 0;
	int prevNodeCounter = 0;
};

template <typename T>
Tree<T>::Tree() {
	root = nullptr;
	counter = 0;
}

template <typename T>
Tree<T>::Tree(int size) {
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		T key = rand() % 100 - 50;
		Node* temp = search(root, key);
		while (temp && (temp->keys[0] == key || temp->keys[1] == key)) {
			key = rand() % 100 - 50;
		}
		root = insert(root, key);
	}
}

template<typename T>
Tree<T>::Tree(const Tree<T>& other) {	
	copy(other.root);
	prevNodeCounter = other.prevNodeCounter;
	//counter = other.counter;
}

template <typename T>
Tree<T>::~Tree() {
	clear(getRoot());
}

template<typename T>
Tree<T>& Tree<T>::operator=(const Tree<T>& other) {
	clear(root);
	copy(other.root);
	prevNodeCounter = other.prevNodeCounter;
	//counter = other.counter;
	return *this;
}

template<typename T>
bool Tree<T>::Iterator::operator==(const Iterator& i) {
	return this.cur == i.cur;
}

template<typename T>
bool Tree<T>::Iterator::operator!=(const Iterator& i) {
	return this.cur != i.cur;
}

template<typename T>
bool Tree<T>::ReverseIterator::operator==(const ReverseIterator& i) {
	return this.cur == i.cur;
}

template<typename T>
bool Tree<T>::ReverseIterator::operator!=(const ReverseIterator& i) {
	return this.cur != i.cur;
}