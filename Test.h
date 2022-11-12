#pragma once
#include <time.h>
#include <math.h>
#include <iostream>
#include "Tree.h"

using namespace std;
typedef unsigned long long INT_64;

// ���������� � ��������� ���������� LineRand()
static INT_64 RRand = 15750;
const INT_64 mRand = (1 << 63) - 1;
const INT_64 aRand = 6364136223846793005;
const INT_64 cRand = 1442695040888963407;

// ������� ��������� ������� ���������� ����� �� ����� ����������
void sRand() { srand(time(0)); RRand = (INT_64)rand(); }

// ������� ��������� ���������� ����� �������� ������������ ��������� Xi+1=(a*Xi+c)%m
INT_64 LineRand() {
	INT_64 y1, y2;
	y1 = (aRand * RRand + cRand) % mRand;
	y2 = (aRand * y1 + cRand) % mRand;
	RRand = y1 & 0xFFFFFFFF00000000LL ^ (y2 & 0xFFFFFFFF00000000LL) >> 32;
	return RRand;
}

void test_rand(int n) {
	// �������� ������ ��� 64 � ��������� ������ ���� INT_64
	Tree<INT_64> tree;
	// ������ ��� ������, ������� ������������ � ������
	INT_64* m = new INT_64[n];
	// ��������� ������� ���������� �����
	sRand();
	// ���������� ������ � ������� ���������� �� ���������� �������
	for (int i = 0; i < n; i++) {
		m[i] = LineRand();
		tree.insert(tree.getRoot(), m[i]);
	}
	// ����� ������� ������ �� �����
	cout << "items count: " << tree.getCounter() << endl;
	// ��������� ��������� ����������� �������, �������� � ������
	double I = 0;
	double D = 0;
	double S = 0;
	// ��������� ������ ��������, 10% - ������� ��������
	for (int i = 0; i < n / 2; i++)
		if (i % 10 == 0) //10% ��������
		{
			tree.resetPrevNodeCounter();
			tree.remove(tree.getRoot(), LineRand());
			D += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
			tree.insert(tree.getRoot(), m[rand() % n]);
			I += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
			tree.search(tree.getRoot(), LineRand());
			S += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
		}
		else //90% �������� ��������
		{
			int ind = rand() % n;
			tree.resetPrevNodeCounter();
			tree.remove(tree.getRoot(), m[ind]);
			D += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
			INT_64 key = LineRand();
			tree.insert(tree.getRoot(), key);
			I += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
			m[ind] = key;
			tree.search(tree.getRoot(), m[rand() % n]);
			S += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
		} //����� �����
	// ����� �����������:
	// ����� ������� ������ ����� �����
	cout << "items count: " << tree.getCounter() << endl;
	// ����� ������������� ������ ����������� �������� BST
	//cout << "1.39*log2(n) = " << 1.39 * (log((double)n) / log(2.0)) << endl; // ��� ������ ������
	cout << "log2(n) = " << (log((double)n) / log(2.0)) << endl; // ��� ����������������� ������ ������
	cout << "log3(n) = " << (log((double)n) / log(3.0)) << endl;
	// ����� ����������������� ������ ����������� �������
	cout << "Count insert: " << I / (n / 2) << endl;
	// ����� ����������������� ������ ����������� ��������
	cout << "Count delete: " << D / (n / 2) << endl;
	// ����� ����������������� ������ ����������� ������
	cout << "Count search: " << S / (n / 2) << endl;
	// ������������ ������ ������� m[]
	delete[] m;
}

void test_ord(int n) {
	// �������� ������ ��� 64 � ��������� ������ ���� INT_64
	Tree<INT_64> tree;
	// ������ ��� ������, ������� ������������ � ������
	INT_64* m = new INT_64[n];
	// ���������� ������ � ������� ���������� � ������������� ������� �������
	// �� ��������� [0, 10000, 20000, ... ,10000*n]
	for (int i = 0; i < n; i++) {
		m[i] = i * 10000;
		tree.insert(tree.getRoot(), m[i]);
	}
	// ����� ������� ������ �� �����
	cout << "items count: " << tree.getCounter() << endl;
	// ��������� ��������� ����������� �������, �������� � ������
	double I = 0;
	double D = 0;
	double S = 0;
	// ��������� ������� ���������� �����
	sRand();
	// ��������� ������ ��������, 10% - ������� ��������
	for (int i = 0; i < n / 2; i++)
		if (i % 10 == 0) //10% ��������
		{
			int k = LineRand() % (10000 * n);
			k = k + !(k % 2); //��������� �������� ����
			tree.resetPrevNodeCounter();
			tree.remove(tree.getRoot(), k);
			D += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
			tree.insert(tree.getRoot(), m[rand() % n]);
			I += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
			k = LineRand() % (10000 * n);
			k = k + !(k % 2); // ��������� �������� ����
			tree.search(tree.getRoot(), k);
			S += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
		}
		else //90% �������� ��������
		{
			int ind = rand() % n;
			tree.resetPrevNodeCounter();
			tree.remove(tree.getRoot(), m[ind]);
			D += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
			int k = LineRand() % (10000 * n);
			k = k + k % 2; // ��������� ������ ����
			tree.insert(tree.getRoot(), k);
			I += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
			m[ind] = k;
			tree.search(tree.getRoot(), m[rand() % n]);
			S += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
		}
	//����� �����������:
	// ����� ������� ������ ����� �����
	cout << "items count: " << tree.getCounter() << endl;
	// ����� ������������� ������ ����������� �������� BST
	cout << "n/2 = " << n / 2 << endl;
	// ����� ����������������� ������ ����������� �������
	cout << "Count insert: " << I / (n / 2) << endl;
	// ����� ����������������� ������ ����������� ��������
	cout << "Count delete: " << D / (n / 2) << endl;
	// ����� ����������������� ������ ����������� ������
	cout << "Count search: " << S / (n / 2) << endl;
	// ������������ ������ ������� m[]
	delete[] m;
}