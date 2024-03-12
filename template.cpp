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




// exception class implementation
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

// default constuctor
template <class T>
PGM<T>::PGM() : board(nullptr), numrows(0), numcols(0), max(0), magic(""), comment("")
{
	cout << "> Default constructor called" << endl;
}


// one-argument constructor
template<class T>
PGM<T> :: PGM(ifstream& infile)
{
	cout << "> One-arg constructor called" << endl;
	readInput(infile);
}

// copy constuctor 
template <class T>
PGM<T>::PGM(const PGM<T>& pgm)
{
	cout << "> Copy constructor called" << endl;
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

// destructor
template <class T>
PGM<T>::~PGM()
{
	cout << "> Destructor called" << endl;
	deallocateMemory();
}

// move constructor
template <class T>
PGM<T>::PGM(PGM<T>&& pgm)
{
	cout << "> Move constructor called" << endl;
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

// ordinary assignment operator
template <class T>
PGM<T>& PGM<T>::operator=(const PGM<T>& pgm)
{
	cout << "> Ordinary assignment operator called" << endl;
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

// move assignment operator
template <class T>
PGM<T>& PGM<T>::operator=(PGM<T>&& pgm)
{
	cout << "> Move assignment operator called" << endl;
	if (this != &pgm)
	{
		swap(board, pgm.board);
		swap(numrows, pgm.numrows);
		swap(numcols, pgm.numcols);
		swap(max, pgm.max);
		swap(magic, pgm.magic);
		swap(comment, pgm.comment);
	}
	return (*this);
}

// allocate memory function
template <class T>
void PGM<T>::allocateMemory()
{
	board = new T * [numrows];
	for (unsigned int i = 0; i < numrows; i++)
	{
		board[i] = new T[numcols];
	}
}


// deallocate memory function
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


// read input function
template<class T>
void PGM<T>::readInput(ifstream& file)
{
	cout << "> Now reading input..." << endl;
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

// function to perform subtraction between the input pgm and this pgm
template <class T>
PGM<T> PGM<T>::operator-(const PGM<T>& pgm)
{
	cout << "> Now subtracting..." << endl;
	PGM<T> temp;
	if (numrows == pgm.numrows && numcols == pgm.numcols)
	{
		temp.numrows = numrows;
		temp.numcols = numcols;
		temp.max = max;
		temp.magic = magic;
		temp.comment = comment;
		temp.allocateMemory();
		for (unsigned int i = 0; i < temp.numrows; i++)
		{
			for (unsigned int j = 0; j < temp.numcols; j++)
			{
				temp.board[i][j] = board[i][j] - pgm.board[i][j];
				if (temp.board[i][j] < 0)
					temp.board[i][j] = 0;
			}
		}
	}
	
	return temp;
}

// save to pgm function
template <class U>
void saveToPGM(const PGM<U>& pgm, const char* file)
{
	cout << "> Now saving pgm..." << endl;
	ofstream dataFile(file, ios::out);
	if (dataFile.is_open())
	{
		dataFile << pgm.magic << endl << pgm.comment << endl;
		dataFile << pgm.numrows << " " << pgm.numcols << endl;
		dataFile << pgm.max << endl;
		for (unsigned int i = 0; i < pgm.numrows; i++)
		{
			for (unsigned int j = 0; j < pgm.numcols; j++)
			{
				dataFile << pgm.board[i][j] << " ";
			}
			dataFile << endl;
		}
	}
	dataFile.close();
}

int main()
{
	ifstream file1("frame1.pgm", ios::in);
	ifstream file2("frame2.pgm", ios::in);

	PGM<short>* ptr1 = nullptr;
	PGM<short>* ptr2 = nullptr;
	PGM<short>* ptr3 = nullptr;

	try
	{
		ptr1 = new PGM<short>(file1);
		ptr2 = new PGM<short>(file2);
		ptr3 = new PGM<short>(*ptr1 - *ptr2);
	}
	catch (PGM<short>::exceptionClass& e)
	{
		cout << e.returnError() << endl;
		return 0;
	}

	saveToPGM(*ptr3, "result.pgm");

	file1.close();
	file2.close();

	delete ptr1;
	ptr1 = nullptr;
	delete ptr2;
	ptr2 = nullptr;
	delete ptr3;
	ptr3 = nullptr;

	return 0;
}