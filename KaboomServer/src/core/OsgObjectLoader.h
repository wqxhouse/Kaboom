//#pragma once
//
//#include <unordered_map>
//
//#include <util/Configuration.h>
//#include <util/XMLLoader.h>
//
//#include <osg/Node>
//#include <osgDB/XmlParser>
//
//#include <osg/MatrixTransform>
//
//class OsgObjectLoader : public XMLLoader {
//public:
//	OsgObjectLoader(std::unordered_map<std::string, Configuration> &config);
//
//    void load(const std::string &filename);
//
//
//private:
//	std::unordered_map<std::string, Configuration> &config;
//
//    virtual void loadXMLNode(osgDB::XmlNode *xmlRoot);
//
//	void createModelFromXML(osgDB::XmlNode* xmlNode);
//
//	osg::ref_ptr<osg::Node> getNodeFromMap(const std::string name);
//
//};
