#include "WorldLoader.h"

#include <btBulletDynamicsCommon.h>

#include <osgbCollision/CollisionShapes.h>
#include <osgbDynamics/MotionState.h>
#include <osgDB/ReadFile>

#include <util/Configuration.h>

#include "World.h"
#include "../debug/OsgObjectConfigLoader.h"

WorldLoader::WorldLoader(World &world)
        : world(world) {
}

void WorldLoader::load(const std::string &filename, const std::string &mediaPath) {
    std::unordered_map<std::string, Configuration> osgNodeConfigMap;

    OsgObjectConfigLoader osgObjectConfigLoader(osgNodeConfigMap);
    osgObjectConfigLoader.load(filename);

    for (auto kv : osgNodeConfigMap) {
        Configuration osgObjectConfig = kv.second;

        std::string filePath = osgObjectConfig.getString("file");
        osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(mediaPath + filePath);
        osg::ref_ptr<osg::MatrixTransform> transfromNode = new osg::MatrixTransform;
        transfromNode->addChild(node);

        osg::Matrix mat;
        mat.makeTranslate(osgObjectConfig.getVec3("position"));
        mat.preMult(osg::Matrix::rotate(osg::Quat(osgObjectConfig.getVec4("orientation"))));
        mat.preMult(osg::Matrix::scale(osgObjectConfig.getVec3("scale")));

        transfromNode->setMatrix(mat);

        osgbDynamics::MotionState *motion = new osgbDynamics::MotionState;
        motion->setTransform(transfromNode);

        btCollisionShape *collisionShape = osgbCollision::btTriMeshCollisionShapeFromOSG(transfromNode);
        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, motion, collisionShape, btVector3(0, 0, 0));
        rigidBodyCI.m_restitution = 1.0f;

        btRigidBody *rigidbody = new btRigidBody(rigidBodyCI);
        world.addRigidBody(rigidbody);
    }
}
