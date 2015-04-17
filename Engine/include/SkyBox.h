/* -*-c++-*- OpenSceneGraph Cookbook
 * Chapter 6 Recipe 7
 * Author: Wang Rui <wangray84 at gmail dot com>
*/

#ifndef H_COOKBOOK_CH6_SKYBOX
#define H_COOKBOOK_CH6_SKYBOX

#include <osg/TextureCubeMap>
#include <osg/Transform>
#include <osg/NodeCallback>

class SkyBoxCallback : public osg::NodeCallback
{
public:
	SkyBoxCallback(osg::Group *geomRoot)
		: _geomRoot(geomRoot) {}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::Geode *skySphere = static_cast<osg::Geode *>(node);
		osg::ShapeDrawable *sd = static_cast<osg::ShapeDrawable *>(skySphere->getDrawable(0));
		osg::Sphere *sphere = static_cast<osg::Sphere *>(sd->getShape());
		sphere->setRadius(_geomRoot->getBound().radius());
	}

private:
	osg::ref_ptr<osg::Group> _geomRoot;
};

class SkyBox : public osg::Transform
{
public:
    SkyBox();
    SkyBox( const SkyBox& copy, osg::CopyOp copyop=osg::CopyOp::SHALLOW_COPY )
    : osg::Transform(copy, copyop) {}
    
    META_Node( osg, SkyBox );
    
    void setEnvironmentMap( unsigned int unit, osg::Image* posX, osg::Image* negX,
                            osg::Image* posY, osg::Image* negY, osg::Image* posZ, osg::Image* negZ );

	void setGeomRoot(osg::Group *geomRoot);
    
    virtual bool computeLocalToWorldMatrix( osg::Matrix& matrix, osg::NodeVisitor* nv ) const;
    virtual bool computeWorldToLocalMatrix( osg::Matrix& matrix, osg::NodeVisitor* nv ) const;

protected:
	osg::ref_ptr<osg::Geode> _skySphere;
    virtual ~SkyBox() {}
};

#endif
