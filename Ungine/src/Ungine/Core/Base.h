#pragma once

#include <memory>
#include "Log.h"

namespace U {

	void InitializeCore();
	void ShutdownCore();

}

#ifdef U_DEBUG
	#define U_ENABLE_ASSERTS
#endif

#ifdef U_PLATFORM_WINDOWS
#if U_DYNAMIC_LINK
	#ifdef U_BUILD_DLL
		#define Ungine_API __declspec(dllexport)
	#else
		#define Ungine_API __declspec(dllimport)
	#endif
#else
	#define Ungine_API
#endif
#else
	#error Ungine only supports Windows!
#endif

#ifdef U_ENABLE_ASSERTS

#define U_ASSERT_NO_MESSAGE(condition) { if(!(condition)) { U_ERROR("Assertion Failed!"); __debugbreak(); } }
#define U_ASSERT_MESSAGE(condition, ...) { if(!(condition)) { U_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#define U_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro

#define U_ASSERT(...) U_ASSERT_RESOLVE(__VA_ARGS__, U_ASSERT_MESSAGE, U_ASSERT_NO_MESSAGE)(__VA_ARGS__)
#define U_CORE_ASSERT(...) U_ASSERT_RESOLVE(__VA_ARGS__, U_ASSERT_MESSAGE, U_ASSERT_NO_MESSAGE)(__VA_ARGS__)

#else
#define U_ASSERT(...)
#define U_CORE_ASSERT(...)

#endif

#define BIT(x) (1 << x)

#define U_BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)

// Pointer wrappers
namespace U {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	using byte = unsigned char;

}
