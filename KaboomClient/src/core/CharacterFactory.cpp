#include "CharacterFactory.h"

#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <components/HealthComponent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/EntityManager.h>
#include "../components/SceneNodeComponent.h"
#include "Model.h"
#include "util/ConfigSettings.h"

CharacterFactory::CharacterFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *CharacterFactory::createCharacter(
        unsigned int id,
        EntityType characterType,
        float x,
        float y,
        float z,
        Quat rotation) const {
    Entity *entity = entityManager.createEntity(id, characterType);
	/*osg::ref_ptr<osg::Capsule> capsule = new osg::Capsule(osg::Vec3(), 0.5f, 1.0f);
	osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(capsule);
    osg::ref_ptr<osg::Geode> model = new osg::Geode;
    model->addDrawable(drawable);

    osg::ref_ptr<osg::MatrixTransform> transformation = new osg::MatrixTransform;
    transformation->addChild(model);

    osg::ref_ptr<osg::Group> playerNode = new osg::Group;

    playerNode->addChild(transformation);*/
	ConfigSettings* config = ConfigSettings::config;
	std::string str_mediaPath = "";
	std::string str_char_model = "";
	config->getValue(ConfigSettings::str_mediaFilePath, str_mediaPath);
	config->getValue(ConfigSettings::str_char_model, str_char_model);
	std::string str_model = str_mediaPath + str_char_model;
	Model *model = new Model(str_model);

	// Rotate the model to fix axes alignment
	osg::Matrix transformMat;
	transformMat *= transformMat.rotate(osg::DegreesToRadians(90.0), osg::Vec3(1, 0, 0));
	transformMat *= transformMat.rotate(osg::DegreesToRadians(180.0), osg::Vec3(0, 0, 1));
	transformMat *= transformMat.translate(osg::Vec3(0, 0, 3.5f));

	// Scale
	float scaleVal = 0.12f;
	transformMat *= transformMat.scale(scaleVal, scaleVal, scaleVal);

	model->getRootNode()->setMatrix(transformMat);

	entity->attachComponent(new SceneNodeComponent(model->getRootNode()));
    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new RotationComponent(rotation));
	entity->attachComponent(new HealthComponent(100,100));

    return entity;
}
