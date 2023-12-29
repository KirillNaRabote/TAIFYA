#include "FileHandler.h"
#include "InputOutputHandler.h"
#include "Visualization.h"
#include "MinimizationHandler.h"
#include "types.h"

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

        unsigned type;

        TransitionTable tTable;

        ReadFuncReturnableValue returnableValue = ReadTransitionTableFromFile(inputFile);

        tTable = returnableValue.table;
        type = returnableValue.type;

        TransitionTable newTable = MinimizeTable(tTable, type);

        PrintTransitionTableToFile(outputFile, newTable, type);

        /*VisualizeGraph(tTable, type);
        VisualizeGraph(newTable, type);*/
    }
    catch (const exception& ex)
    {
        std::cout << "Caught exception: " << endl << ex.what() << std::endl;
        return 1;
    }

    return 0;
}