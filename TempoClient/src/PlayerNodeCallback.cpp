#include "PlayerNodeCallback.h"

#include <osg/MatrixTransform>
#include "Global.h"

void PlayerNodeCallback::operator()(osg::Node* node, osg::NodeVisitor* nv) 
{
	osg::MatrixTransform *mt = static_cast<osg::MatrixTransform *>(node);
	osg::Matrix m;
	m.makeTranslate(position);
	mt->setMatrix(m);
}