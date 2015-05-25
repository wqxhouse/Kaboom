#pragma once

#include <unordered_map>
#include <osg/Node>
#include <osg/MatrixTransform>

class Material;
class GeometryCache
{
public:
	GeometryCache();
	~GeometryCache();

	bool addModel(std::string fileName);
	bool addTypeId(const int type_id, std::string fileName, Material* material);
	bool addTypeId(const int type_id, std::string fileName, Material* material, osg::Matrix matrix);

	// If possible, use this function instead of osgDB::readNodeFile()
	osg::ref_ptr<osg::Node> getNodeByFileName(std::string fileName);
	osg::ref_ptr<osg::Node> getNodeById(const int type_id);
	osg::ref_ptr<osg::Node> getNodeById(const int type_id, std::string& fileName);
	Material* getMaterialById(const int type_id);
	osg::Matrix getMatrixById(const int type_id);

private:
	std::unordered_map<std::string, osg::ref_ptr<osg::Node>> _modelMap;
	std::unordered_map<int, std::string> _fileNameMap;
	std::unordered_map<int, Material *> _materialMap;
	std::unordered_map<int, osg::Matrix> _matrixMap;

};