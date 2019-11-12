#include "stdafx.h"
#include "TSP.h"


int main()
{
	srand(time(NULL));
	TSP example;
	//int n = 0;
	char option;
	do {
		cout << endl;
		cout << "==== MENU ===" << endl;
		cout << "1.Wczytaj" << endl;
		cout << "2.Brute force" << endl;
		cout << "3.Branch and bound" << endl;
		cout << "4.Test" << endl;
		cout << "5.Czyœæ rozwi¹zanie" << endl;
		cout << "0.Wyjscie" << endl;
		cout << "Podaj opcje:";
		option = _getch();
		cout << endl;

		switch (option) {
		case '1':
			example.load();
			break;
		case '2':
			example.bruteForce();
			break;
		case '3':
			example.branchAndBound();
			break;
		case '4':
			//cout << "wielkosc";
			//cin >> n;
			//example.test(n);
			break;
		case '5':
			example.clear();
			break;
		}

	} while (option != '0');
	example.clear();
	return 0;
}

