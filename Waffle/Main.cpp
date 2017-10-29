//Kolejnosc indeksow: [wiersz][kolumna]

#include <iostream>
#include <vector>
#include <random>
#include <time.h>

//(-1,-1) <=> wolne pole
//(-2,-2) <=> zablokowane pole
//(x,y) gdzie "x" i "y" s¹ nieujemne <=> pole zajête przez klocek

using namespace std;

enum state { BLOCKED, FREE, UP, DOWN, RIGHT, LEFT };

struct waffle
{
	state** cell;
	int size;
	int amount_of_inserted;

	waffle(int L);
	void random(int percent);
	void standard_cover();
	void free_cover();
	void print();
	int how_much_free();
};

waffle::waffle(int L)
{
	size = L;
	amount_of_inserted = 0;
	cell = new state*[L];
	for (int i = 0; i < L; i++) cell[i] = new state[L];
}

void waffle::print()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (cell[i][j] == FREE) cout << char(176);//puste pole
			else if (cell[i][j] == BLOCKED) cout << char(178);//zablokowane pole
			else if (cell[i][j] == RIGHT) cout << char(205);//poziomy klocek
			else if (cell[i][j] == LEFT) cout << char(205);//poziomy klocek
			else if (cell[i][j] == UP) cout << char(186);//pionowy klocek
			else if (cell[i][j] == DOWN) cout << char(186);//pionowyy klocek
			cout << " ";
		}
		cout << endl;
	}
}

void waffle::random(int percent)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (rand() % 100 < percent) cell[i][j] = BLOCKED;
			else cell[i][j] = FREE;
		}
	}
}

void waffle::standard_cover()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (cell[i][j] == FREE)
			{
				if (j + 1 < size && cell[i][j + 1] == FREE)
				{
					cell[i][j] = RIGHT;
					cell[i][j + 1] = LEFT;
					amount_of_inserted++;
				}
				else if (i + 1 < size && cell[i + 1][j] == FREE)
				{
					cell[i][j] = DOWN;
					cell[i + 1][j] = UP;
					amount_of_inserted++;
				}
			}
		}
	}
}

void waffle::free_cover()
{
	for (int i = 0; i < size; i++) for (int j = 0; j < size; j++) cell[i][j] = FREE;
}

int waffle::how_much_free()
{
	int result = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (cell[i][j] == FREE) result++;
		}
	}
	return result;
}


int solve(waffle* input)
{
	int result = input->amount_of_inserted;
	waffle propagation_map(input->size);

	for (int i = 0; i < input->size; i++)
	{
		for (int j = 0; j < input->size; j++)
		{
			if (input->cell[i][j] == FREE)
			{
				propagation_map.free_cover();//Inicjalizacja mapy propagacji
											 //Uwaga: w mapie propagacji sa uzywane tylko dwie mozliwe wartosci "state" w nastepujacym znaczeniu:
											 //	BLOCKED - oznacza, ze do tego pola propagacja juz dotarla i pole jest zamkniete na propagacje
											 //	FREE - oznacza, ze do tego pola propagacja jeszcze nie dotarla i jest ono na nia otwarte
											 //
				bool finish = false;

			}
		}
	}
}



void recur(int i, int j, waffle* input, waffle* propagation_map_pointer, waffle copy, int* result_pointer, bool* finish)
{
	//WARUNEK WSTAWIENIA KLOCKA I ZAMKNIECIA DRZEWA WYOWLAN:
	//Sprawdzamy czy mozna wstawic klocek na jeden z czterech sposobow tak, aby jedna jego kostka znajdowala sie w polu (i,j)

	bool if_inserted = false;//Flaga sprawdzajaca czy wstawienie zostalo wykonane

							 //pole na gorze
	if (i - 1 >= 0 && copy.cell[i - 1][j] == FREE)
	{
		copy.cell[i - 1][j] = DOWN;
		copy.cell[i][j] = UP;
		if_inserted = true;
	}
	//pole na prawo
	else if (j + 1 < copy.size && copy.cell[i][j + 1] == FREE)
	{
		copy.cell[i][j + 1] = LEFT;
		copy.cell[i][j] = RIGHT;
		(*result_pointer)++;
		if_inserted = true;
	}
	//pole na dole
	else if (i + 1 < copy.size && copy.cell[i + 1][j] == FREE)
	{
		copy.cell[i + 1][j] = UP;
		copy.cell[i][j] = DOWN;
		if_inserted = true;
	}
	//pole na lewo
	else if (j - 1 >= 0 && copy.cell[i][j - 1] == FREE)
	{
		copy.cell[i][j - 1] = RIGHT;
		copy.cell[i][j] = LEFT;
		if_inserted = true;//gestg
	}

	//Sprawdzenie na podstawie flagi czy wstawienie zostalo wykonane, jesli tak to program sie konczy:
	if (if_inserted)
	{
		(*result_pointer)++;
		*input = copy;
		*finish = true;
		return;
	}

	//Jesli nic nie wstawiono to:
	//DODANIE AKTUALNEGO POLA DO MAPY PROPAGACJI
	//Pole przekazane do tej funkcji na pewno bylo FREE, zatem nie trzeba juz tego sprawdzac i mozna je dodac do mapy propagacji
	propagation_map_pointer->cell[i][j] = BLOCKED;

	//Jezeli kazde sasiadujace pole jest zajete to rozpatrujemy wszystkie mozliwosci przesuniecia sasiednich klockow
	//POLE NA GORZE
	if (i - 1 >= 0)
	{
		if (copy.cell[i - 1][j] == UP)
		{
			copy.cell[i][j] = UP;
			copy.cell[i - 1][j] = DOWN;
			copy.cell[i - 2][j] = FREE;
		}
	}
}

int main()
{
	srand(time(NULL));
	int L = 8;

	/*
	// Create
	int L;
	typedef vector<vector<pair<int, int>>> board1;

	board1 vec(L, vector<pair<int, int>>(L));

	// Write
	vec[2][3].first = 1;
	vec[2][3].second = 2;
	// Read
	cout << vec[2][3].first << " " << vec[2][3].second;

	cout << endl << endl;
	wafel abc(4);
	*/

	waffle wafelek(8);
	wafelek.random(30);

	wafelek.standard_cover();
	wafelek.print();

	getchar();
	return 0;
}