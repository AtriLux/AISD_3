#pragma once
#include <time.h>
#include <math.h>
#include <iostream>
#include "Tree.h"

using namespace std;
typedef unsigned long long INT_64;

// переменная и константы генератора LineRand()
static INT_64 RRand = 15750;
const INT_64 mRand = (1 << 63) - 1;
const INT_64 aRand = 6364136223846793005;
const INT_64 cRand = 1442695040888963407;

// функция установки первого случайного числа от часов компьютера
void sRand() { srand(time(0)); RRand = (INT_64)rand(); }

// функция генерации случайного числа линейный конгруэнтный генератор Xi+1=(a*Xi+c)%m
INT_64 LineRand() {
	INT_64 y1, y2;
	y1 = (aRand * RRand + cRand) % mRand;
	y2 = (aRand * y1 + cRand) % mRand;
	RRand = y1 & 0xFFFFFFFF00000000LL ^ (y2 & 0xFFFFFFFF00000000LL) >> 32;
	return RRand;
}

void test_rand(int n) {
	// создание дерева для 64 – разрядных ключей типа INT_64
	Tree<INT_64> tree;
	// массив для ключей, которые присутствуют в дереве
	INT_64* m = new INT_64[n];
	// установка первого случайного числа
	sRand();
	// заполнение дерева и массива элементами со случайными ключами
	for (int i = 0; i < n; i++) {
		m[i] = LineRand();
		tree.insert(tree.getRoot(), m[i]);
	}
	// вывод размера дерева до теста
	cout << "items count: " << tree.getCounter() << endl;
	// обнуление счётчиков трудоёмкости вставки, удаления и поиска
	double I = 0;
	double D = 0;
	double S = 0;
	// генерация потока операций, 10% - промахи операций
	for (int i = 0; i < n / 2; i++)
		if (i % 10 == 0) //10% промахов
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
		else //90% успешных операций
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
		} //конец теста
	// вывод результатов:
	// вывод размера дерева после теста
	cout << "items count: " << tree.getCounter() << endl;
	// вывод теоретической оценки трудоёмкости операций BST
	//cout << "1.39*log2(n) = " << 1.39 * (log((double)n) / log(2.0)) << endl; // для дерева поиска
	cout << "log2(n) = " << (log((double)n) / log(2.0)) << endl; // для сбалансированного дерева поиска
	cout << "log3(n) = " << (log((double)n) / log(3.0)) << endl;
	// вывод экспериментальной оценки трудоёмкости вставки
	cout << "Count insert: " << I / (n / 2) << endl;
	// вывод экспериментальной оценки трудоёмкости удаления
	cout << "Count delete: " << D / (n / 2) << endl;
	// вывод экспериментальной оценки трудоёмкости поиска
	cout << "Count search: " << S / (n / 2) << endl;
	// освобождение памяти массива m[]
	delete[] m;
}

void test_ord(int n) {
	// создание дерева для 64 – разрядных ключей типа INT_64
	Tree<INT_64> tree;
	// массив для ключей, которые присутствуют в дереве
	INT_64* m = new INT_64[n];
	// заполнение дерева и массива элементами с возрастающими чётными ключами
	// на интервале [0, 10000, 20000, ... ,10000*n]
	for (int i = 0; i < n; i++) {
		m[i] = i * 10000;
		tree.insert(tree.getRoot(), m[i]);
	}
	// вывод размера дерева до теста
	cout << "items count: " << tree.getCounter() << endl;
	// обнуление счётчиков трудоёмкости вставки, удаления и поиска
	double I = 0;
	double D = 0;
	double S = 0;
	// установка первого случайного числа
	sRand();
	// генерация потока операций, 10% - промахи операций
	for (int i = 0; i < n / 2; i++)
		if (i % 10 == 0) //10% промахов
		{
			int k = LineRand() % (10000 * n);
			k = k + !(k % 2); //случайный нечётный ключ
			tree.resetPrevNodeCounter();
			tree.remove(tree.getRoot(), k);
			D += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
			tree.insert(tree.getRoot(), m[rand() % n]);
			I += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
			k = LineRand() % (10000 * n);
			k = k + !(k % 2); // случайный нечётный ключ
			tree.search(tree.getRoot(), k);
			S += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
		}
		else //90% успешных операций
		{
			int ind = rand() % n;
			tree.resetPrevNodeCounter();
			tree.remove(tree.getRoot(), m[ind]);
			D += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
			int k = LineRand() % (10000 * n);
			k = k + k % 2; // случайный чётный ключ
			tree.insert(tree.getRoot(), k);
			I += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
			m[ind] = k;
			tree.search(tree.getRoot(), m[rand() % n]);
			S += tree.getPrevNodeCounter();
			tree.resetPrevNodeCounter();
		}
	//вывод результатов:
	// вывод размера дерева после теста
	cout << "items count: " << tree.getCounter() << endl;
	// вывод теоретической оценки трудоёмкости операций BST
	cout << "n/2 = " << n / 2 << endl;
	// вывод экспериментальной оценки трудоёмкости вставки
	cout << "Count insert: " << I / (n / 2) << endl;
	// вывод экспериментальной оценки трудоёмкости удаления
	cout << "Count delete: " << D / (n / 2) << endl;
	// вывод экспериментальной оценки трудоёмкости поиска
	cout << "Count search: " << S / (n / 2) << endl;
	// освобождение памяти массива m[]
	delete[] m;
}