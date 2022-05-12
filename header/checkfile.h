#include <fstream>
#include <iostream>
#include <string>
using namespace std;

bool endsWith(string const &fileName, string const &type)
{
    if (fileName.length() >= type.length())
    {
        return (0 == fileName.compare(fileName.length() - type.length(), type.length(), type));
    }
    else
    {
        return false;
    }
}

bool checkFile(ifstream &file, string fileName)
{
    // transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
    if (file && endsWith(fileName, ".ts"))
    {
        char header[4];
        file.read(header, 4);
        if (header[0] == 'G') // checks sync byte
        {
            file.ignore(184);
            file.read(header, 4);
            if (header[0] == 'G')
            {
                file.seekg(0, file.beg);
                return true;
            }
        }
    }
    return false;
}
