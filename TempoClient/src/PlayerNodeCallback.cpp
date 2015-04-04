#include "PlayerNodeCallback.h"

#include <osg/MatrixTransform>
#include "Global.h"

void PlayerNodeCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)  {
	if (movingForward) {
		position.y() += 0.05;
	}

	if (movingBackward) {
		position.y() -= 0.05;
	}

	if (movingLeft) {
		position.x() -= 0.05;
	}

	if (movingRight) {
		position.x() += 0.05;
	}

	osg::MatrixTransform *mt = static_cast<osg::MatrixTransform *>(node);
	osg::Matrix m;
	m.makeTranslate(position);
	mt->setMatrix(m);
}