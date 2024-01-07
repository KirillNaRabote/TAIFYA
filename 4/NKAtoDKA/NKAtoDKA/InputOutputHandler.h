#pragma once

#include <string>

void CheckInputParameters(int numOfParam)
{
    if (numOfParam != 3)
    {
        std::string msg = "Invalid parameters\nUsage: NKAtoDKA.exe <input.csv> <output.csv>";
        throw std::invalid_argument(msg);
    }
}

template <typename FileStream>
void CheckFile(FileStream& file, const std::string& fileName)
{
    if (!file.is_open())
    {
        std::string msg = "The file " + fileName + " could not be opened";
        throw std::ios_base::failure(msg);
    }
}