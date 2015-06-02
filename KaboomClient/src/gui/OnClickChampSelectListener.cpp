#include "stdafx.h"
#include "OnClickChampSelectListener.h"

OnClickChampSelectListener::~OnClickChampSelectListener()
{
}

void OnClickChampSelectListener::ProcessEvent(Rocket::Core::Event& event)
{	
	*charId = num;
	for (unsigned int j = 0; j < nameElement->GetNumChildren(); j++){
		Rocket::Core::Element *tr = nameElement->GetChild(j);
		if (j == 1)
			j++;
		for (int k = 0; k < nameElement->GetNumChildren(); k++){
			Rocket::Core::Element *child = nameElement->GetChild(k);
			child->SetProperty("color", "red");
			if (child->GetId() == Rocket::Core::String(std::to_string(num).c_str())){
				child->SetProperty("color", "black");
			}
		}
	}
	for (int i = 0; i < nameElement->GetNumChildren(); i++){


	}
	std::cout << "char Id should be the value "<<num<<" it is the value  "<<charId << std::endl;
}
void OnClickChampSelectListener::setNumber(unsigned int &a,unsigned int number){
	charId = &a;
	num = number;
}
unsigned int* OnClickChampSelectListener::getNumber(){
	return charId;
}
void OnClickChampSelectListener::setNameElement(Rocket::Core::Element *doug){
	nameElement = doug;
}