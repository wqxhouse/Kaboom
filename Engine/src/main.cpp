#include "stdafx.h" 
#include "Core.h"
#include "Scene.h"

int main()
{
	Core::init(800, 600, 800, 600);
	setupScene();

	Core::run();

	return 0;
}
