#include "CharacteristicComponent.h"

CharacteristicComponent::CharacteristicComponent(EntityType type, int feature):
type(type),
feature(feature)
{
}
CharacteristicComponent::CharacteristicComponent() :
type(UNINITIATED),
feature(0)
{
}
CharacteristicComponent::~CharacteristicComponent()
{
}
void CharacteristicComponent::setType(EntityType lype){
	type = lype;
}
EntityType CharacteristicComponent::getType(){
	return type;
}
void CharacteristicComponent::setFeature(int i){
	feature = i;
}
int CharacteristicComponent::getFeature(){
	return feature;
}
