#pragma once

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

		bool find(int key) { // ���� ����� ���������� true, ���� ���� key ��������� � �������, ����� false.
			for (int i = 0; i < size; ++i)
				if (key[i] == key) return true;
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
	Node* search(Node* p, int key); // ����� � ������
	Node* search_min(Node* p); // ����� ������������ �������� � ���������
	Node* merge(Node* leaf); // ������� ������������ ��� ��������
	Node* redistribute(Node* leaf); // ����������������� ����� ������������ ��� ��������
	Node* fix(Node* leaf); // ������������ ����� �������� ��� ����������� ������� ������ (���������� merge ��� redistribute) 
	Node* remove(Node* p, int key); // �������� �� ������

	// ������� � ������
	Node* insert(Node* p, int key) { // ������� ����� k � ������ � ������ p; ������ ���������� ������ ������, �.�. �� ����� ��������
		if (!p) {
			Node* newNode; // ���� ������ �����, �� ������� ������ 2-3-������� (������)
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
		Node* x;
		x->key[0] = elem->key[0];
		x->first = elem->first;
		x->second = elem->second;
		x->third = nullptr;
		x->fourth = nullptr;
		x->parent = elem->parent;

		Node* y;
		y->key[0] = elem->key[2];
		y->first = elem->third;
		y->second = elem->fourth;
		y->third = nullptr;
		y->fourth = nullptr;
		y->parent = elem->parent;

		if (x->first)  x->first->parent = x;    // ��������� ������������� "��������" "�������".
		if (x->second) x->second->parent = x;   // ����� ����������, "���������" "�������" �������� "�������",
		if (y->first)  y->first->parent = y;    // ������� ����� ��������� ���������� ���������.
		if (y->second) y->second->parent = y;

		if (elem->parent) {
			elem->parent->insertToNode(elem->key[1]);

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
			elem->becomeNode2(elem->key[1], x, y);
			return elem;
		}
	}

protected:
	Node* root;	// ������ ������
};

template <typename T>
Tree<T>::Tree() {
	//T keys[3];
	Node* first = nullptr;
	Node* second = nullptr;
	Node* third = nullptr;
	Node* fourth = nullptr;
	Node* parent = nullptr;
	int length = 0;
}

template <typename T>
Tree<T>::~Tree() {
}