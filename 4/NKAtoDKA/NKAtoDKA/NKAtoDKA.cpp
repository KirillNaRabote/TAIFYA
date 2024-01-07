#include <iostream>
#include <fstream>

#include "types.h"
#include "InputOutputHandler.h"
#include "FileHandler.h"

using namespace std;

template <typename T>
void PrintVector(vector<T> vec)
{
    for (const auto& el : vec)
    {
        cout << el << " ";
    }
    cout << endl;
}

int main(int argc, char* argv[])
{
    try
    {
        CheckInputParameters(argc);

        string inputFileName = argv[1];
        ifstream inputFile(inputFileName);

        string outputFileName = argv[2];
        ofstream outputFile(outputFileName);

        CheckFile(inputFile, inputFileName);
        CheckFile(outputFile, outputFileName);

        Table tableNKA = ReadTable(inputFile);
    }
    catch(const exception& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}