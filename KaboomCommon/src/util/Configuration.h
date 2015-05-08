#pragma once

#include <string>
#include <unordered_map>

class Configuration {
public:
    bool has(const char *name) const;

    bool getBool(const char *name) const;
    int getInt(const char *name) const;
    unsigned int getUint(const char *name) const;
    float getFloat(const char *name) const;
    std::string getString(const char *name) const;

    template <typename T>
    T getPointer(const char *name) const;

    void set(const char *name, bool value);
    void set(const char *name, int value);
    void set(const char *name, unsigned int value);
    void set(const char *name, float value);
    void set(const char *name, const char *value);
    void set(const char *name, void *value);

private:
    union ValueType {
        bool b;
        int i;
        unsigned int ui;
        float f;
        char *s;
        void *ptr;
    };

    std::unordered_map<std::string, ValueType> properties;
};
