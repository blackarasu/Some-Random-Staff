yt// Find your password.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <fstream>
#include <iostream>
#include <string>
#include "MemoryMapped.h"

const std::string SEPARATOR = " ";
//const unsigned int bufferSize = 35000;
const size_t oneGB = 1 << 30;
//const size_t oneGB = 1 << 10;

std::string AddSeparator(size_t line)
{
	std::string str = std::to_string(line);
	int iS = 1;
	int dlugosc = str.size();
	for (int i = 0; i<dlugosc; i++)
	{
		if (i == 3 * iS)
		{
			str.insert(dlugosc - i, SEPARATOR);
			iS++;
		}
	}
	return str;
}

/*bool findPassword(const char* fileName, const std::string &passwordToFind) //zwraca true jesli nie udalo sie znaleźć pliku lub znaleziono haslo w bazie
{
	size_t line = 1;
	std::ifstream readFile;
	readFile.open(fileName);
	if (readFile.is_open())
	{
		std::string password;
		while (!readFile.eof())
		{
			readFile >> password;
			if (passwordToFind == password)
			{
				readFile.close();
				std::cout << std::endl << "Haslo: " << passwordToFind << " istnieje w bazie "<<fileName << std::endl << "Miejsce: " << AddSeparator(line) << std::endl;
				return true;
			}
			++line;
			if (line % 500000 == 0)
			{
				std::cout << ".";
			}
		}
	}
	else
	{
		std::cout << "Nie udalo sie otworzyc pliku" << std::endl;
		return true;
	}
	std::cout << std::endl << AddSeparator(line) << std::endl;
	return false;
}*/

bool findPassword(const char* fileName, const std::string passwordToFind)//MemoryMap
{
	uint64_t loadedBytes = 0;
	size_t line = 1;
	std::string onePassword;
	MemoryMapped mmapFile(fileName);
	if (mmapFile.isValid())
	{
		//while (loadedBytes<mmapFile.size())
		//{
			char* buffer = (char*)mmapFile.getData();
			for (size_t i = 0; i < mmapFile.mappedSize(); ++i)
			{
				if (buffer[i] == '\n')
				{
					if (passwordToFind == onePassword)
					{
						mmapFile.close();
						std::cout << std::endl << "Haslo: " << passwordToFind << " istnieje w bazie " << fileName << std::endl << "Miejsce: " << AddSeparator(line) << std::endl;
						return true;
					}
					onePassword = "";
					++line;
					if (line % 500000 == 0)
					{
						std::cout << ".";
					}
				}
				else
				{
					if (buffer[i] != '\r')
					{
						onePassword += buffer[i];
					}	
				}
				++loadedBytes;
			}
			/*
			delete buffer;
			startAt += oneGB;
			size_t numBytes = mmapFile.size() - oneGB;
			if (numBytes > oneGB)
			{
				numBytes = oneGB;
			}
			if (!mmapFile.remap(startAt, numBytes))
			{
				
			}*/
		//}
	}
	else
	{
		std::cout << "Nie udalo sie otworzyc pliku" << std::endl;
		return true;
	}
	std::cout <<"Liczba rekordow: "<< AddSeparator(line) << std::endl;
	mmapFile.close();
	return false;
}

bool continueProgram()
{
	char answer;
	std::cout << "Znalezc kolejne hasla [Y/N]: ";
	std::cin >> answer;
	if (answer == 'Y' || answer=='y')
	{
		return true;
	}
	return false;
}

int main()
{
	do {
		std::string passwordToFind;
		std::cout << "Wpisz haslo do wyszukania: ";
		std::cin >> passwordToFind;
		if (!findPassword("Top2Billion-probable-v2.txt", passwordToFind))
		{
			std::cout << std::endl << "Haslo jest bezpieczne - nie istnieje w bazie top 2mld"<<std::endl;
		}
	} while (continueProgram());
    return 0;
}

