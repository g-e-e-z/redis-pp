#ifndef REDIS_DATABASE_H
#define REDIS_DATABASE_H

#include <string>

class RedisDatabase {
   public:
    // Get singleton instance
    static RedisDatabase& getInstance();

    // Persistance: Dump/ load db from file
    bool dump(const std::string& filename);
    bool load(const std::string& filename);

   private:
    RedisDatabase() = default;
    ~RedisDatabase() = default;
    RedisDatabase(const RedisDatabase&) = delete;
    RedisDatabase& operator=(const RedisDatabase&) = delete;
};

#endif  // !REDIS_DATABASE_H
