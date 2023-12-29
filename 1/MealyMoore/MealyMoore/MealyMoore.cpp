#include "FileHandler.h"
#include "MealyMooreHandler.h"
#include "InputOutputHandler.h"
#include "Visualization.h"
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

        TransitionTable newTable = ConvertTable(tTable, type);
        unsigned newType = (type == MEALY_TYPE) ? MOORE_TYPE : MEALY_TYPE;

        PrintTransitionTableToFile(outputFile, newTable, newType);
        
        VisualizeGraph(tTable, type);
        VisualizeGraph(newTable, newType);
    }
    catch (const exception& ex)
    {
        std::cout << "Caught exception: " << endl << ex.what() << std::endl;
        return 1;
    }

    return 0;
}