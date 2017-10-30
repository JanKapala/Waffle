//Kolejnosc indeksow: [wiersz][kolumna]

#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <cstdio>
#include <ctime>

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
	waffle(const waffle &init);
	void random(int percent);
	void standard_cover();
	void free_cover();
	void print();
	void print(int level);
	int how_much_free();
	
	~waffle();
};

waffle::waffle(int L)
{
	size = L;
	amount_of_inserted = 0;
	cell = new state*[L];
	for (int i = 0; i < L; i++) cell[i] = new state[L];
}

waffle::waffle(const waffle &init)
{
	size = init.size;
	amount_of_inserted = init.amount_of_inserted;
	cell = new state*[init.size];
	for (int i = 0; i < init.size; i++) cell[i] = new state[init.size];
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cell[i][j] = init.cell[i][j];
		}
	}
}

waffle::~waffle()
{
	for (int i = 0; i < size; i++) delete[] this->cell[i];
	delete[] this->cell;
}

void waffle::print()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (cell[i][j] == FREE) cout << char(176);//puste pole
			else if (cell[i][j] == BLOCKED) cout << char(178);//zablokowane pole
			else if (cell[i][j] == RIGHT) cout << 'R';//poziomy klocek
			else if (cell[i][j] == LEFT) cout << 'L';//poziomy klocek
			else if (cell[i][j] == UP) cout << 'U';//pionowy klocek
			else if (cell[i][j] == DOWN) cout << 'D';//pionowyy klocek
			cout << " ";
		}
		cout << endl;
	}
}

void waffle::print(int level)
{
	for (int i = 0; i < size; i++)
	{
		for (int a = 0; a < level; a++) cout << "	";
		for (int j = 0; j < size; j++)
		{
			if (cell[i][j] == FREE) cout << char(176);//puste pole
			else if (cell[i][j] == BLOCKED) cout << char(178);//zablokowane pole
			else if (cell[i][j] == RIGHT) cout << 'R';//poziomy klocek
			else if (cell[i][j] == LEFT) cout << 'L';//poziomy klocek
			else if (cell[i][j] == UP) cout << 'U';//pionowy klocek
			else if (cell[i][j] == DOWN) cout << 'D';//pionowyy klocek
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

void write_a_to_b(waffle* a, waffle* b)
{
	if (a->size != b->size)
	{
		cout << "ERROR: funkcja write_a_to_b: rozne rozmiary tablic argumentow a i b" << endl;
		return;
	}
	for (int i = 0; i < a->size; i++)
	{
		for (int j = 0; j < a->size; j++)
		{
			b->cell[i][j] = a->cell[i][j];
		}
	}
}

void recur(int i, int j, waffle* input, waffle* propagation_map_pointer, waffle copy, int* result_pointer, bool* finish,int level=0)
{
	//WARUNEK NIEODWIEDZONEGO POLA
	//Jezeli propagacja dotarla do tego pola to nie wywolujemy tutaj rekurencji
	if (propagation_map_pointer->cell[i][j] == BLOCKED) return;

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
		if_inserted = true;
	}

	//Sprawdzenie na podstawie flagi czy wstawienie zostalo wykonane, jesli tak to rekurencja sie konczy(zamykane jest drzewo wywolan):
	if (if_inserted)
	{
		(*result_pointer)++;
		write_a_to_b(&copy, input);
		*finish = true;
		return;
	}

	//Jesli nic nie wstawiono to:
	//DODANIE AKTUALNEGO POLA DO MAPY PROPAGACJI
	propagation_map_pointer->cell[i][j] = BLOCKED;

	//Wydruk kontrolny
	//for (int a = 0; a < level; a++) cout << "	";
	//cout << "Mapa propagacji:" << endl;
	//propagation_map_pointer->print(level);
	//cout << endl;
	//for (int a = 0; a < level; a++) cout << "	";
	//cout << "Mapa stanu tej kopii:" << endl;
	//copy.print(level);

	//Skoro kazde sasiadujace pole jest zajete(lub zablokowane lub nie istnieje) to rozpatrujemy wszystkie mozliwosci przesuniecia sasiednich klockow
	//ROZPATRYWNIE PRZESUNIEC

	//pole na gorze
	if (i - 1 >= 0 && !(*finish))
	{
		waffle new_copy = copy;
		switch (new_copy.cell[i - 1][j])
		{
		case UP:
			new_copy.cell[i][j] = UP;
			new_copy.cell[i - 1][j] = DOWN;
			new_copy.cell[i - 2][j] = FREE;
			recur(i - 2, j, input, propagation_map_pointer, new_copy, result_pointer, finish, level + 1);
			break;
		case RIGHT:
			new_copy.cell[i][j] = UP;
			new_copy.cell[i - 1][j] = DOWN;
			new_copy.cell[i - 1][j + 1] = FREE;
			recur(i - 1, j + 1, input, propagation_map_pointer, new_copy, result_pointer, finish, level + 1);
			break;
		case LEFT:
			new_copy.cell[i][j] = UP;
			new_copy.cell[i - 1][j] = DOWN;
			new_copy.cell[i - 1][j - 1] = FREE;
			recur(i - 1, j - 1, input, propagation_map_pointer, new_copy, result_pointer, finish, level + 1);
		}
	}

	//pole na dole
	if (i + 1 < copy.size && !(*finish))
	{
		waffle new_copy(copy);
		switch (new_copy.cell[i + 1][j])
		{
		case DOWN:
			new_copy.cell[i][j] = DOWN;
			new_copy.cell[i + 1][j] = UP;
			new_copy.cell[i + 2][j] = FREE;
			recur(i + 2, j, input, propagation_map_pointer, new_copy, result_pointer, finish, level + 1);
			break;
		case RIGHT:
			new_copy.cell[i][j] = DOWN;
			new_copy.cell[i + 1][j] = UP;
			new_copy.cell[i + 1][j + 1] = FREE;
			recur(i + 1, j + 1, input, propagation_map_pointer, new_copy, result_pointer, finish, level + 1);
			break;
		case LEFT:
			new_copy.cell[i][j] = DOWN;
			new_copy.cell[i + 1][j] = UP;
			new_copy.cell[i + 1][j - 1] = FREE;
			recur(i + 1, j - 1, input, propagation_map_pointer, new_copy, result_pointer, finish, level + 1);
		}
	}

	//pole na lewo
	if (j - 1 >= 0 && !(*finish))
	{
		waffle new_copy(copy);
		switch (new_copy.cell[i][j-1])
		{
		case LEFT:
			new_copy.cell[i][j] = LEFT;
			new_copy.cell[i][j-1] = RIGHT;
			new_copy.cell[i][j-2] = FREE;
			recur(i, j - 2, input, propagation_map_pointer, new_copy, result_pointer, finish, level + 1);
			break;
		case UP:
			new_copy.cell[i][j] = LEFT;
			new_copy.cell[i][j - 1] = RIGHT;
			new_copy.cell[i - 1][j - 1] = FREE;
			recur(i - 1, j - 1, input, propagation_map_pointer, new_copy, result_pointer, finish, level + 1);
			break;
		case DOWN:
			new_copy.cell[i][j] = LEFT;
			new_copy.cell[i][j - 1] = RIGHT;
			new_copy.cell[i + 1][j - 1] = FREE;
			recur(i + 1, j - 1, input, propagation_map_pointer, new_copy, result_pointer, finish, level + 1);
		}
	}

	//pole na prawo
	if (j + 1 < copy.size && !(*finish))
	{
		waffle new_copy(copy);
		switch (new_copy.cell[i][j + 1])
		{
		case RIGHT:
			new_copy.cell[i][j] = RIGHT;
			new_copy.cell[i][j + 1] = LEFT;
			new_copy.cell[i][j + 2] = FREE;
			recur(i, j + 2, input, propagation_map_pointer, new_copy, result_pointer, finish, level+1);
			break;
		case UP:
			new_copy.cell[i][j] = RIGHT;
			new_copy.cell[i][j + 1] = LEFT;
			new_copy.cell[i - 1][j + 1] = FREE;
			recur(i - 1, j + 1, input, propagation_map_pointer, new_copy, result_pointer, finish, level + 1);
			break;
		case DOWN:
			new_copy.cell[i][j] = RIGHT;
			new_copy.cell[i][j + 1] = LEFT;
			new_copy.cell[i + 1][j + 1] = FREE;
			recur(i + 1, j + 1, input, propagation_map_pointer, new_copy, result_pointer, finish, level + 1);
		}
	}
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
				bool finish = false;
				recur(i, j, input, &propagation_map, (*input), &result, &finish);
			}
		}
	}
	return result;
}

int main()
{
	srand(time(NULL));

	/*
	waffle wafelek(8);
	wafelek.random(30);

	wafelek.standard_cover();
	cout << "Liczba wstawionych wafelkow: " << wafelek.amount_of_inserted << endl;
	wafelek.print();

	cout << "Liczba wstawionych wafelkow po procesie upychania: " << solve(&wafelek) << endl;
	wafelek.print();
	cout << "Done!" << endl;
	*/
	
	cout << "Generowanie danych wejsciowych rozpoczete" << endl;
	int iterations1 = 70000;
	waffle** tab1 = new waffle*[iterations1];
	for (int i = 0; i < iterations1; i++)
	{
		tab1[i] = new waffle(5);
		tab1[i]->random(30);
		tab1[i]->standard_cover();
	}

	int iterations2 = 1;
	waffle** tab2 = new waffle*[iterations2];
	for (int i = 0; i < iterations2; i++)
	{
		tab2[i] = new waffle(100);
		tab2[i]->random(30);
		tab2[i]->standard_cover();
	}
	cout << "Generowanie danych wejsciowych zakonczone" << endl;

	cout << "Rozwiazywanie przypadkow poprawnosciowych rozpoczete:" << endl;
	clock_t start = clock();
	for (int i = 0; i < iterations1; i++) solve(tab1[i]);
	cout << "Czas wykonania to: " << clock() - start << endl;

	cout << "Rozwiazywanie przypadkow wydajnosciowych rozpoczete:" << endl;
	start = clock();
	for (int i = 0; i < iterations2; i++) solve(tab2[i]);
	cout << "Czas wykonania to: " << clock() - start << endl;

	getchar();
	return 0;
}