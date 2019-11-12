#include "stdafx.h"
#include "TSP.h"

void TSP::load()
{
	ifstream file;
	string filename;
	bool test;

	//procedura sprawdzenia poprawnoœci zaladowania pliku
	do
	{
		test = false;
		cout << "\nPodaj nazwe pliku z danymi." << endl;
		cin >> filename;
		file.open(filename.c_str(), ios::in);
		if (file.good() == false)
		{
			cout << "Blad wczytywania pliku.\n";
			test = true;
		}
	} while (test);

	//proces odczytu danych i zapisania ich do wektora
	string line;

	while (getline(file, line))
	{
		vector <int> lineData;
		stringstream lineStream(line);
		int value;
		while (lineStream >> value)
		{
			lineData.push_back(value);
		}
		edges.push_back(lineData);
	}
	file.close();

	//utworzenie macierzy sasiedztwa pod postacia tablicy dwuwymiarowej
	toMatrix(edges);

	//zwolnienie pamieci wektora
	vector<vector<int>>().swap(edges);
}

void TSP::toMatrix(vector <vector<int>> e)
{
	numberOfCities = e[0][0];

	matrixOfDistance = new int* [numberOfCities];

	for (int i = 0; i < numberOfCities; i++)
	{
		matrixOfDistance[i] = new int[numberOfCities];
	}

	for (int i = 0; i < numberOfCities; i++)
	{
		for (int j = 0; j < numberOfCities; j++)
		{
			matrixOfDistance[i][j] = e[i + 1][j];
		}
	}

	cities = new int[numberOfCities];
	for (int i = 0; i < numberOfCities; i++)
	{
		cities[i] = i;
	}
}

void TSP::bruteForce()
{
	bestResult = INT_MAX;
	solution = new int[numberOfCities];
	memset(solution, 0, numberOfCities);

	do
	{
		int result = 0;


		for (int i = 0; i < numberOfCities - 1; i++)
		{
			result += matrixOfDistance[cities[i]][cities[i + 1]];
		}

		result += matrixOfDistance[cities[numberOfCities - 1]][cities[0]];

		if (result < bestResult)
		{
			bestResult = result;
			for (int i = 0; i < numberOfCities; i++)
			{
				solution[i] = cities[i];
			}
		}

	} while (next_permutation(cities, cities + numberOfCities));


	for (int i = 0; i < numberOfCities; i++)
	{
		cout << solution[i] << " ";
	}
	cout << bestResult;
}

void TSP::clear()
{
	for (int i = 0; i < numberOfCities; i++)
	{
		delete[] matrixOfDistance[i];
	}

	delete[] matrixOfDistance;
	delete[] cities;
	delete[] solution;
	delete[] possibleSolution;
	delete[] visited;
}
/*
void TSP::test(int size)
{
	numberOfCities = size;
	matrixOfDistance = new int*[size];

	for (int i = 0; i < size; i++)
	{
		matrixOfDistance[i] = new int[size];
	}
	cities = new int[numberOfCities];
	for (int i = 0; i < numberOfCities; i++)
	{
		cities[i] = i;
	}

	for (int i = 0; i < 1; i++)
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				matrixOfDistance[i][j] = rand() % 100 + 1;
			}
		}

		auto start = chrono::system_clock::now();
		bruteForce();
		auto end = chrono::system_clock::now();

		chrono::duration<double> elapsed_seconds = end - start;

		cout << elapsed_seconds.count() << endl;
	}

	clear();
}*/

void TSP::branchAndBound()
{
	bestResult = INT_MAX;									//upperBound
	possibleSolution = new int[numberOfCities + 1];			//obecne rozwiazanie
	solution = new int[numberOfCities + 1];
	visited = new bool[numberOfCities];

	int lowerBound = 0;

	memset(visited, 0, numberOfCities);

	for (int i = 0; i < numberOfCities; i++)
		lowerBound += minLine(i);

	visited[0] = true;
	possibleSolution[0] = 0;

	treeSearch(lowerBound, 0, 1);

	for (int i = 0; i < numberOfCities + 1; i++)
	{
		cout << solution[i] << " ";
	}
	cout << bestResult;
}

void TSP::toSolution()
{
	for (int i = 0; i < numberOfCities; i++)
		solution[i] = possibleSolution[i];

	solution[numberOfCities] = possibleSolution[0];
}

int TSP::minLine(int l)
{
	int min = INT_MAX;

	for (int i = 0; i < numberOfCities; i++)
		if (matrixOfDistance[l][i] < min && l != i && visited[i] == false)
			min = matrixOfDistance[l][i];
	return min;
}

void TSP::treeSearch(int lowerBound, int cost, int level)
{
	if (level == numberOfCities)
	{
		if (matrixOfDistance[possibleSolution[level - 1]][possibleSolution[0]] != (-1))
		{
			int result = cost + matrixOfDistance[possibleSolution[level - 1]][possibleSolution[0]];

			if (result < bestResult)
			{
				toSolution();
				bestResult = result;
			}
		}
		return;
	}

	for (int i = 0; i <= numberOfCities; i++)
	{
		if (matrixOfDistance[possibleSolution[level - 1]][i] != (-1) && visited[i] == false)
		{
			int temp = lowerBound;
			lowerBound = 0;

			cost += matrixOfDistance[possibleSolution[level - 1]][i];

			for (int j = 1; j < numberOfCities; j++)
			{
				if (j == i)
					visited[0] = true;
				else
					visited[0] = false;

				if (visited[j] == false)
					lowerBound += minLine(j);
			}

			visited[0] = true;

			if (lowerBound + cost < bestResult)
			{
				possibleSolution[level] = i;
				visited[i] = true;

				treeSearch(lowerBound, cost, level + 1);
			}

			cost -= matrixOfDistance[possibleSolution[level - 1]][i];
			lowerBound = temp;

			memset(visited, false, numberOfCities);
			for (int j = 0; j <= level - 1; j++)
				visited[possibleSolution[j]] = true;
		}
	}

}

