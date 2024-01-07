#pragma once
#include <string>
#include <vector>

const std::string RIGHT_GRAMMAR_STR = "right";
const std::string LEFT_GRAMMAR_STR = "left";

using VecOfStr = std::vector<std::string>;
using OutputSignalNewState = std::pair<std::string, std::string>;
using RulesForOneState = std::pair<std::string, std::vector<OutputSignalNewState>>;
using Grammar = std::vector<RulesForOneState>;
using TableNKA = std::vector<std::vector<std::string>>;