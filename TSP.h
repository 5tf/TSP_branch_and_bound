#pragma once
#include "stdafx.h"

class TSP
{
private:

	int* solution;
	int bestResult;
	int numberOfCities;
	int** matrixOfDistance;
	vector <vector<int>> edges;

	void toMatrix(vector <vector<int>>);

	int* cities;

	int* possibleSolution;
	bool* visited;

	void toSolution();
	int minLine(int);
	void treeSearch(int, int, int);

public:

	//void test(int);

	void load();
	void bruteForce();
	void branchAndBound();
	void clear();
};