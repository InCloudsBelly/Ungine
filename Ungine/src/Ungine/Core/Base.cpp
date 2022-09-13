#include "Upch.h"
#include "Base.h"

#include "Log.h"

#define Ungine_BUILD_ID "v0.1a"

namespace U {

	void InitializeCore()
	{
		U::Log::Init();

		U_CORE_TRACE("Ungine{}", Ungine_BUILD_ID);
		U_CORE_TRACE("Initializing...");
	}

	void ShutdownCore()
	{
		U_CORE_TRACE("Shutting down...");
	}

}