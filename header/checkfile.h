#include<fstream>
#include<iostream>
#include<string>
using namespace std;

bool hasEnding (string const &fname, string const &type)
{
    if (fname.length() >= type.length())
    {
        return (0 == fname.compare (fname.length() - type.length(), type.length(), type));
    }
    else
    {
        return false;
    }
}


bool check(ifstream& f,string fname)
{
    //transform(fname.begin(), fname.end(), fname.begin(), ::tolower);
    if(f && hasEnding(fname,".ts"))
    {
        char a[4];
        f.read(a,4);
        if(a[0]=='G')
        {
            f.ignore(184);
            f.read(a,4);
            if(a[0]=='G')
            {
                f.seekg (0, f.beg);
                return true;
            }
        }
    }
    return false;
}
