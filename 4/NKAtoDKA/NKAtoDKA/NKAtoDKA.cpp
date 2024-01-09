#include <iostream>
#include <fstream>


#include "types.h"
#include "InputOutputHandler.h"
#include "CFileHandler.h"
#include "CNKAtoDKA.h"

using namespace std;

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

        Table tableNKA = CFileHandler::ReadTable(inputFile);

        Table tableDKA = CNKAtoDKA::ConvertNKAtoDKA(tableNKA);

        CFileHandler::PrintTable(outputFile, tableDKA);

        inputFile.close();
        outputFile.close();
    }
    catch(const exception& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}