#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "../include/RedisServer.h"
#include "../include/RedisDatabase.h"

int main(int argc, char *argv[]) {
    int port = 6379;  // default
    if (argc >= 2) port = std::stoi(argv[1]);

    if (RedisDatabase::getInstance().load("dump.my_rdb"))
        std::cout << "Database Loaded from dump.my_rdb\n";
    else
        std::cout << "No dump found or load failed; starting with empty db.\n";

    RedisServer server(port);

    // Background persistance: db snapshots every X sec
    std::thread persistanceThread([]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(60));
            // dump the database
            if (!RedisDatabase::getInstance().dump("dump.my_rdb"))
                std::cerr << "Error Duming Database\n";
            else
             std::cout << "Database Dumped to dump.my_rdb\n";
        }
    });

    persistanceThread.detach();

    server.run();
    return 0;
}
