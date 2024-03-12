#include <iostream>
#include <fstream>
#include <new>
#include <cstdlib>
#include <string>

using namespace std;

template <class T>
class PGM
{
protected:
	T** board;
	T numrows;
	T numcols;
	T max;
	string magic;
	string comment;
	void readInput(ifstream&);
	void allocateMemory();
	void deallocateMemory();

public:
	// for file exceptions
	class exceptionClass;
	// default constructor 
	PGM();
	// one-arg constructor 
	PGM(ifstream&);
	// destructor
	~PGM();
	// overloaded subtraction operator
	PGM<T> operator-(const PGM<T>&);
	// copy constructor
	PGM<T>(const PGM<T>&);
	// move constructor
	PGM<T>(PGM<T>&&);
	// overloaded assignment operator
	PGM<T>& operator=(const PGM<T>&);
	// move assignment operator
	PGM<T>& operator=(PGM<T>&&);
	// template friend function for saving PGM object
	// file
	template <class U>
	friend void saveToPGM(const PGM<U>&, const char*);
};

template <class T>
class PGM<T>::exceptionClass
{
private:
	const char* error;
public:
	exceptionClass(const char* e) : error(e)
	{
		throw e;
	}
	const char* returnError()
	{
		return error;
	}
};

template <class T>
PGM<T>::PGM() : board(nullptr), numrows(0), numcols(0), max(0), magic(""), comment("")
{}

template<class T>
PGM<T> :: PGM(ifstream& infile)
{
	readInput(infile);
}

template <class T>
PGM<T>::PGM(const PGM<T>& pgm)
{
	numrows = pgm.numrows;
	numcols = pgm.numcols;
	max = pgm.max;
	magic = pgm.magic;
	comment = pgm.comment;
	allocateMemory();
	for (unsigned int i = 0; i < numrows; i++)
	{
		for (unsigned int j = 0; j < numcols; j++)
		{
			board[i][j] = pgm.board[i][j];
		}
	}
}

template <class T>
PGM<T>::PGM(PGM<T>&& pgm)
{
	board = pgm.board;
	pgm.board = nullptr;
	numrows = pgm.numrows;
	pgm.numrows = 0;
	numcols = pgm.numcols;
	pgm.numcols = 0;
	max = pgm.max;
	pgm.max = 0;
	magic = pgm.magic;
	comment = pgm.comment;
	pgm.magic = "";
	pgm.comment = "";
}

template <class T>
PGM<T>& PGM<T>::operator=(const PGM<T>& pgm)
{
	if (this != &pgm)
	{
		if (pgm.board != nullptr)
		{
			numrows = pgm.numrows;
			numcols = pgm.numcols;
			max = pgm.max;
			allocateMemory();
			magic = pgm.magic;
			comment = pgm.comment;
			for (unsigned int i = 0; i < numrows; i++)
			{
				for (unsigned int j = 0; j < numcols; j++)
				{
					board[i][j] = pgm.board[i][j];
				}
			}

		}
	}
	return (*this);
}

template <class T>
PGM<T>& PGM<T>::operator=(PGM<T>&& pgm)
{
	if (this != &pgm)
	{
		swap(board, pgm.board);
		swap(numrows, pgm.numrows);
		swap(numcols, pgm.numcols);
		swap(max, pgm.max);
	}
	return (*this);
}

template <class T>
void PGM<T>::allocateMemory()
{
	board = new T * [numrows];
	for (unsigned int i = 0; i < numrows; i++)
	{
		board[i] = new T[numcols];
	}
}

template <class T> // why do we have to have this line before every implementation???
void PGM<T>::deallocateMemory() // the the <T> in PGM<T> is used to specify the data type, isn't it???
{
	if (board != nullptr)
	{
		for (unsigned int i = 0; i < numrows; i++)
		{
			delete[] board[i];
			board[i] = nullptr;
		}
		delete[] board;
		board = nullptr;
	}
}

template<class T>
void PGM<T>::readInput(ifstream& file)
{
	if (file.is_open())
	{
		getline(file, magic);
		getline(file, comment);
		file >> numcols >> numrows >> max;
		this->allocateMemory();
		for (unsigned int i = 0; i < numrows; i++)
		{
			for (unsigned int j = 0; j < numcols; j++)
			{
				file >> board[i][j];
			}
		}
	}
	else
		throw exceptionClass("the file is not open yet");
}


template <class T>
PGM<T> PGM<T>::operator-(const PGM<T>& pgm)
{
	PGM<T> temp;
	if (numrows == pgm.numrows && numcols == pgm.numcols)
	{
		temp.numrows = numrows;
		temp.numcols = numcols;
		temp.max = max;
		for (unsigned int i = 0; i < numrows; i++)
		{
			for (unsigned int j = 0; j < numcols; j++)
			{
				temp.board[i][j] = pgm.board[i][j] - board[i][j];
			}
		}
	}
	
	return temp;
}

template <class T>
PGM<T>::PGM() : 
{}