#pragma once

#include <string>
#include <unordered_map>
#include <osg/Node>

class Configuration {
public:
    bool has(const char *name) const;

    bool getBool(const char *name) const;
    int getInt(const char *name) const;
    unsigned int getUint(const char *name) const;
    float getFloat(const char *name) const;
    std::string getString(const char *name) const;
	osg::Vec3 getVec3(const char *name) const;
	osg::Vec4 getVec4(const char *name) const;

    template <typename T>
    T getPointer(const char *name) const {
        return static_cast<T>(properties.at(name).ptr);
    }

	

    void set(const char *name, bool value);
    void set(const char *name, int value);
    void set(const char *name, unsigned int value);
    void set(const char *name, float value);
    void set(const char *name, const char *value);
    void set(const char *name, void *value);
	void set(const char *name, osg::Vec3 vec3);
	void set(const char *name, osg::Vec4 vec4);

private:
    struct ValueType {
        bool b;
        int i;
        unsigned int ui;
        float f;
        char *s;
        void *ptr;
		osg::Vec3 vec3;
		osg::Vec4 vec4;
    };

    std::unordered_map<std::string, ValueType> properties;
};
