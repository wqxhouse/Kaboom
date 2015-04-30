#pragma once

#include <osg/ref_ptr>
#include <osg/Node>

#include <core/Component.h>

class SceneNodeComponent : public Component {
public:
    SceneNodeComponent(osg::ref_ptr<osg::Node> node);

    osg::ref_ptr<osg::Node> getNode() const;
    void setNode(osg::ref_ptr<osg::Node> node);

private:
    osg::ref_ptr<osg::Node> node;
};
