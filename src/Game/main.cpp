#include "stdafx.h"
#include "GameApp.h"
#include "GameAppBullet.h"
//-----------------------------------------------------------------------------
#if defined(_MSC_VER)
#	pragma comment( lib, "Engine.lib" )
#endif
//-----------------------------------------------------------------------------
int main(
	[[maybe_unused]] int   argc,
	[[maybe_unused]] char* argv[])
{
	auto engineDevice = EngineDevice::Create({});
	//engineDevice->RunApp(std::make_shared<GameApp>());
	engineDevice->RunApp(std::make_shared<GameAppBullet>());

}
//-----------------------------------------------------------------------------