#pragma once 

class GeometryPicker : public osgGA::GUIEventHandler
{
public:
	bool GeometryPicker::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	void GeometryPicker::pick(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

private:
	float _mX;
	float _mY;

};