#include "stdafx.h" 
#include "Core.h"
#include "Scene.h"

int main()
{
	Core::init(160, 80,		// winPosX, winPosY
			   1280, 720,		// winSizeX, winSizeY
			   1280, 720);		// renderBufferX, renderBufferY

	setupScene();
	Core::run();

	return 0;
}
