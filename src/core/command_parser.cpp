#include "../../include/command_parser.hpp"
#include <sstream>

std::vector<std::string> tokenizeCommand(const std::string& input) {
    std::vector<std::string> tokens;
    std::stringstream stream(input);
    std::string token;

    while (stream >> token) {
        tokens.push_back(token);
    }

    return tokens;
}