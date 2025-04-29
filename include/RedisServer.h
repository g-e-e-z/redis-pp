#ifndef REDIS_SERVER_H
#define REDIS_SERVER_H

#include <atomic>
#include <string>

class RedisServer {
   public:
    RedisServer(int port);
    void run();
    void shutdown();

   private:
    int port;
    int server_socket;
    std::atomic<bool> running;

    // Signal handling for graceful shutdown (ctrl + c)
    void setupSignalHandler();
};
#endif  // !REDIS_SERVER_H
