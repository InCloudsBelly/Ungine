#pragma once

#ifdef U_PLATFORM_WINDOWS

extern U::Application* U::CreateApplication();

int main(int argc, char** argv)
{
	U::InitializeCore();
	U::Application* app = U::CreateApplication();
	U_CORE_ASSERT(app, "Client Application is null!");
	app->Run();
	delete app;
	U::ShutdownCore();
}

#endif
