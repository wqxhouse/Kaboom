#include "Configuration.h"

bool Configuration::has(const char *name) const {
    return properties.count(name) > 0;
}

bool Configuration::getBool(const char *name) const {
    return properties.at(name).b;
}

int Configuration::getInt(const char *name) const {
    return properties.at(name).i;
}

unsigned int Configuration::getUint(const char *name) const {
    return properties.at(name).ui;
}

float Configuration::getFloat(const char *name) const {
    return properties.at(name).f;
}

std::string Configuration::getString(const char *name) const {
    return std::string(properties.at(name).s);
}

template <typename T>
T Configuration::getPointer(const char *name) const {
    return static_cast<T>(properties.at(name).ptr);
}

void Configuration::set(const char *name, bool value) {
    properties[name].b = value;
}

void Configuration::set(const char *name, int value) {
    properties[name].i = value;
}

void Configuration::set(const char *name, unsigned int value) {
    properties[name].ui = value;
}

void Configuration::set(const char *name, float value) {
    properties[name].f = value;
}

void Configuration::set(const char *name, const char *value) {
    if (has(name)) {
        delete[] properties[name].s;
    }

    int len = strnlen_s(name, 256);
    char *szValue = new char[len];

    strncpy_s(szValue, len, value, len);
    properties[name].s = szValue;
}

void Configuration::set(const char *name, void *value) {
    properties[name].ptr = value;
}
