#include "../include/RedisCommandHandler.h"
#include "../include/RedisDatabase.h"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// RESP parser:
// *2\r\n$4\r\n\PING\r\n$4\r\nTEST\r\n
// *2 -> array has 2 elements
// $4 -> next string has 4 characters
// PING
// TEST

std::vector<std::string> parserRespCommand(const std::string &input) {
    std::vector<std::string> tokens;
    if (input.empty()) return tokens;

    // If it doesnt begin with *, fallback to splitting by whitespaces
    if (input[0] != '*') {
        std::istringstream iss(input);
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    size_t pos = 0;
    // Expect '*' followed by number of elements
    if (input[pos] != '*') return tokens;
    pos++; // skip '*':-

    // crlf = Carriage Return (\r), Line Feed (\n)
    size_t crlf = input.find("\r\n", pos);
    if (crlf == std::string::npos) return tokens;

    int numElements = std::stoi(input.substr(pos, crlf - pos));
    pos = crlf + 2;

    for (int i = 0; i < numElements; i++) {
        if (pos >= input.size() || input[pos] != '$') break; // format error
        pos++; // skip '$'

        crlf = input.find("\r\n", pos);
        if (crlf == std::string::npos) break;
        int len = std::stoi(input.substr(pos, crlf - pos));
        pos = crlf + 2;

        if (pos + len > input.size()) break;
        std::string token = input.substr(pos, len);
        tokens.push_back(token);
        pos += len + 2; // skip token and CRLF
    }
    return tokens;
}

RedisCommandHandler::RedisCommandHandler() {}

std::string RedisCommandHandler::processCommand(const std::string& commandLine) {
    // Use RESP parser
    std::vector<std::string> tokens = parserRespCommand(commandLine);
    if (tokens.empty()) return "-Error: Empty command\r\n";

    // for (auto& t : tokens) {
    //     std::cout << t << "\n";
    // }

    std::string cmd = tokens[0];
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
    std::ostringstream response;

    // Connect to database
    RedisDatabase& db = RedisDatabase::getInstance();

    // Check commands
    if (cmd == "PING") {
        response << "+PONG\r\n";
    } else if (cmd == "ECHO") {
        // ....
    } else {
        response << "-Error: Unknown command\r\n";
    }

    return response.str();
}
