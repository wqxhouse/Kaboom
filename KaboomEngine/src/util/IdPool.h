#pragma once

#include <unordered_set>

template <typename T>
class IdPool {
public:
    IdPool() : next(0) {
    }

    unsigned int allocate() {
        while (allocated.count(next) > 0) {
            ++next;
        }

        allocated.insert(next);

        return next++;
    }

    inline void free(T id) {
        allocated.erase(id);
    }

private:
    T next;
    std::unordered_set<T> allocated;
};
