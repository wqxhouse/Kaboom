#include "XMLLoader.h"

#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osgDB/XmlParser>

void XMLLoader::loadXMLFile(const std::string &filePath)
{
	osg::ref_ptr<osgDB::XmlNode> xmlRoot = osgDB::readXmlFile(filePath);
	if (xmlRoot.valid())
	{
		osgDB::FilePathList& filePaths = osgDB::getDataFilePathList();
		filePaths.push_back(osgDB::getFilePath(filePath));
		loadXMLNode(xmlRoot.get());
		filePaths.pop_back();
	}
	else
	{
		// TODO : output to console bad xml
		OSG_WARN << "Bad xml at " << filePath << std::endl;
	}
}

bool XMLLoader::isXMLNodeType(osgDB::XmlNode* xmlNode)
{
	switch (xmlNode->type)
	{
	case osgDB::XmlNode::ATOM:
	case osgDB::XmlNode::NODE:
	case osgDB::XmlNode::GROUP:
		return true;
	default:
		return false;
	}
}

void XMLLoader::setDefaultString(std::string &s) {
	s = s.empty() ? "default" : s;
}

void XMLLoader::loadInt(osgDB::XmlNode* xmlNode, int& i) {
	std::stringstream ss; ss << xmlNode->getTrimmedContents();
	ss >> i;
}

void XMLLoader::loadUint(osgDB::XmlNode* xmlNode, unsigned int& i) {
    std::stringstream ss; ss << xmlNode->getTrimmedContents();
    ss >> i;
}

void XMLLoader::loadFloat(osgDB::XmlNode* xmlNode, float& f) {
	std::stringstream ss; ss << xmlNode->getTrimmedContents();
	ss >> f;
}

void XMLLoader::loadBool(osgDB::XmlNode* xmlNode, bool& b) {
	std::stringstream ss; ss << xmlNode->getTrimmedContents();
	ss >> b;
}

void XMLLoader::loadString(osgDB::XmlNode* xmlNode, std::string& s) {
	std::stringstream ss; ss << xmlNode->getTrimmedContents();
	ss >> s;
}

void XMLLoader::loadVec3(osgDB::XmlNode* xmlNode, osg::Vec3& vec) {
	std::stringstream ss; ss << xmlNode->getTrimmedContents();

	// Read in a vector of floats
	for (int n = 0; n < vec.num_components; ++n)
	{
		float v = 0.0f; ss >> v;
		vec[n] = v;
	}
}

void XMLLoader::loadVec4(osgDB::XmlNode* xmlNode, osg::Vec4& vec) {
	std::stringstream ss; ss << xmlNode->getTrimmedContents();

	// Read in a vector of floats
	for (int n = 0; n < vec.num_components; ++n)
	{
		float v = 0.0f; ss >> v;
		vec[n] = v;
	}
}
