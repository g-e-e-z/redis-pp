#ifndef REDIS_DATABASE_H
#define REDIS_DATABASE_H

#include <chrono>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

class RedisDatabase {
   public:
    // Get singleton instance
    static RedisDatabase& getInstance();

    // Common Commands
    bool flushAll();

    // Key/Value Operations
    void set(const std::string& key, const std::string& value);
    bool get(const std::string& key, std::string& value);
    // bool get(const std::string& key);
    std::vector<std::string> keys();
    std::string type(const std::string& key);
    bool del(const std::string& key);
    bool expire(const std::string& key, int seconds);
    bool rename(const std::string& oldKey, const std::string& newKey);

    // Persistance: Dump/ load db from file
    bool dump(const std::string& filename);
    bool load(const std::string& filename);

   private:
    RedisDatabase() = default;
    ~RedisDatabase() = default;
    RedisDatabase(const RedisDatabase&) = delete;
    RedisDatabase& operator=(const RedisDatabase&) = delete;

    std::mutex db_mutex;
    std::unordered_map<std::string, std::string> kv_store;
    std::unordered_map<std::string, std::vector<std::string>> list_store;
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::string>>
        hash_store;

    std::unordered_map<std::string, std::chrono::steady_clock::time_point>
        expiry_map;
};

#endif  // !REDIS_DATABASE_H
