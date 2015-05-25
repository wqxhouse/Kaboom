
#include "GeometryCache.h"

#include <osgDB/ReadFile>
#include "stdafx.h" 

GeometryCache::GeometryCache()
{

}

GeometryCache::~GeometryCache()
{

}

bool GeometryCache::addModel(std::string fileName)
{
	osg::ref_ptr<osg::Node> model = NULL;
	model = osgDB::readNodeFile(fileName);

	if (model.get() == NULL)
	{
		OSG_WARN << "addTypeId: model failed to load '" << fileName << "'" << std::endl;
		return false;
	}

	_modelMap.insert(std::make_pair(fileName, model));
	return true;
}

bool GeometryCache::addTypeId(const int type_id, std::string fileName, Material* material)
{
	if (material == NULL)
	{
		OSG_WARN << "addTypeId: No material for type id '" << type_id << "'" << std::endl;
	}
	else {
		_materialMap.insert(std::make_pair(type_id, material));
	}

	_fileNameMap.insert(std::make_pair(type_id, fileName));

	return addModel(fileName);
}

bool GeometryCache::addTypeId(const int type_id, std::string fileName, Material* material, osg::Matrix matrix)
{
	bool retVal = addTypeId(type_id, fileName, material);

	if (retVal) {
		_matrixMap.insert(std::make_pair(type_id, matrix));
	}

	return retVal;
}

osg::ref_ptr<osg::Node> GeometryCache::getNodeByFileName(std::string fileName)
{
	std::unordered_map<std::string, osg::ref_ptr<osg::Node>>::iterator itr = _modelMap.find(fileName);
	if (itr != _modelMap.end())
	{
		return itr->second;
	}
	
	// Load the model if the cache does not have it yet
	if (!addModel(fileName)) {
		return NULL;
	}

	std::unordered_map<std::string, osg::ref_ptr<osg::Node>>::iterator itr2 = _modelMap.find(fileName);
	if (itr2 == _modelMap.end())
	{
		OSG_WARN << "getNodeByFileName: failed to load and store model (should not go here)" << std::endl;
		return NULL;
	}
	return itr2->second;
}

osg::ref_ptr<osg::Node> GeometryCache::getNodeById(const int type_id)
{
	std::string unusedStr;
	return getNodeById(type_id, unusedStr);
}


osg::ref_ptr<osg::Node> GeometryCache::getNodeById(const int type_id, std::string& fileName)
{
	std::unordered_map<int, std::string>::iterator itr1 = _fileNameMap.find(type_id);
	if (itr1 == _fileNameMap.end())
	{
		OSG_WARN << "getNodeById: Id '" << type_id << "' has no file path" << std::endl;
		return NULL;
	}

	fileName = itr1->second;
	return getNodeByFileName(fileName);
}

osg::Matrix GeometryCache::getMatrixById(const int type_id)
{
	std::unordered_map<int, osg::Matrix>::iterator itr1 = _matrixMap.find(type_id);
	if (itr1 == _matrixMap.end())
	{
		OSG_WARN << "getMatrixNodeById: Id '" << type_id << "' has no matrix" << std::endl;
		//return NULL;
		return osg::Matrix::identity();
	}

	return itr1->second;
}

Material* GeometryCache::getMaterialById(const int type_id)
{
	std::unordered_map<int, Material *>::iterator itr = _materialMap.find(type_id);
	if (itr != _materialMap.end())
	{
		return itr->second;
	}
	else
	{
		OSG_WARN << "getMaterialById: Material for type id '" << type_id << "' not found" << std::endl;
		return NULL;
	}
}

