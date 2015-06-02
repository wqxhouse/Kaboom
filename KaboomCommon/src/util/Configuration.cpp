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
    return properties.at(name).s;
}

osg::Vec3 Configuration::getVec3(const char *name) const {
	return properties.at(name).vec3;
}

osg::Vec4 Configuration::getVec4(const char *name) const {
	return properties.at(name).vec4;
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

void Configuration::set(const char *name, const std::string &value) {
    properties[name].s = value;
}
void Configuration::set(const char *name, void *value) {
    properties[name].ptr = value;
}

void Configuration::set(const char *name, osg::Vec3 value) {
	properties[name].vec3 = value;
}

void Configuration::set(const char *name, osg::Vec4 value) {
	properties[name].vec4 = value;
}


