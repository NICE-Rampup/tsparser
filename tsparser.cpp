/*

*/

#include "header/convert.h"
#include "header/tsheader.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		int count = 0;
		fstream file(argv[1], ios::in | ios::binary);
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

	return 0;
}

/*
uint32_t myint;
while(fstr.read(reinterpret_cast<char*>(&myint), sizeof(myint))) {
	// do something with myint
}

*/
