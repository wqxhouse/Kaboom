//#include "OsgObjectLoader.h"
//
//#include <osgDB/ReadFile>
//
//#include <osgDB/FileUtils>
//#include <osgDB/FileNameUtils>
//#include <osgDB/Options>
//#include <osgDB/XmlParser>
//
//OsgObjectLoader::OsgObjectLoader(std::unordered_map<std::string, Configuration> &config)
//	: config(config) {
//}
//
//void OsgObjectLoader::load(const std::string &filename) {
//    loadXMLFile(filename);
//}
//
//void OsgObjectLoader::loadXMLNode(osgDB::XmlNode *xmlRoot) {
//
//	if (xmlRoot->type == osgDB::XmlNode::ROOT) {
//		for (unsigned int i = 0; i < xmlRoot->children.size(); ++i) {
//			osgDB::XmlNode* xmlChild = xmlRoot->children[i];
//			if (xmlChild->name == "world")
//				return loadXMLNode(xmlChild);
//		}
//	}
//
//	for (unsigned int i = 0; i < xmlRoot->children.size(); ++i) {
//		osgDB::XmlNode* xmlChild = xmlRoot->children[i];
//		if (!isXMLNodeType(xmlChild)) continue;
//
//		const std::string& childName = xmlChild->name;
//		//std::cout << childName << std::endl;
//
//		if (childName == "model") {
//			createModelFromXML(xmlChild);
//		}
//	}
//}
//
//
//void OsgObjectLoader::createModelFromXML(osgDB::XmlNode* xmlNode) {
//	std::string name = xmlNode->properties["name"];
//	osg::ref_ptr<osg::Node> modelNode = nullptr;
//	osg::Vec3 position;
//	osg::Vec4 orientation;
//	osg::Vec3 scaleVec3;
//
//	osg::ref_ptr<osg::Node> transformNode;
//
//	for (unsigned int i = 0; i < xmlNode->children.size(); ++i) {
//
//		osgDB::XmlNode* xmlChild = xmlNode->children[i];
//		if (!isXMLNodeType(xmlChild)) {
//			continue;
//		}
//		const std::string& childName = xmlChild->name;
//
//		if (childName == "file") {
//			std::string fileName = "";
//			loadString(xmlChild, fileName);
//			modelNode = osgDB::readNodeFile(fileName);
//
//			if (modelNode == nullptr) {
//				std::cout << "model is null: " << name << std::endl;
//				return;
//			}
//
//			transformNode = new osg::MatrixTransform;
//			transformNode->addChild(modelNode);
//
//			config[name].set("node", transformNode);
//		}
//		else if (childName == "position") {
//			loadVec3(xmlChild, position);
//
//			osg::Matrix translate;
//			translate.makeTranslate(position);
//
//			transformNode = config[name].getPointer<osg::ref_ptr<osg::Node>>("node");
//			if (transformNode != nullptr) {
//				transformNode->setMatrix(translate);
//			}
//
//		}
//		else if (childName == "orientation") {
//			loadVec4(xmlChild, orientation);
//
//			osg::Quat quat = osg::Quat(orientation);
//
//			transformNode = config[name].getPointer<osg::MatrixTransform *>("node");
//			if (transformNode != nullptr) {
//
//				osg::Matrix mat = transformNode->getMatrix();
//
//				osg::Vec3 pos, scale;
//				osg::Quat rot, so;
//				mat.decompose(pos, rot, scale, so);
//
//				mat.makeTranslate(pos);
//				mat.preMult(osg::Matrix::rotate(quat));//give our rotation here
//				mat.preMult(osg::Matrix::scale(scale));
//
//				transformNode->setMatrix(mat);
//			}
//		}
//		else if (childName == "scale") {
//			loadVec3(xmlChild, scaleVec3);
//
//			transformNode = config[name].getPointer<osg::MatrixTransform *>("node");
//			if (transformNode != nullptr) {
//
//				osg::Matrix mat = transformNode->getMatrix();
//
//				osg::Vec3 pos, scale;
//				osg::Quat rot, so;
//				mat.decompose(pos, rot, scale, so);
//
//				mat.makeTranslate(pos);
//				mat.preMult(osg::Matrix::rotate(rot));
//				mat.preMult(osg::Matrix::scale(scaleVec3));//give our scaling here
//
//				transformNode->setMatrix(mat);
//			}
//		}
//	}
//}
