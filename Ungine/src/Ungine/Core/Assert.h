#pragma once

#ifdef U_DEBUG
	#define U_ENABLE_ASSERTS
#endif

#ifdef U_ENABLE_ASSERTS
	#define U_ASSERT_NO_MESSAGE(condition) { if(!(condition)) { U_ERROR("Assertion Failed"); __debugbreak(); } }
	#define U_ASSERT_MESSAGE(condition, ...) { if(!(condition)) { U_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

	#define U_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro
	#define U_GET_ASSERT_MACRO(...) U_EXPAND_VARGS(U_ASSERT_RESOLVE(__VA_ARGS__, U_ASSERT_MESSAGE, U_ASSERT_NO_MESSAGE))

	#define U_ASSERT(...) U_EXPAND_VARGS( U_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
	#define U_CORE_ASSERT(...) U_EXPAND_VARGS( U_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#else
	#define U_ASSERT(...)
	#define U_CORE_ASSERT(...)
#endif
