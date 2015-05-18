#pragma once

#include <unordered_map>
#include <osg/Node>

class Material;
class GeometryCache
{
public:
	GeometryCache();
	~GeometryCache();

	bool addModel(std::string fileName);
	bool addTypeId(const int type_id, std::string fileName, Material* material);

	// If possible, use this function instead of osgDB::readNodeFile()
	osg::ref_ptr<osg::Node> getNodeByFileName(std::string fileName);
	osg::ref_ptr<osg::Node> getNodeById(const int type_id);
	osg::ref_ptr<osg::Node> getNodeById(const int type_id, std::string& fileName);
	Material* getMaterialById(const int type_id);

private:
	std::unordered_map<std::string, osg::ref_ptr<osg::Node>> _modelMap;
	std::unordered_map<int, std::string> _fileNameMap;
	std::unordered_map<int, Material *> _materialMap;

};