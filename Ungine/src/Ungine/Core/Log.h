#pragma once

#include "Ungine/Core/Base.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace U {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core Logging Macros
#define U_CORE_TRACE(...)	U::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define U_CORE_INFO(...)	U::Log::GetCoreLogger()->info(__VA_ARGS__)
#define U_CORE_WARN(...)	U::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define U_CORE_ERROR(...)	U::Log::GetCoreLogger()->error(__VA_ARGS__)
#define U_CORE_FATAL(...)	U::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client Logging Macros
#define U_TRACE(...)	U::Log::GetClientLogger()->trace(__VA_ARGS__)
#define U_INFO(...)	U::Log::GetClientLogger()->info(__VA_ARGS__)
#define U_WARN(...)	U::Log::GetClientLogger()->warn(__VA_ARGS__)
#define U_ERROR(...)	U::Log::GetClientLogger()->error(__VA_ARGS__)
#define U_FATAL(...)	U::Log::GetClientLogger()->critical(__VA_ARGS__)