#include "stdafx.h"
#include "OnClickChampSelectListener.h"

OnClickChampSelectListener::~OnClickChampSelectListener()
{
}

void OnClickChampSelectListener::ProcessEvent(Rocket::Core::Event& event)
{	
	*charId = num;
	//for (int j = 0; j < nameElement->GetNumChildren(); j++){
	//	Rocket::Core::Element *robot = nameElement->GetChild(j);
	//	robot->SetClass("selected",false);
	//	if (robot->GetId() == Rocket::Core::String(std::to_string(num).c_str())){
	//		robot->SetClass("selected", true);
	//	}
	//}
	Rocket::Core::Element *red_robot = nameElement->GetChild(0);
	Rocket::Core::Element *green_robot = nameElement->GetChild(1);
	Rocket::Core::Element *blue_robot = nameElement->GetChild(2);
	Rocket::Core::Element *purple_robot = nameElement->GetChild(3);

	switch (num)
	{
		case 0:
			red_robot->SetClassNames("robo red_selected");
			green_robot->SetClassNames("robo green");
			blue_robot->SetClassNames("robo blue");
			purple_robot->SetClassNames("robo purple");
			break;
		case 1:
			red_robot->SetClassNames("robo red");
			green_robot->SetClassNames("robo green_selected");
			blue_robot->SetClassNames("robo blue");
			purple_robot->SetClassNames("robo purple");
			break;
		case 2:
			red_robot->SetClassNames("robo red");
			green_robot->SetClassNames("robo green");
			blue_robot->SetClassNames("robo blue_selected");
			purple_robot->SetClassNames("robo purple");
			break;
		case 3:
			red_robot->SetClassNames("robo red");
			green_robot->SetClassNames("robo green");
			blue_robot->SetClassNames("robo blue");
			purple_robot->SetClassNames("robo purple_selected");
			break;
		default:
			red_robot->SetClassNames("robo red");
			green_robot->SetClassNames("robo green");
			blue_robot->SetClassNames("robo blue");
			purple_robot->SetClassNames("robo purple");
			break;
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