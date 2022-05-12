#include "header/convert.h"
#include "header/tsheader.h"
#include "header/checkfile.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		string fileName = argv[1];
		ifstream file(fileName, ios::in | ios::binary);
		if (checkFile(file, fileName))
		{
			int count = 0;
			while (!file.eof())
			{
				int h;
				++count;
				file.read(reinterpret_cast<char *>(&h), sizeof(int));
				string s = to_string(h);
				char const *pchar = s.c_str();
				Header h1(pchar, sizeof(pchar));
				h1.display();
				file.ignore(184); // ignores payload
			}
			cout << "no of packets: " << count << endl;
		}
		else
		{
			cout << "invalid file" << endl;
			return 0;
		}
	}

	return 0;
}

/*
uint32_t myint;
while(fstr.read(reinterpret_cast<char*>(&myint), sizeof(myint))) {
	// do something with myint
}
*/
