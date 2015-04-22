#include "CharacteristicComponent.h"

CharacteristicComponent::CharacteristicComponent(EntityType type, int feature,int owner):
type(type),
feature(feature),
owner(owner)
{
}
CharacteristicComponent::CharacteristicComponent() :
type(UNINITIATED),
feature(0),
owner(0)
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
void CharacteristicComponent::setOwner(int o){
	owner = o;
}
int CharacteristicComponent::getOwner(){
	return owner;
}