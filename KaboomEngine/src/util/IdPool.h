#pragma once

#include <unordered_set>

class IdPool {
public:
    IdPool() : next(0) {}

    unsigned int allocate() {
        while (allocated.count(next) > 0) {
            ++next;
        }

        allocated.insert(next);

        return next++;
    }

    inline void free(unsigned int id) {
        allocated.erase(id);
    }

private:
    unsigned int next;
    std::unordered_set<unsigned int> allocated;
};
