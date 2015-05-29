#pragma once

#include <osg/ref_ptr>
#include <osg/Node>

#include <components/Component.h>

class SceneNodeComponent : public Component {
public:
    SceneNodeComponent(osg::ref_ptr<osg::Node> node)
            : node(node) {
    }

    inline osg::ref_ptr<osg::Node> getNode() const {
        return node;
    }

    inline void setNode(osg::ref_ptr<osg::Node> node) {
        this->node = node;
    }

private:
    osg::ref_ptr<osg::Node> node;
};
