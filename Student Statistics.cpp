//
//
//
//
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <stdlib.h>
#include <ctime>
#include <io.h>
#include <iomanip>

using std::cout;
using std::cin;
using std::string;
using std::right;
using std::setprecision;
using std::showpoint;
using std::fixed;
using std::ofstream;
using std::ifstream;
using std::setw;
using std::ios_base;

struct Student
{
	const static int SCORE_SIZE = 10;

	string name, classLevel, major;
	int testScores[SCORE_SIZE], mode[3];
	int median, numModes;
	double mean, stdDev;
};

void getStudentSet(Student students[], int size);
void getStudentInfo(Student &student);
void getStudentScores(Student &student);
void getStudentStatistics(Student &student);
int findMedian(int intArray[], int size);
int removeDuplicateInts(int baseArray[], int copyArray[], int size);
void copySortedIntArray(int baseArray[], int copyArray[], int size);
void copyIntArray(int baseArray[], int copyArray[], int size);
void sortIntArray(int intArray[], int size);
void sortBySndDev(Student students[], int size);
void swap(int &num1, int &num2);
double calcStandardDeviation(int intArray[], int size, double mean);
int findMode(int intArray[], int modeArray[], int size);
void saveStudentSet(Student students[], int size, const char *file);
void openStudentSet(Student students[], int size, const char *file);
void printMenu(const char *file);
void printStudentSet(Student students[], int size);
void printStudentInfo(Student student);

template <class T>
void swapVars(T *var1, T *var2);

int main()
{
	const int ST_SIZE = 10;
	const char *FILE = "Default.dat", *tempFile = "";
	char tempChar;
	string input;

	int choice;

	Student students[ST_SIZE];

	if (_access(FILE, 00) != 0){
		cout << "\n\n\n\n\t\t       Default Student Set not found"
			<< "\n\n\t   Press any button to start data entry for default set";
		system("pause>nul");
		system("cls");
		getStudentSet(students, ST_SIZE);
		saveStudentSet(students, ST_SIZE, FILE);
	}

	else
		openStudentSet(students, ST_SIZE, FILE);

	//cout << "\n\n";
	//system("pause");

	while (true){
		printMenu(FILE);
		cin >> choice;   cin.ignore(80, '\n');
		system("cls");
			switch (choice)
			{
			case 1: 
					getStudentSet(students, ST_SIZE);
					system("cls");
					cout << "\n\n\n\n\t\t       Enter new file name: ";
					getline(cin, input);
					FILE = input.c_str();
					cout << "\n\n\t\t       Data Saved in file " << input << "\n\n";
					system("pause");
					saveStudentSet(students, ST_SIZE, FILE);   break;

			case 2: cout << "\n\n\n\n\t\t       Enter file name to open: ";
					getline(cin, input);
					tempFile = input.c_str();
					if (_access(tempFile, 00) != 0)
						cout << "\n\n\n\n\t\t       File \"" << input << "\" not found";
					
					else{
						FILE = tempFile;
						openStudentSet(students, ST_SIZE, FILE);
						cout << "\n\n\n\n\t\t       File \"" << input << "\" is in use";

					}
					cout << "\n\n";
					system("pause");   break;

			case 3:	sortIntArray(students[0].testScores, 10);
				printStudentSet(students, ST_SIZE);  break;

			case 4: cout << "\n\n\n\n\n\t\t       Exit program? (y/n): ";
				tempChar = _getch();
				if (tempChar == 'Y' || tempChar == 'y')
					exit(0);
				break;
			default: cout << "Incorrect. Please enter a choice(1 - 4): ";
		}
	system("cls");
	}
}

void getStudentSet(Student students[], int size)
{
	for (int i = 0; i < size; i++)
	{	
		cout << "\n\n\n\n"
			 << "\t\t       Data Entry for Student #" << i + 1 << "\n";
		getStudentInfo(students[i]);
		getStudentScores(students[i]);
		getStudentStatistics(students[i]);
		system("cls");
	}

	sortBySndDev(students, size);
}

void getStudentInfo(Student &student)
{
	int temp;

	cout << "\n"
		 << "\t\t       Enter Name: ";
	getline(cin, student.name);

	cout << "\n"
		 << "\t\t       Enter Class Level\n"
		 << "       (1 - Freshman, 2 - Sophmore, 3 - Junior, 4 - Senior): ";
	cin >> temp;   cin.ignore(80, '\n');

	switch (temp)
	{
		case 1: student.classLevel = "Freshman";   break;
		case 2: student.classLevel = "Sophmore";   break;
		case 3: student.classLevel = "Junior";   break;
		case 4: student.classLevel = "Senior";   break;
		default: student.classLevel = "Freshman";   break;
	}

	cout << "\n"
		 << "\t\t       Enter Major\n"
		 << "\t       (1 - Liberal Arts, 2 - General Science): ";
	cin >> temp;   cin.ignore(80, '\n');

	student.major = (temp == 1 ? "Liberal Arts": "General Science");
}

void getStudentScores(Student &student)
{
	long seed = time(0);
	srand(seed);

	for (int i = 0; i < student.SCORE_SIZE; i++)
	{
		student.testScores[i] = rand() % 101;
	}

	sortIntArray(student.testScores, student.SCORE_SIZE);
}

void getStudentStatistics(Student &student) // Make more funcs
{
	int sum = 0;
	const int size = student.SCORE_SIZE;

	for (int i = 0; i < size; i++)
	{
		sum += student.testScores[i];
	}

	student.mean = sum / double(size);

	//

	int sortedScores[size];
	
	copySortedIntArray(student.testScores, sortedScores, size);
	student.median = findMedian(sortedScores, size);
	copySortedIntArray(student.testScores, sortedScores, size);
	student.stdDev = calcStandardDeviation(sortedScores, size, student.mean);

	student.numModes = findMode(sortedScores, student.mode, size);
	if (student.numModes == 0)
	{
		student.mode[0] = int(student.mean);
		student.numModes = 1;
	}
}

int findMedian(int intArray[], int size)
{
	int noDupes[sizeof(intArray)/4];
	int lSize = removeDuplicateInts(intArray, noDupes, size);

	if (lSize > 1)
		return (lSize % 2 == 0 ? (noDupes[lSize / 2 - 1] + noDupes[lSize / 2]) / 2 : noDupes[lSize / 2]);
	else
		return noDupes[0];
}

int removeDuplicateInts(int baseArray[], int copyArray[], int size)
{
	int j = 1;
	copyArray[0] = baseArray[0];

	for (int i = 1; i < size; i++)
	{
		if (baseArray[i] != baseArray[i - 1])
		{
			copyArray[j] = baseArray[i];
			j++;
		}
	}
	return j;
}

void copySortedIntArray(int baseArray[], int copyArray[], int size)
{
	copyIntArray(baseArray, copyArray, size);
	sortIntArray(copyArray, size);
}

void copyIntArray(int baseArray[], int copyArray[], int size)
{
	for (int i = 0; i < size; i++)
	{
		copyArray[i] = baseArray[i];
	}
}

void sortIntArray(int intArray[], int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
			if (intArray[i] > intArray[j])
				swap(intArray[i], intArray[j]);
	}
}

void sortBySndDev(Student students[], int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (students[i].stdDev > students[j].stdDev)
			{
				swapVars(students + i, students + j);
			}
		}
	}
}

void swap(int &num1, int &num2)
{
	int temp = num1;
	num1 = num2;
	num2 = temp;
}
template <class T>
void swapVars(T *var1, T *var2) {

	T temp;
	temp = *var1;
	*var1 = *var2;
	*var2 = temp;
}

double calcStandardDeviation(int intArray[], int size, double mean)
{
	double sum = 0, temp;

	for (int i = 0; i < size; i++)
	{
		temp = intArray[i] - mean;
		temp = temp * temp;
		sum += temp;
	}
	return sqrt(sum / double(size));
}

// modeArray must be size 3, intArray should be sorted
int findMode(int intArray[], int modeArray[], int size)
{
	int freqArray[10][2];
	int numCount = 1, modeCount = 0, dupeNodes = 1, highFreq = 1;

	freqArray[0][0] = intArray[0];
	freqArray[0][1] = 1;

	for (int i = 1; i < size; i++)
	{
		if (freqArray[numCount - 1][0] == intArray[i])
			freqArray[numCount - 1][1]++;
		else
		{
			freqArray[numCount][0] = intArray[i];
			freqArray[numCount][1] = 1;
			numCount++;
		}
	}

	for (int i = 0; i < numCount; i++)
	{
		if (freqArray[i][1] > highFreq)
			highFreq = freqArray[i][1];
	}


	if (freqArray[0][1] == highFreq)
	{
		modeArray[modeCount] = freqArray[0][0];
		modeCount++;
	}

	
	for (int i = 1; i < numCount; i++)
	{
		if (freqArray[i][1] == highFreq)
		{
			if (freqArray[i - 1][1] == highFreq)
			{
				modeArray[modeCount - 1] += freqArray[i][0];
				dupeNodes++;
			}

			else
			{
				if (dupeNodes > 1)
				{
					modeArray[modeCount - 1] /= dupeNodes;
					dupeNodes = 1;
				}
				modeArray[modeCount] = freqArray[i][0];
				modeCount++;
			}
		}

		else if (dupeNodes > 1)
		{
			modeArray[modeCount - 1] /= dupeNodes;
			dupeNodes = 1;
		}
	}

	if (dupeNodes > 1)
		modeArray[modeCount - 1] /= dupeNodes;

	return modeCount;
}

void saveStudentSet(Student students[], int size, const char *file)
{
	ofstream fout(file, ios_base::out | ios_base::app | ios_base::binary);

	for (int i = 0; i < size; i++)
	{
		fout.write((char *)&students[i], sizeof students[i]);
	}

	fout.close();
}

void openStudentSet(Student students[], int size, const char *file)
{
	ifstream fin(file, ios_base::in | ios_base::binary);


	for (int i = 0; i < size; i++)
	{
		fin.read((char *)&students[i], sizeof students[i]);

	}
	fin.close();
}

void printMenu(const char *file)
{
	cout << "\n\n\n\n"
		<< "\t\t       Current File: " << file << "\n"
		<< "\n                                     \n"
		<< "\t\t       1. Create new Student Set   \n"
		<< "\t\t       2. Open Student Set         \n"
		<< "\t\t       3. Display Student Info     \n"
		<< "\t\t       4. Quit                     \n"
		<< "\n\t\t       Enter your choice(1 - 3): ";
}

void printStudentSet(Student students[], int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << "\n\n\n";
		printStudentInfo(students[i]);
		system("pause");
		system("cls");
	}

	system("pause");
	system("cls");
}
void printStudentInfo(Student student)
{
	cout << setprecision(2) <<  fixed << showpoint;
	cout << "\t\t       Name:  " << student.name;
	cout << "\n\t\t       Class: " << student.classLevel;
	cout << "\n\t\t       Major: " << student.major;
	cout << "\n\n\t\t       Scores \n";
	for (int i = 0; i < student.SCORE_SIZE; i++)
	{
		cout << (i % 2 != 0 ? "\t" : "\n\t               ");
		cout << "Test " << i + 1 << ": " << student.testScores[i];
		
	}
	cout << "\n\n\t\t       Mean: " << student.mean;
	cout << "\n\n\t\t       Median: " << student.median;
	cout << "\n\n\t\t       SDV: " << student.stdDev;
	cout << "\n\n\t\t       Mode(s): ";
	for (int i = 0; i < student.numModes; i++)
	{
		cout << student.mode[i] << " ";
	}
	cout << "\n\n";
}